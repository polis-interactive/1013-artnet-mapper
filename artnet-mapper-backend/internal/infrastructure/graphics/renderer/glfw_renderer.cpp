//
// Created by brucegoose on 7/5/23.
//

#include <iostream>

#include "glfw_renderer.hpp"
#include "../graphics.hpp"

namespace infrastructure::graphics {
    GlfwRenderer::GlfwRenderer(
            const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
    ):
        Renderer(dimensions, pixel_multiplier, is_rgbw),
        _mapping_shader("mapping_shader", true),
        _full_screen_shader("full_screen_texture", true),
        _onscreen_buffer(dimensions, pixel_multiplier, true, 3),
        _offscreen_buffer(dimensions, 1, true, 4),
        _rgba_pixel_buffer(dimensions, true)
    {}
    bool GlfwRenderer::SetupContext() {
        if (!glfwInit()) {
            std::cout << "GlfwRenderer::Setup - unable to start glfw context" << std::endl;
            return false;
        }
        glfwSetTime(0);
        setWindowHints();
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

    void GlfwRenderer::Setup(infrastructure::GraphicsPtr &graphics) {
        _onscreen_buffer.Setup();
        _offscreen_buffer.Setup();
        _full_vbo.Setup();

        _full_screen_shader.Setup();
        _onscreen_buffer.SetupLocation(_full_screen_shader._program);

        _mapping_shader.Setup();
        graphics->_resolution->Setup(_mapping_shader._program);
        _onscreen_buffer.SetupLocation(_mapping_shader._program);
        graphics->_artnet_texture.SetLocation(_mapping_shader._program);

        graphics->_do_artnet_mapping->SetValue(false);

    }

    void GlfwRenderer::Render(infrastructure::GraphicsPtr &graphics, CpuPixelBuffer *buffer) {
        // setup
        glfwPollEvents();
        glViewport(0, 0,  _dimensions.width * _multiplier, _dimensions.height * _multiplier);

        // Display pass
        _onscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics->_display_shader.Use();
        Uniform::AttachUniforms(graphics->_display_uniforms, graphics->_display_shader._program);
        graphics->_pixel_type_texture.Bind(graphics->_display_shader._program);
        graphics->_artnet_texture.Bind(graphics->_display_shader._program);
        _full_vbo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _full_screen_shader.Use();
        _onscreen_buffer.BindTexture(_full_screen_shader._program);
        _full_vbo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(_window);

        if (buffer == nullptr) {
            return;
        }

        // do the mapping to pbo
        glViewport(0, 0,  _dimensions.width, _dimensions.height);
        _offscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _mapping_shader.Use();
        _onscreen_buffer.BindTexture(_mapping_shader._program);
        graphics->_artnet_texture.Bind(_mapping_shader._program);
        graphics->_resolution->Attach(_mapping_shader._program);
        _full_vbo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    void GlfwRenderer::Teardown() noexcept {
        _full_vbo.Teardown();

        _full_screen_shader.Teardown();
        _mapping_shader.Teardown();

        _onscreen_buffer.Teardown();
        _offscreen_buffer.Teardown();
    }

    void GlfwRenderer::setWindowHints() {
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
    }
}