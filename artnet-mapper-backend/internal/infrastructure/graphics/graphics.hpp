//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_HPP
#define INFRASTRUCTURE_GRAPHICS_HPP

#include <memory>
#include <filesystem>
#include <thread>

#include "./common.hpp"
#include "./gl/shader.hpp"
#include "./gl/texture.hpp"
#include "./gl/pixel_buffer.hpp"
#include "./renderer/renderer.hpp"

namespace infrastructure::graphics {

    struct GraphicsManager {
        virtual void PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) = 0;
    };
    typedef std::shared_ptr<GraphicsManager> GraphicsManagerPtr;

    /* graphics needs to be shutdown last */

    class Graphics: public std::enable_shared_from_this<Graphics> {
    public:
        // also need to either add a callback for posting images / a message handler if we need
        // debug stuff going on
        [[nodiscard]] static std::shared_ptr<Graphics> Create(const Config &config);
        explicit Graphics(const Config &config);
        // no copy assignment, no empty assignment
        Graphics() = delete;
        Graphics (const Graphics&) = delete;
        Graphics& operator= (const Graphics&) = delete;
        void Start();
        void Stop();
        ~Graphics();
    private:
        void run(const std::stop_token &st);
        std::unique_ptr<std::jthread> _graphics_thread = nullptr;
        bool _stop_running = true;

        void setup();
        void teardown();

        void runGraphics(const std::stop_token &st);
        std::atomic_bool _is_ready = false;

        domain::InstallationSummary _summary;
        Shader _main_shader;
        Texture _pixel_type_texture;
        Texture _artnet_texture;
        PixelBuffers _pbos;
        std::unique_ptr<Renderer> _renderer;

    };
};

#endif //INFRASTRUCTURE_GRAPHICS_HPP
