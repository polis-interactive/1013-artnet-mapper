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
        std::cout << "-1" << std::endl;
        float vertices[] = {
                // positions          // texture coords
                -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Top-left
                1.0f,  -1.0f, 0.0f,   1.0f, 0.0f,  // Top-right
                1.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Bottom-right
                -1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // Bottom-left
        };
        std::cout << "0" << std::endl;
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3, // second triangle
        };
        std::cout << "1" << std::endl;
        glGenVertexArrays(1, &_vao);
        std::cout << "2" << std::endl;
        glGenBuffers(1, &_vbo);
        std::cout << "3" << std::endl;
        glGenBuffers(1, &_ebo);
        std::cout << "4" << std::endl;

        glBindVertexArray(_vao);
        std::cout << "5" << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        std::cout << "6" << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        std::cout << "7" << std::endl;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        std::cout << "8" << std::endl;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        std::cout << "9" << std::endl;

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        std::cout << "10" << std::endl;
        glEnableVertexAttribArray(0);
        std::cout << "11" << std::endl;

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        std::cout << "12" << std::endl;
        glEnableVertexAttribArray(1);
        std::cout << "13" << std::endl;

        ThrowOnGlError("FullVao::Setup - unable to allocate vao, vbo, ebo");
        std::cout << "14" << std::endl;

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