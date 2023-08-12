//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_SHADER_HPP
#define INFRASTRUCTURE_GRAPHICS_SHADER_HPP

#include "../common.hpp"


namespace infrastructure::graphics {

    class Shader {
    public:
        // no copy assignment, no empty assignment
        Shader() = delete;
        Shader (const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;
    protected:
        friend class infrastructure::Graphics;
        friend class GlfwRenderer;
        Shader(const std::string &shader_file_name, const bool owns_vertex_shader);
        ~Shader();
        void Setup();
        void Setup(const Shader &shader);
        void Use() const;
        void Teardown();
    private:
        void setupFragmentShader();
        void setupProgram();
        void ensureShaderCompilation(const GLuint shader);
        const std::string _shader_file_name;
        const bool _owns_vertex_shader;
        GLuint _vertex_shader = 0;
        GLuint _fragment_shader = 0;
        GLuint _program = 0;
        bool _is_initialized = false;

    };
}

#endif //INFRASTRUCTURE_GRAPHICS_SHADER_HPP
