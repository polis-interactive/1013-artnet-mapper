//
// Created by broosegoose on 8/28/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HEADLESS_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HEADLESS_HPP

#include "renderer.hpp"

#include "../gl/full_vbo.hpp"


namespace infrastructure::graphics {

    class GlfwHeadlessRenderer: public Renderer {
    public:
        explicit GlfwHeadlessRenderer(
                const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
        );
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, CpuPixelBuffer *buffer) final;
        void Teardown() noexcept final;
    private:
        GLFWwindow *_window = nullptr;
        graphics::FullVbo _full_vbo;
        CpuPixelBuffer _rgba_pixel_buffer;
    };

}


#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_GLFW_HEADLESS_HPP
