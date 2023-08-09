//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_HPP
#define INFRASTRUCTURE_GRAPHICS_HPP

#include <memory>
#include <thread>

#include "./common.hpp"
#include "./gl/shader.hpp"
#include "./gl/texture.hpp"
#include "./gl/pixel_buffer.hpp"
#include "./renderer/renderer.hpp"

namespace infrastructure {

    struct GraphicsManager {
        virtual void PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) = 0;
    };
    typedef std::shared_ptr<GraphicsManager> GraphicsManagerPtr;

    class Graphics;
    typedef std::shared_ptr<Graphics> GraphicsPtr;

    class Graphics: public std::enable_shared_from_this<Graphics> {
    public:
        [[nodiscard]] static std::shared_ptr<Graphics> Create(
            const GraphicsConfig &config, GraphicsManagerPtr manager
        );
        explicit Graphics(const GraphicsConfig &config, GraphicsManagerPtr manager);
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

        domain::installation::Config _summary;
        domain::installation::Layout _layout;
        graphics::Shader _main_shader;
        graphics::Texture _pixel_type_texture;
        graphics::Texture _artnet_texture;
        graphics::PixelBuffers _pbos;
        std::unique_ptr<graphics::Renderer> _renderer;

    };
};

#endif //INFRASTRUCTURE_GRAPHICS_HPP
