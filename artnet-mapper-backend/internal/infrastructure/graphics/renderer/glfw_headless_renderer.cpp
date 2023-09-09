//
// Created by broosegoose on 8/28/23.
//

#include "glfw_headless_renderer.hpp"
#include "../graphics.hpp"

namespace infrastructure::graphics {
    GlfwHeadlessRenderer::GlfwHeadlessRenderer(
        const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
    ):
        Renderer(dimensions, pixel_multiplier, is_rgbw),
        _rgba_pixel_buffer(dimensions, true)
    {}

    bool GlfwHeadlessRenderer::SetupContext() {
        if (!glfwInit()) {
            std::cout << "GlfwRenderer::Setup - unable to start glfw context" << std::endl;
            return false;
        }
        glfwSetTime(0);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        _window = glfwCreateWindow(
            _dimensions.width, _dimensions.height, "ArtnetMapper", NULL, NULL
        );
        glfwMakeContextCurrent(_window);
        int version = gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
        if (version == 0) {
            std::cout << "GlfwRenderer::Setup - Failed to initialize Gles2 context" << std::endl;
            return false;
        }

        std::cout << "GlfwRenderer::Setup - Vendor graphic card: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "GlfwRenderer::Setup - Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "GlfwRenderer::Setup - Version GL: "<< glGetString(GL_VERSION) << std::endl;
        std::cout << "GlfwRenderer::Setup - Version GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glViewport(0, 0, _dimensions.width, _dimensions.height);
        glfwSwapInterval(0);
        glfwSwapBuffers(_window);
        glfwShowWindow(_window);
        return true;
    }

    void GlfwHeadlessRenderer::Setup(infrastructure::GraphicsPtr &graphics) {
        _full_vbo.Setup();
        graphics->_do_artnet_mapping->SetValue(true);
    }

    void GlfwHeadlessRenderer::Render(infrastructure::GraphicsPtr &graphics, CpuPixelBuffer *buffer) {
        // setup
        glfwPollEvents();

        // Display pass
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics->_display_shader.Use();
        Uniform::AttachUniforms(graphics->_display_uniforms, graphics->_display_shader._program);
        graphics->_pixel_type_texture.Bind(graphics->_display_shader._program);
        graphics->_artnet_texture.Bind(graphics->_display_shader._program);
        _full_vbo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(_window);

        ThrowOnGlError("final render");

        if (_is_rgbw) {
            buffer->RenderBuffer();
            ThrowOnGlError("Just started pixel read");
            return;
        }

        _rgba_pixel_buffer.RenderBuffer();
        ThrowOnGlError("Just started pixel read");
        buffer->CopyRgbaToRgb(_rgba_pixel_buffer);
    }

    void GlfwHeadlessRenderer::Teardown() noexcept {
        _full_vbo.Teardown();
    }
}