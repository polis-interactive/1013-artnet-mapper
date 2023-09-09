//
// Created by brucegoose on 8/8/23.
//

#include <iostream>
#include "shader.hpp"

#include "utility/files.hpp"

namespace infrastructure::graphics {

    Shader::Shader(const std::string &shader_file_name, const bool owns_vertex_shader):
        _shader_file_name(shader_file_name),
        _owns_vertex_shader(owns_vertex_shader)
    {
        const std::filesystem::path asset_dir = SHADER_DIR;
        const auto fragment_file = asset_dir / (shader_file_name + ".frag");
        if (
            utility::FileExists(fragment_file) != utility::Ternary::TRUE
        ) {
            throw std::runtime_error(
                "infrastructure::graphics::Shader - Fragment shader not found: " + shader_file_name
            );
        }
        if (!_owns_vertex_shader) {
            return;
        }
        const auto vertex_file = asset_dir / (shader_file_name + ".vert");
        if (
                utility::FileExists(vertex_file) != utility::Ternary::TRUE
            ) {
            throw std::runtime_error(
                "infrastructure::graphics::Shader - Vertex shader not found: " + shader_file_name
            );
        }
    }

    void Shader::ensureShaderCompilation(const GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
            throw std::runtime_error("Shader::ensureShaderCompilation: failed to compile shader: " + _shader_file_name);
        }
    }

    void Shader::Setup() {
        if (_is_initialized) {
            return;
        }
        assert(_owns_vertex_shader);
        const std::filesystem::path asset_dir = SHADER_DIR;
        const auto vertex_file = asset_dir / (_shader_file_name + ".vert");
        const auto vertex_shader = utility::LoadFileContent(vertex_file);
        const char* vertex_shader_src = vertex_shader.c_str();
        _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertex_shader, 1, &vertex_shader_src, nullptr);
        glCompileShader(_vertex_shader);
        ensureShaderCompilation(_vertex_shader);

        setupFragmentShader();
        setupProgram();
        _is_initialized = true;
    }

    void Shader::Setup(const infrastructure::graphics::Shader &shader) {
        if (_is_initialized) {
            return;
        }
        assert(!_owns_vertex_shader);
        _vertex_shader = shader._vertex_shader;
        setupFragmentShader();
        setupProgram();
        _is_initialized = true;
    }

    void Shader::setupFragmentShader() {
        const std::filesystem::path asset_dir = SHADER_DIR;
        const auto fragment_file = asset_dir / (_shader_file_name + ".frag");
        const auto fragment_shader = utility::LoadFileContent(fragment_file);
        const char* fragment_shader_src = fragment_shader.c_str();
        _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(_fragment_shader, 1, &fragment_shader_src, nullptr);
        glCompileShader(_fragment_shader);
        ensureShaderCompilation(_fragment_shader);
    }

    void Shader::setupProgram() {
        _program = glCreateProgram();
        glAttachShader(_program, _vertex_shader);
        glAttachShader(_program, _fragment_shader);
        glLinkProgram(_program);

        GLint success;
        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(_program, 512, nullptr, infoLog);
            std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
            throw std::runtime_error("Shader::ensureShaderLinking: failed to compile shader: " + _shader_file_name);
        }
    }

    void Shader::Use() const {
        glUseProgram(_program);
    }

    void Shader::Teardown() {
        if (!_is_initialized) {
            return;
        }

        glDetachShader(_program, _vertex_shader);
        glDetachShader(_program, _fragment_shader);
        if (_owns_vertex_shader) {
            glDeleteShader(_vertex_shader);
        }
        glDeleteShader(_fragment_shader);
        glDeleteProgram(_program);

        _is_initialized = false;
    }

    Shader::~Shader() {
        Teardown();
    }

}