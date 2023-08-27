//
// Created by broosegoose on 8/11/23.
//

#include "full_vbo.hpp"

namespace infrastructure::graphics {

    FullVbo::~FullVbo() {
        Teardown();
    }

    void FullVbo::Setup() {
        if (_is_initialized) {
            return;
        }
        float vertices[] = {
                // positions          // texture coords
                -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Top-left
                1.0f,  -1.0f, 0.0f,   1.0f, 0.0f,  // Top-right
                1.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Bottom-right
                -1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // Bottom-left
        };
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3, // second triangle
        };
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        ThrowOnGlError("FullVao::Setup - unable to allocate vao, vbo, ebo");

        _is_initialized = true;
    }

    void FullVbo::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    }

    void FullVbo::Teardown() {
        if (!_is_initialized) {
            return;
        }
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);

        _is_initialized = false;
    }


}