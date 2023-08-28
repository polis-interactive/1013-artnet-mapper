//
// Created by brucegoose on 7/5/23.
//

#include <iostream>

#include "graphics.hpp"

namespace infrastructure {

    GraphicsPtr Graphics::Create(const GraphicsConfig &config, GraphicsManagerPtr manager) {
        auto graphics = std::make_shared<Graphics>(config, std::move(manager));
        return std::move(graphics);
    }

    Graphics::Graphics(const GraphicsConfig &config, GraphicsManagerPtr manager):
        _config(config.installation_config),
        _display_shader(config.display.shader, true),
        _pixel_type_texture(
            _config.dimensions, config.display.pixel_texture, graphics::ImageTextureTypes::R8,
            "pixel_type_texture", 1
        ),
        _artnet_texture(
            _config.dimensions, config.display.artnet_texture, graphics::ImageTextureTypes::RGB8,
            "artnet_texture", 2
        ),
        _renderer(graphics::Renderer::Create(
            config.display.render_type, _config.dimensions, config.display.pixel_multiplier,
            config.installation_config.rgbw_pixels
        )),
        _pixel_multiplier(graphics::IntUniform::Create(
           "pixel_multiplier", (int) _config.pixel_types
        )),
        _resolution(graphics::Float2Uniform::Create(
            "resolution", (float) _config.dimensions.width, (float) _config.dimensions.height
        )),
        _do_artnet_mapping(graphics::BoolUniform::Create(
            "do_artnet_mapping", config.display.render_type == domain::RendererType::HEADLESS
        )),
        _display_uniforms({_time, _brightness, _pixel_multiplier, _resolution, _do_artnet_mapping }),
        _frameTime(1 / _config.fps),
        _render_art_net(config.display.render_art_net),
        _manager(std::move(manager))
    {
        for (int i = 0; i < _config.buffer_count; i++) {
            auto buffer = new CpuPixelBuffer(_config.dimensions, _config.rgbw_pixels);
            _cpu_buffers.push_back(buffer);
        }
    }

    Graphics::~Graphics() {
        teardown();
    }

    void Graphics::Start() {
        if (!_stop_running) {
            return;
        }
        _stop_running = false;
        auto self(shared_from_this());
        _graphics_thread = std::make_unique<std::jthread>([this, self](const std::stop_token& st) {
            run(st);
        });
    }

    void Graphics::Stop() {
        if (_stop_running) {
            return;
        }
        _stop_running = true;
        if (_graphics_thread != nullptr) {
            _graphics_thread->request_stop();
            _graphics_thread->join();
        }
        _graphics_thread = nullptr;
    }

    void Graphics::PostBrightness(const float new_brightness) {
        _atm_brightness.store(new_brightness);
    }

    void Graphics::run(const std::stop_token &st) {
        bool has_run = false;
        while (!st.stop_requested()) {
            if (has_run) {
                std::cout << "Graphics::run: main loop failed; waiting and trying again..." << std::endl;
            }
            has_run = true;
            setup();
            _is_ready = true;
            if (_render_art_net) {
                runGraphicsArtNet(st);
            } else {
                runGraphics(st);
            }
            _is_ready = false;
            teardown();
        }
    }

    bool Graphics::setup() {
        auto success = _renderer->SetupContext();
        if (!success) {
            return success;
        }
        _display_shader.Setup();
        _pixel_type_texture.Setup();
        _pixel_type_texture.SetLocation(_display_shader._program);
        _artnet_texture.Setup();
        _artnet_texture.SetLocation(_display_shader._program);
        graphics::Uniform::SetupUniforms(_display_uniforms, _display_shader._program);

        auto self(shared_from_this());
        _renderer->Setup(self);

        return true;
    }

    void Graphics::runGraphicsArtNet(const std::stop_token &st) {
        auto self(shared_from_this());
        unsigned int consecutive_failed_frames = 0;
        auto run_start = utility::Clock::now();
        while (!st.stop_requested()) {
            auto frame_start = utility::Clock::now();
            auto frame_time = std::chrono::duration<float, std::chrono::seconds::period>(frame_start - run_start);
            _time->SetValue(frame_time.count());
            _brightness->SetValue(_atm_brightness.load());
            if (!renderNextFrame(self)) {
                consecutive_failed_frames++;
            } else {
                consecutive_failed_frames = 0;
            }
            // check frame rate, timeout
            auto frame_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedFrame = frame_end - frame_start;
            if (elapsedFrame < _frameTime)
            {
                std::this_thread::sleep_for(_frameTime - elapsedFrame);
            }
            // here I need to handle threshold fps, as well as missed frames
        }
    }

    void Graphics::runGraphics(const std::stop_token &st) {
        auto self(shared_from_this());
        unsigned int consecutive_failed_frames = 0;
        while (!st.stop_requested()) {
            auto start = std::chrono::high_resolution_clock::now();
            _renderer->Render(self, nullptr);
            // check frame rate, timeout
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedTime = end - start;
            if (elapsedTime < _frameTime)
            {
                std::this_thread::sleep_for(_frameTime - elapsedTime);
            }
            // here I need to handle threshold fps, as well as missed frames
        }
    }

    bool Graphics::renderNextFrame(GraphicsPtr &self) {
        CpuPixelBuffer *cpu_buffer = nullptr;
        {
            std::lock_guard<std::mutex> lk(_cpu_buffers_mutex);
            if (!_cpu_buffers.empty()) {
                cpu_buffer = _cpu_buffers.front();
                _cpu_buffers.pop_front();
            }
        }
        if (cpu_buffer == nullptr) {
            return false;
        }
        _renderer->Render(self, cpu_buffer);
        auto post_buffer = std::shared_ptr<CpuPixelBuffer>(
            cpu_buffer, std::bind_front(&Graphics::requeueBuffer, shared_from_this())
        );
        _manager->PostGraphicsUpdate(std::move(post_buffer));
        return true;
    }


    void Graphics::requeueBuffer(CpuPixelBuffer *reclaim_buffer) {
        std::unique_lock<std::mutex> lock(_cpu_buffers_mutex);
        _cpu_buffers.push_back(reclaim_buffer);
    }

    void Graphics::teardown() {
        _artnet_texture.Teardown();
        _pixel_type_texture.Teardown();
        _display_shader.Teardown();
        _renderer->Teardown();
    }

    CpuPixelBuffer::CpuPixelBuffer(const domain::Dimensions &dimensions, const bool is_rgbw):
        _width((int) dimensions.width),
        _height((int) dimensions.height),
        _format(is_rgbw ? GL_RGBA : GL_RGB),
        _size(_width * _height * (is_rgbw ? 4 : 3)),
        _pixels(_size, 0)
    {}

    uint8_t *CpuPixelBuffer::GetMemory() {
        return _pixels.data();
    }

    std::size_t CpuPixelBuffer::GetSize() {
        return _size;
    }

    void CpuPixelBuffer::RenderBuffer() const {
        glReadPixels(0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, (void *) _pixels.data());
    }

    void CpuPixelBuffer::CopyRgbaToRgb(infrastructure::CpuPixelBuffer &rgba_buffer) {
        std::vector<uint8_t>& source = rgba_buffer._pixels;
        std::vector<uint8_t>& dest = this->_pixels;

        for (GLsizei y = 0; y < _height; ++y) {
            for (GLsizei x = 0; x < _width; ++x) {
                // Calculate the source and destination indices
                size_t srcIndex = (y * _width + x) * 4;
                size_t destIndex = (y * _width + x) * 3;

                dest[destIndex]     = source[srcIndex];     // R
                dest[destIndex + 1] = source[srcIndex + 1]; // G
                dest[destIndex + 2] = source[srcIndex + 2]; // B
            }
        }
    }
}