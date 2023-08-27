//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_COMMON_HPP
#define INFRASTRUCTURE_GRAPHICS_COMMON_HPP

#include <string>
#include <iostream>



#define GLAD_GL_IMPLEMENTATION
#include "glad/glad_egl.h"
#include "glad/glad.h"

#ifdef _GLFW_RENDERER_

#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_EGL 1
#define GLFW_NATIVE_INCLUDE_NONE 1
#include <GLFW/glfw3native.h>

#endif

#include "domain/installation.hpp"
#include "domain/display.hpp"
#include "utility/buffers.hpp"

namespace infrastructure {

    struct GraphicsConfig {
        domain::Display display;
        domain::installation::Config installation_config;
        static GraphicsConfig from_json(const nlohmann::json& j) {
            GraphicsConfig conf{};
            conf.display = domain::Display::from_json(j.at("display"));
            conf.installation_config = domain::installation::Config::from_json(j.at("installation_config"));
            return conf;
        }
    };

    // forward declaration, for the boys
    class Graphics;
    typedef std::shared_ptr<Graphics> GraphicsPtr;

    class CpuPixelBuffer: public utility::SizedBuffer {
    public:
        [[nodiscard]] uint8_t *GetMemory() final;
        [[nodiscard]] std::size_t GetSize() final;
        void RenderBuffer() const;
        // no copy assignment, empty assignment
        CpuPixelBuffer() = delete;
        CpuPixelBuffer (const CpuPixelBuffer&) = delete;
        CpuPixelBuffer& operator= (const CpuPixelBuffer&) = delete;
        // should be protected but meh
        CpuPixelBuffer(const domain::Dimensions &dimensions, const bool is_rgbw);
        void CopyRgbaToRgb(CpuPixelBuffer &rgba_buffer);
    private:
        const GLsizei _width;
        const GLsizei _height;
        const GLenum _format;
        const std::size_t _size;
        std::vector<uint8_t> _pixels;
    };

    inline void ThrowOnGlError(const std::string &display_on_error) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL ERROR: " << err << std::endl;
            throw std::runtime_error(display_on_error);
        }
    }

    inline void ThrowOnEGLError(const std::string& msg) {
        EGLint error = eglGetError();

        if (error != EGL_SUCCESS) {
            std::stringstream s;
            s << "EGL error 0x" << std::hex << error << " at " << msg;
            throw std::runtime_error(s.str());
        }
    }

    inline const char *eglGetErrorStr()
    {
        switch (eglGetError())
        {
            case EGL_SUCCESS:
                return "The last function succeeded without error.";
            case EGL_NOT_INITIALIZED:
                return "EGL is not initialized, or could not be initialized, for the "
                       "specified EGL display connection.";
            case EGL_BAD_ACCESS:
                return "EGL cannot access a requested resource (for example a context "
                       "is bound in another thread).";
            case EGL_BAD_ALLOC:
                return "EGL failed to allocate resources for the requested operation.";
            case EGL_BAD_ATTRIBUTE:
                return "An unrecognized attribute or attribute value was passed in the "
                       "attribute list.";
            case EGL_BAD_CONTEXT:
                return "An EGLContext argument does not name a valid EGL rendering "
                       "context.";
            case EGL_BAD_CONFIG:
                return "An EGLConfig argument does not name a valid EGL frame buffer "
                       "configuration.";
            case EGL_BAD_CURRENT_SURFACE:
                return "The current surface of the calling thread is a window, pixel "
                       "buffer or pixmap that is no longer valid.";
            case EGL_BAD_DISPLAY:
                return "An EGLDisplay argument does not name a valid EGL display "
                       "connection.";
            case EGL_BAD_SURFACE:
                return "An EGLSurface argument does not name a valid surface (window, "
                       "pixel buffer or pixmap) configured for GL rendering.";
            case EGL_BAD_MATCH:
                return "Arguments are inconsistent (for example, a valid context "
                       "requires buffers not supplied by a valid surface).";
            case EGL_BAD_PARAMETER:
                return "One or more argument values are invalid.";
            case EGL_BAD_NATIVE_PIXMAP:
                return "A NativePixmapType argument does not refer to a valid native "
                       "pixmap.";
            case EGL_BAD_NATIVE_WINDOW:
                return "A NativeWindowType argument does not refer to a valid native "
                       "window.";
            case EGL_CONTEXT_LOST:
                return "A power management event has occurred. The application must "
                       "destroy all contexts and reinitialise OpenGL ES state and "
                       "objects to continue rendering.";
            default:
                break;
        }
        return "Unknown error!";
    }

}



#endif //INFRASTRUCTURE_GRAPHICS_COMMON_HPP
