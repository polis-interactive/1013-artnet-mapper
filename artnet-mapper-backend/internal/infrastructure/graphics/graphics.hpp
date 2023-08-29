//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_HPP
#define INFRASTRUCTURE_GRAPHICS_HPP

#include <memory>
#include <thread>

#include "./common.hpp"
#include "./gl/shader.hpp"
#include "./gl/image_texture.hpp"
#include "./gl/pixel_buffer.hpp"
#include "./gl/uniform.hpp"
#include "./gl/full_vbo.hpp"
#include "./renderer/renderer.hpp"

#include "utility/clock.hpp"

namespace infrastructure {

    struct GraphicsManager {
        virtual void PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) = 0;
        virtual void RequestReboot() = 0;
    };
    typedef std::shared_ptr<GraphicsManager> GraphicsManagerPtr;


    class Graphics: public std::enable_shared_from_this<Graphics> {
    public:
        [[nodiscard]] static GraphicsPtr Create(
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

        const bool _render_art_net;

        GraphicsManagerPtr _manager;

        bool setup();
        void teardown();

        void runGraphics(const std::stop_token &st);
        void runGraphicsArtNet(const std::stop_token &st);
        std::atomic_bool _is_ready = false;

        bool renderNextFrame(GraphicsPtr &self);
        void requeueBuffer(CpuPixelBuffer *reclaim_buffer);

        domain::installation::Config _config;
        const utility::Duration _frame_time;

        std::unique_ptr<graphics::Renderer> _renderer;

        std::atomic<float> _atm_brightness = 1.0;

        std::deque<CpuPixelBuffer *> _cpu_buffers;
        std::mutex _cpu_buffers_mutex;

        const unsigned int _rolling_frames_length;
        const double _rolling_frames_divisor;
        std::deque<bool> _frames_success;
        const double _frames_success_threshold;
        std::deque<utility::Duration> _frames_duration;
        const utility::Duration _frame_duration_threshold;

    public:
        // these should be protected, but that's not how inheritance works...

        void PostBrightness(const float new_brightness);

        graphics::FloatUniformPtr _time = graphics::FloatUniform::Create("time", 1.0);
        graphics::FloatUniformPtr _brightness = graphics::FloatUniform::Create("brightness", 1.0);

        graphics::IntUniformPtr _pixel_multiplier;
        graphics::Float2UniformPtr _resolution;

        graphics::ImageTexture _pixel_type_texture;

        graphics::BoolUniformPtr _do_artnet_mapping;
        graphics::ImageTexture _artnet_texture;

        graphics::Shader _display_shader;

        std::vector<graphics::UniformPtr> _display_uniforms;

    };
};

#endif //INFRASTRUCTURE_GRAPHICS_HPP
