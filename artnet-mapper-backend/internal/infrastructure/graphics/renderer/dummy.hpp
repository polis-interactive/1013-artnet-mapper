//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP
#define INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP

#include "renderer.hpp"

#include "../gl/shader.hpp"

namespace infrastructure::graphics {

    class DummyRenderer: public Renderer {
    public:
        explicit DummyRenderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier);
    protected:
        bool SetupContext() final;
        void Setup(GraphicsPtr &graphics) final;
        void Render(GraphicsPtr &graphics, PixelBuffer *pbo) final;
        void Teardown() noexcept final;
    private:
        graphics::Shader _default_shader;
        GLFWwindow *_window = nullptr;
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_RENDERER_DUMMY_HPP
