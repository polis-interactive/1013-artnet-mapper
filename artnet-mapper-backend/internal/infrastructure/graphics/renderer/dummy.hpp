//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP

#include "renderer.hpp"

#include "../gl/shader.hpp"
#include "../gl/full_vao.hpp"
#include "../gl/fbo.hpp"

namespace infrastructure::graphics {

    class DummyRenderer: public Renderer {
    public:
        explicit DummyRenderer(
                const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
        );
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, CpuPixelBuffer *buffer) final;
        void Teardown() noexcept final;
    private:
        graphics::Shader _default_shader;
        graphics::Shader _full_screen_shader;
        graphics::Shader _mapping_shader;
        GLFWwindow *_window = nullptr;
        graphics::FullVao _full_vao;
        graphics::Fbo _onscreen_buffer;
        graphics::Fbo _offscreen_buffer;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP
