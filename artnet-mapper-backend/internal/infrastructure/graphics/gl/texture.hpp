//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
#define INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    enum class TextureTypes {
        R8 = 0,
        RGB8 = 1
    };

    class Texture {
    public:
        // no copy assignment, no empty assignment
        Texture() = delete;
        Texture (const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;
    protected:
        friend class infrastructure::Graphics;
        Texture (const domain::Dimensions &dimensions, const std::string &texture_file_name, const TextureTypes type);
        ~Texture();
        void Setup();
        void Bind(const int &texture_number) const;
        void Teardown();
    private:
        const int _width;
        const int _height;
        const TextureTypes _type;
        std::filesystem::path _path;
        unsigned char *_data = nullptr;
        GLuint _texture = 0;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
