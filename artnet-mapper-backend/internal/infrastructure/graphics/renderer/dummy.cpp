//
// Created by brucegoose on 7/5/23.
//

#include "dummy.hpp"

#include "../graphics.hpp"

namespace infrastructure::graphics {
    DummyRenderer::DummyRenderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier):
            Renderer(dimensions, pixel_multiplier),
            _default_shader("my_shader", true)
    {}

    bool DummyRenderer::SetupContext() {
        if (!glfwInit()) {
            std::cout << "GlfwRenderer::Setup - unable to start glfw context" << std::endl;
            return false;
        }
        glfwSetTime(0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE,GLFW_TRUE);

        glfwWindowHint(GLFW_SAMPLES, 0);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 16);
        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        _window = glfwCreateWindow(
                _dimensions.width * _multiplier, _dimensions.height * _multiplier, "ArtnetMapper", NULL, NULL
        );
        if (!_window)
        {
            std::cout << "GlfwRenderer::Setup - Failed to create GLFW window background" << std::endl;
            return false;
        }

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

        glViewport(0, 0, _dimensions.width * _multiplier, _dimensions.height * _multiplier);
        glfwSwapInterval(0);
        glfwSwapBuffers(_window);
        // glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwShowWindow(_window);

        return true;
    }

    void DummyRenderer::Setup(GraphicsPtr &graphics) {
        _default_shader.Setup();
        glGenVertexArrays(1, &_vao);
    }

    void DummyRenderer::Render(infrastructure::GraphicsPtr &graphics, PixelBuffer *pbo) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _default_shader.Use();
        graphics->_pixel_type_texture.Bind(_default_shader._program);
        graphics->_artnet_texture.Bind(_default_shader._program);
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glfwSwapBuffers(_window);
    }
    void DummyRenderer::Teardown() noexcept {}
}