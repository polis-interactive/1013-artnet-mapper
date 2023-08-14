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
            "pixel_type_texture", 0
        ),
        _artnet_texture(
            _config.dimensions, config.display.artnet_texture, graphics::ImageTextureTypes::RGB8,
            "artnet_texture", 1
        ),
        _pbos(_config),
        _renderer(graphics::Renderer::Create(
            config.display.render_type, _config.dimensions, config.display.pixel_multiplier
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
        _frameTime(1 / _config.fps)

    {}

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

    void Graphics::run(const std::stop_token &st) {
        bool has_run = false;
        while (!st.stop_requested()) {
            if (has_run) {
                std::cout << "Graphics::run: main loop failed; waiting and trying again..." << std::endl;
            }
            has_run = true;
            setup();
            _is_ready = true;
            runGraphics(st);
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
        _artnet_texture.Setup();
        _pbos.Setup();
        graphics::Uniform::SetupUniforms(_display_uniforms, _display_shader._program);
        auto self(shared_from_this());
        _renderer->Setup(self);
        return true;
    }

    void Graphics::runGraphics(const std::stop_token &st) {
        auto self(shared_from_this());
        unsigned int consecutive_failed_frames = 0;
        while (!st.stop_requested()) {
            auto start = std::chrono::high_resolution_clock::now();
            reclaimSpentPbos();
            postReadyPbos();
            if (!renderNextFrame(self)) {
                consecutive_failed_frames++;
            } else {
                consecutive_failed_frames = 0;
            }
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
        if (_pbos._ready_queue.empty()) {
            return false;
        }
        auto next_pbo = _pbos._ready_queue.front();
        _pbos._ready_queue.pop_front();
        _renderer->Render(self, next_pbo);
        _pbos._render_queue.push_back(next_pbo);
        return true;
    }

    void Graphics::reclaimSpentPbos() {
        std::unique_lock<std::mutex> lock(_pbos._spent_mutex);
        while (!_pbos._spent_queue.empty()) {
            auto reclaim_pbo = _pbos._spent_queue.front();
            // I think I need to check for success here... may be from a previous loop
            reclaim_pbo->UnmapBuffer();
            _pbos._ready_queue.push_back(reclaim_pbo);
            _pbos._spent_queue.pop_front();
        }
    }

    void Graphics::postReadyPbos() {
        while (!_pbos._render_queue.empty()) {
            auto render_pbo = _pbos._render_queue.front();
            if (!render_pbo->MapBuffer()) {
                // not ready, just break as the next one won't be either
                break;
            }
            _pbos._render_queue.pop_front();
            auto post_pbo = std::shared_ptr<graphics::PixelBuffer>(
                    render_pbo, std::bind_front(&Graphics::requeuePbo, shared_from_this())
            );
            _manager->PostGraphicsUpdate(std::move(post_pbo));
        }
    }

    void Graphics::requeuePbo(graphics::PixelBuffer *reclaim_pbo) {
        std::unique_lock<std::mutex> lock(_pbos._spent_mutex);
        _pbos._spent_queue.push_back(reclaim_pbo);
    }

    void Graphics::teardown() {
        _pbos.Teardown();
        _artnet_texture.Teardown();
        _pixel_type_texture.Teardown();
        _display_shader.Teardown();
        _renderer->Teardown();
    }
}