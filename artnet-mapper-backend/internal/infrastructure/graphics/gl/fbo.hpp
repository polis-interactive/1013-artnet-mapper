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
        Fbo (
            const domain::Dimensions &dimensions, const unsigned int &pixel_multiplier,
            const bool is_rgbw, const int &texture_number);
        ~Fbo();
        void Setup();
        void SetupLocation(const GLuint &shader_program);
        void BindFbo() const;
        void BindTexture(const GLuint &shader_program) const;
        void Teardown();
    private:
        const int _width;
        const int _height;
        const unsigned int _multiplier;
        const int _texture_number;
        const bool _is_rgbw;
        GLuint _fbo = 0;
        GLuint _texture = 0;
        std::map<GLuint, GLint> _locations;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_FBO_HPP