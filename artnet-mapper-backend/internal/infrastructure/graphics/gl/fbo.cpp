//
// Created by broosegoose on 8/11/23.
//

#include "fbo.hpp"

namespace infrastructure::graphics {

    Fbo::Fbo(const domain::Dimensions &dimensions, const int &texture_number):
        _width((int) dimensions.width),
        _height((int) dimensions.height),
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        ThrowOnGlError("Fbo::Setup - unable to allocate fbo");

        _is_initialized = true;
    }

    void Fbo::BindFbo() const {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }

    void Fbo::BindTexture(const GLuint &shader_program) const {
        glActiveTexture(GL_TEXTURE0 + _texture_number);
        glBindTexture(GL_TEXTURE_2D, _texture);
        GLint texture_location = glGetUniformLocation(shader_program, "previous_render");
        glUniform1i(texture_location, _texture_number);
    }

    void Fbo::BindTextureRoot() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture);
    }

    void Fbo::Teardown() {
        if (!_is_initialized) {
            return;
        }
        glDeleteFramebuffers(1, &_fbo);
        glDeleteTextures(1, &_texture);
        _is_initialized = false;
    }
}