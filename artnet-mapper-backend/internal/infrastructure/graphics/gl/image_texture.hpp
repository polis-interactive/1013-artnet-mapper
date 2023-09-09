//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
#define INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    enum class ImageTextureTypes {
        R8 = 0,
        RGB8 = 1
    };

    class ImageTexture {
    public:
        // no copy assignment, no empty assignment
        ImageTexture() = delete;
        ImageTexture (const ImageTexture&) = delete;
        ImageTexture& operator= (const ImageTexture&) = delete;

        // these should be protected, but you know, inheritance
        ImageTexture (
            const domain::Dimensions &dimensions, const std::string &texture_file_name, const ImageTextureTypes &type,
            const std::string &uniform_name, const int &texture_number
        );
        ~ImageTexture();
        void Setup();
        void SetLocation(const GLuint &shader_program);
        void Bind(const GLuint &shader_program) const;
        void Teardown();
    private:
        const int _width;
        const int _height;
        const std::string _uniform_name;
        const int _texture_number;
        const ImageTextureTypes _type;
        std::map<GLuint, GLint> _locations;
        std::filesystem::path _path;
        unsigned char *_data = nullptr;
        GLuint _texture = 0;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_TEXTURE_HPP
