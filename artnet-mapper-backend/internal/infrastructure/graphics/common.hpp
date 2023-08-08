//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_COMMON_HPP
#define INFRASTRUCTURE_GRAPHICS_COMMON_HPP

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


#include "domain/installation.hpp"
#include "domain/display.hpp"

namespace infrastructure {

    struct GraphicsConfig {
        domain::Display display;
        domain::installation::Config installation_config;
        domain::installation::Layout installation_layout;
        static GraphicsConfig from_source(const nlohmann::json& j, const std::filesystem::path &assets_dir) {
            GraphicsConfig conf{};
            conf.display = domain::Display::from_source(j.at("display"), assets_dir);
            conf.installation_config = domain::installation::Config::from_json(j.at("installation_config"));
            conf.installation_layout = domain::installation::Layout::from_json(j.at("installation_layout"));
            return conf;
        }
    };

}



#endif //INFRASTRUCTURE_GRAPHICS_COMMON_HPP
