//
// Created by broosegoose on 8/11/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP
#define INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    class FullVao {
    public:
        // no empty assignment
        FullVao (const FullVao&) = delete;
        FullVao& operator= (const FullVao&) = delete;

        // these should be protected, but you know, inheritance
        FullVao () = default;
        ~FullVao();
        void Setup();
        void Bind() const;
        void Teardown();
    private:
        GLuint _vao = 0;
        GLuint _vbo = 0;
        GLuint _ebo = 0;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP
