//
// Created by broosegoose on 8/11/23.
//

#include "full_vao.hpp"

namespace infrastructure::graphics {

    FullVao::~FullVao() {
        Teardown();
    }

    void FullVao::Setup() {
        if (_is_initialized) {
            return;
        }
        float vertices[] = {
                // positions    // texture coords
                1.0, 1.0,       1.0f, 1.0f, // top right
                1.0, -1.0,      1.0f, 0.0f, // bottom right,
                -1.0, -1.0,     0.0f, 0.0f, // bottom left,
                -1.0, 1.0,      0.0f, 1.0f, // top left,
        };
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3, // second triangle
        };
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);

        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        ThrowOnGlError("FullVao::Setup - unable to allocate vao, vbo, ebo");

        _is_initialized = true;
    }

    void FullVao::Bind() const {
        glBindVertexArray(_vao);
    }

    void FullVao::Teardown() {
        if (!_is_initialized) {
            return;
        }
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);

        _is_initialized = false;
    }


}