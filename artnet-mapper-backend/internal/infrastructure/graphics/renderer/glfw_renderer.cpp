//
// Created by brucegoose on 7/5/23.
//

#include <iostream>

#include "glfw_renderer.hpp"
#include "../graphics.hpp"

namespace infrastructure::graphics {
    GlfwRenderer::GlfwRenderer(const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier):
        Renderer(dimensions, pixel_multiplier),
        _mapping_shader("mapping_shader", false),
        _onscreen_buffer(dimensions, 3),
        _offscreen_buffer(dimensions, 4)
    {}
    bool GlfwRenderer::SetupContext() {
        if (!glfwInit()) {
            std::cout << "GlfwRenderer::Setup - unable to start glfw context" << std::endl;
            return false;
        }
        glfwSetTime(0);
        setWindowHints();
        _window = glfwCreateWindow(_dimensions.width, _dimensions.height, "ArtnetMapper", NULL, NULL);
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

        glViewport(0, 0, _dimensions.width, _dimensions.height);
        glfwSwapInterval(0);
        glfwSwapBuffers(_window);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwShowWindow(_window);

        return true;
    }

    void GlfwRenderer::Setup(infrastructure::GraphicsPtr &graphics) {
        _mapping_shader.Setup(graphics->_display_shader);
        graphics->_resolution->Setup(_mapping_shader._program);
        _full_vao.Setup();
        _onscreen_buffer.Setup();
        _offscreen_buffer.Setup();
    }

    void GlfwRenderer::Render(infrastructure::GraphicsPtr &graphics, PixelBuffer *pbo) {
        // setup
        glfwPollEvents();
        /*

        // Display pass
        _onscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics->_display_shader.Use();
        Uniform::AttachUniforms(graphics->_display_uniforms, graphics->_display_shader._program);
        graphics->_pixel_type_texture.Bind(graphics->_display_shader._program);
        _full_vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glfwSwapBuffers(_window);


        // do the mapping to pbo
        _offscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _mapping_shader.Use();
        _onscreen_buffer.BindTexture(_mapping_shader._program);
        graphics->_artnet_texture.Bind(_mapping_shader._program);
        graphics->_resolution->Attach(_mapping_shader._program);
        _full_vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         */
        _offscreen_buffer.BindFbo();
        _offscreen_buffer.BindTextureRoot();

        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();

        pbo->RenderBuffer();

        ThrowOnGlError("Just started pixel read");
    }

    void GlfwRenderer::Teardown() noexcept {
        _mapping_shader.Teardown();
        _full_vao.Teardown();
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
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
}