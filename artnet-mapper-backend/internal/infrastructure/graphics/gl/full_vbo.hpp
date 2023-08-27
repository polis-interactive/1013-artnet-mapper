//
// Created by broosegoose on 8/11/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP
#define INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    class FullVbo {
    public:
        // no empty assignment
        FullVbo (const FullVbo&) = delete;
        FullVbo& operator= (const FullVbo&) = delete;

        // these should be protected, but you know, inheritance
        FullVbo () = default;
        ~FullVbo();
        void Setup();
        void Bind() const;
        void Teardown();
    private:
        GLuint _vbo = 0;
        GLuint _ebo = 0;
        bool _is_initialized = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_FULL_VAO_HPP
