//
// Created by broosegoose on 8/11/23.
//

#include "fbo.hpp"

namespace infrastructure::graphics {

    Fbo::Fbo(
        const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier, const bool is_rgbw,
        const int &texture_number
    ):
        _width((int) dimensions.width),
        _height((int) dimensions.height),
        _multiplier(pixel_multiplier),
        _is_rgbw(is_rgbw),
        _texture_number(texture_number)
    {}

    Fbo::~Fbo() {
        Teardown();
    }

    void Fbo::Setup() {
        if (_is_initialized) {
            return;
        }

        glGenFramebuffers(1, &_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        const auto gl_format = _is_rgbw ? GL_RGBA : GL_RGB;
        glTexImage2D(
            GL_TEXTURE_2D, 0, gl_format, _width * _multiplier, _height * _multiplier,
            0, gl_format, GL_UNSIGNED_BYTE, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Unable to setup FBO");
        }

        ThrowOnGlError("Fbo::Setup - incomplete fbo setup");

        _is_initialized = true;
    }

    void Fbo::SetupLocation(const GLuint &shader_program) {
        GLint texture_location = glGetUniformLocation(shader_program, "previous_render_texture");
        if (texture_location == -1) {
            std::cout << "Fbo::SetupLocation previous_render_texture not found in shader" << std::endl;
        }
        _locations.insert({ shader_program, texture_location });
    }


    void Fbo::BindFbo() const {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }

    void Fbo::BindTexture(const GLuint &shader_program) const {
        glActiveTexture(GL_TEXTURE0 + _texture_number);
        glBindTexture(GL_TEXTURE_2D, _texture);
        const auto texture_location = _locations.at(shader_program);
        if (texture_location != -1) {
            glUniform1i(texture_location, _texture_number);
        }
    }

    void Fbo::Teardown() {
        if (!_is_initialized) {
            return;
        }
        glDeleteFramebuffers(1, &_fbo);
        glDeleteTextures(1, &_texture);
        _locations.clear();
        _is_initialized = false;
    }
}