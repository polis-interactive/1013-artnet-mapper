//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP

#include "renderer.hpp"

#include "../gl/shader.hpp"
#include "../gl/full_vao.hpp"
#include "../gl/fbo.hpp"

namespace infrastructure::graphics {

    class GlfwRenderer: public Renderer {
    public:
        explicit GlfwRenderer(
            const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
        );
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, CpuPixelBuffer *buffer) final;
        void Teardown() noexcept final;
    private:
        void setWindowHints();
        GLFWwindow *_window = nullptr;
        graphics::Shader _full_screen_shader;
        graphics::Shader _mapping_shader;
        graphics::FullVao _full_vao;
        graphics::Fbo _onscreen_buffer;
        graphics::Fbo _offscreen_buffer;
        CpuPixelBuffer _rgba_pixel_buffer;
    };

}


#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HPP
