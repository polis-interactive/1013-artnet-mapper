//
// Created by brucegoose on 7/5/23.
//

#include "dummy.hpp"

#include "../graphics.hpp"

namespace infrastructure::graphics {
    DummyRenderer::DummyRenderer(
            const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
    ):
            Renderer(dimensions, pixel_multiplier, is_rgbw),
            _default_shader("my_shader", true),
            _full_screen_shader("full_screen_texture", true),
            _mapping_shader("mapping_shader", true),
            _onscreen_buffer(dimensions, pixel_multiplier, false, 3),
            _offscreen_buffer(dimensions, 1, true, 4)
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
        glfwWindowHint(GLFW_STENCIL_BITS, 0);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
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

        int version = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
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
        _full_screen_shader.Setup();
        _mapping_shader.Setup();
        _full_vao.Setup();

        graphics->_pixel_type_texture.SetLocation(_default_shader._program);
        graphics->_artnet_texture.SetLocation(_default_shader._program);

        _onscreen_buffer.Setup();
        _onscreen_buffer.SetupLocation(_full_screen_shader._program);
        _onscreen_buffer.SetupLocation(_mapping_shader._program);

        _offscreen_buffer.Setup();
        graphics->_resolution->Setup(_mapping_shader._program);
    }

    void DummyRenderer::Render(infrastructure::GraphicsPtr &graphics, CpuPixelBuffer *buffer) {
        glfwPollEvents();
        glViewport(0, 0,  _dimensions.width * _multiplier, _dimensions.height * _multiplier);

        _onscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _default_shader.Use();
        graphics->_pixel_type_texture.Bind(_default_shader._program);
        graphics->_artnet_texture.Bind(_default_shader._program);
        _full_vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // draw
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _full_screen_shader.Use();
        _onscreen_buffer.BindTexture(_full_screen_shader._program);
        _full_vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(_window);

        // pbo out
        glViewport(0, 0,  _dimensions.width, _dimensions.height);
        ThrowOnGlError("Just started pixel 1");
        _offscreen_buffer.BindFbo();
        ThrowOnGlError("Just started pixel 2");
        glClearColor(0.0, 1.0, 1.0, 1.0);
        ThrowOnGlError("Just started pixel 3");
        glClear(GL_COLOR_BUFFER_BIT);
        ThrowOnGlError("Just started pixel 4");
        glFlush();
        ThrowOnGlError("Just started pixel 5");

        // pbo->RenderBuffer();

        // ThrowOnGlError("Just started pixel read");

    }
    void DummyRenderer::Teardown() noexcept {}
}