//
// Created by brucegoose on 7/5/23.
//

#include "headless_renderer.hpp"
#include "../graphics.hpp"

namespace infrastructure::graphics {
    HeadlessRenderer::HeadlessRenderer(
            const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw
    ):
        Renderer(dimensions, pixel_multiplier, is_rgbw),
        _offscreen_buffer(dimensions, 1, false, 3)
    {}

    bool HeadlessRenderer::SetupContext() {
        _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        ThrowOnEGLError("eglGetDisplay");

        int major, minor;

        if (eglInitialize(_display, &major, &minor) == EGL_FALSE)
        {
            std::cerr << "Failed to get EGL version! Error: " << eglGetErrorStr() << std::endl;
            eglTerminate(_display);
            return false;
        }

        std::cout << "Initialized EGL version: " << major << minor << std::endl;

        EGLConfig config;
        EGLint num_config;

        eglChooseConfig(_display, nullptr, &config, 1, &num_config);
        ThrowOnEGLError("eglChooseConfig");

        eglBindAPI(EGL_OPENGL_API);
        ThrowOnEGLError("eglBindAPI");

        const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

        _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttribs);
        ThrowOnEGLError("eglCreateContext");

        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, _context);
        ThrowOnEGLError("eglMakeCurrent");

        int version = gladLoadGLES2Loader((GLADloadproc) eglGetProcAddress);
        if (version == 0) {
            std::cout << "GlfwRenderer::Setup - Failed to initialize Gles2 context" << std::endl;
            return false;
        }

        std::cout << "HeadlessRenderer::Setup - Vendor graphic card: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "HeadlessRenderer::Setup - Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "HeadlessRenderer::Setup - Version GL: "<< glGetString(GL_VERSION) << std::endl;
        std::cout << "HeadlessRenderer::Setup - Version GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glViewport(0, 0, _dimensions.width, _dimensions.height);

        return true;
    }

    void HeadlessRenderer::Setup(GraphicsPtr &graphics) {
        _full_vao.Setup();
        _offscreen_buffer.Setup();
    }

    void HeadlessRenderer::Render(infrastructure::GraphicsPtr &graphics, CpuPixelBuffer *buffer) {
        _offscreen_buffer.BindFbo();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics->_display_shader.Use();
        Uniform::AttachUniforms(graphics->_display_uniforms, graphics->_display_shader._program);
        graphics->_pixel_type_texture.Bind(graphics->_display_shader._program);
        graphics->_artnet_texture.Bind(graphics->_display_shader._program);
        _full_vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ThrowOnEGLError("renderUnable");

        buffer->RenderBuffer();

        ThrowOnGlError("Just started pixel read");
    }
    void HeadlessRenderer::Teardown() noexcept {
        _full_vao.Teardown();
        _offscreen_buffer.Teardown();

        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        ThrowOnEGLError("eglMakeCurrent");

        eglTerminate(_display);
        ThrowOnEGLError("eglTerminate");
    }
}