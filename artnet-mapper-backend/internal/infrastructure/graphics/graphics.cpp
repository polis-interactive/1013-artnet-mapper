//
// Created by brucegoose on 7/5/23.
//

#include <iostream>

#include "graphics.hpp"

namespace infrastructure::graphics {
    std::shared_ptr<Graphics> Graphics::Create(const infrastructure::graphics::Config &config) {
        auto graphics = std::make_shared<Graphics>(config);
        return std::move(graphics);
    }

    Graphics::Graphics(const infrastructure::graphics::Config &config):
        _summary(config.graphics_summary),
        _main_shader(config.graphics_shader),
        _pixel_type_texture(config.graphics_summary, config.graphics_pixel_texture),
        _artnet_texture(config.graphics_summary, config.graphics_artnet_texture),
        _pbos(config),
        _renderer(Renderer::Create(config.graphics_renderer_type, _summary))
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

    void Graphics::setup() {
        _renderer->Setup();
        _main_shader.Setup();
        _pixel_type_texture.Setup();
        _artnet_texture.Setup();
        _pbos.Setup();
    }

    void Graphics::teardown() {
        _pbos.Teardown();
        _artnet_texture.Teardown();
        _pixel_type_texture.Teardown();
        _main_shader.Teardown();
        _renderer->Teardown();
    }
}