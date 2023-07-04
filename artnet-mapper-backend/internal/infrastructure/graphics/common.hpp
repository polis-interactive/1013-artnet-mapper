//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_CONFIG_HPP
#define INFRASTRUCTURE_GRAPHICS_CONFIG_HPP

#include <string>

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


#include "domain/installation_summary.hpp"

namespace infrastructure::graphics {

    enum class RendererType {
        GLFW,
        HEADLESS
    };

    struct Config {
        const RendererType graphics_renderer_type;
        const domain::InstallationSummary graphics_summary;
        const std::string graphics_shader;
        const std::string graphics_pixel_texture;
        const std::string graphics_artnet_texture;
        const unsigned int graphics_pbo_count;
        const unsigned int graphics_fps;
    };

}



#endif //INFRASTRUCTURE_GRAPHICS_CONFIG_HPP
