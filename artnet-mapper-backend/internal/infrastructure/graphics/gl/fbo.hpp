//
// Created by broosegoose on 8/11/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_FBO_HPP
#define INFRASTRUCTURE_GRAPHICS_FBO_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    class Fbo {
    public:
        // no empty assignment
        Fbo (const Fbo&) = delete;
        Fbo& operator= (const Fbo&) = delete;

        // these should be protected, but you know, inheritance
        Fbo (const domain::Dimensions &dimensions, const int &texture_number);
        ~Fbo();
        void Setup();
        void BindFbo() const;
        void BindTextureRoot() const;
        void BindTexture(const GLuint &shader_program) const;
        void Teardown();
    private:
        const int _width;
        const int _height;
        const int _texture_number;
        GLuint _fbo = 0;
        GLuint _texture = 0;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_FBO_HPP