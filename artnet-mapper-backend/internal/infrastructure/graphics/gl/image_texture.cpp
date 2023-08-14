//
// Created by broosegoose on 8/8/23.
//

#include "image_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.hpp"

#include "utility/files.hpp"

#include <iostream>

namespace infrastructure::graphics {

    ImageTexture::ImageTexture(
        const domain::Dimensions &dimensions, const std::string &texture_file_name, const ImageTextureTypes &type,
        const std::string &&uniform_name, const int &texture_number
    ):
        _width((int) dimensions.width),
        _height((int) dimensions.height),
        _type(type),
        _uniform_name(std::move(uniform_name)),
        _texture_number(texture_number)
    {
        _path = TEXTURE_DIR;
        _path /= texture_file_name;
        if (utility::FileExists(_path) != utility::Ternary::TRUE) {
            throw std::runtime_error(
                "infrastructure::graphics::Texture - Texture not found: " + _path.string()
            );
        }
    }

    void ImageTexture::Setup() {
        if (_is_initialized) {
            return;
        }

        auto width = _width;
        auto height = _height;
        auto channels = _type == ImageTextureTypes::R8 ? 1 : 3;
        auto expected_channels = _type == ImageTextureTypes::R8 ? 1 : 3;
        _data = stbi_load(_path.c_str(), &width, &height, &channels, expected_channels);
        if (_data == nullptr) {
            throw std::runtime_error("Texture::Setup - Failed to load texture: " + _path.string());
        } else if (width != _width || height != _height) {
            throw std::runtime_error("Texture::Setup - Image did not match parameters: " + _path.string());
        }

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        switch (_type) {
            case ImageTextureTypes::R8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, _data);
                break;
            case ImageTextureTypes::RGB8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
                break;
        }
        ThrowOnGlError("Texture::Setup - Failed to initialize texture: " + _path.string());

        _is_initialized = true;
    }

    void ImageTexture::Bind(const GLuint &shader_program) const {
        glActiveTexture(GL_TEXTURE0 + _texture_number);
        glBindTexture(GL_TEXTURE_2D, _texture);
        GLint texture_location = glGetUniformLocation(shader_program, _uniform_name.c_str());
        glUniform1i(texture_location, _texture_number);
    }

    void ImageTexture::Teardown() {
        if (!_is_initialized) {
            return;
        }
        glDeleteTextures(1, &_texture);
        stbi_image_free(_data);

        _is_initialized = false;
    }

    ImageTexture::~ImageTexture() {
        Teardown();
    }

}