//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
#define INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP

#include "../common.hpp"

namespace infrastructure::graphics {
    class Texture {
    public:
        // no copy assignment, no empty assignment
        Texture() = delete;
        Texture (const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;
    protected:
        friend class Graphics;
        Texture (const domain::InstallationSummary &summary, const std::string texture_file_name);
        void Setup();
        void Teardown();
    private:
        const unsigned int _width;
        const unsigned int _height;
        std::filesystem::path texture_path;
        bool _is_initialized;
        const Config &config;
    };
}

#endif //INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
