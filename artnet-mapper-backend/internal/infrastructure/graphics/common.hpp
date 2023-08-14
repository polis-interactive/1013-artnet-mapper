//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_COMMON_HPP
#define INFRASTRUCTURE_GRAPHICS_COMMON_HPP

#include <string>
#include <iostream>

#ifdef _GLFW_RENDERER_

#define GLAD_GL_IMPLEMENTATION
#include "glad/glad_egl.h"
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>

#else

/*
 * EGL headers.
 */
#include <EGL/egl.h>

/*
 * OpenGL headers.
 */
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#endif


#include "domain/installation.hpp"
#include "domain/display.hpp"

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

    inline void ThrowOnGlError(const std::string &display_on_error) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL ERROR: " << err << std::endl;
            throw std::runtime_error(display_on_error);
        }
    }

}



#endif //INFRASTRUCTURE_GRAPHICS_COMMON_HPP
