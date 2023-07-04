//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_SHADER_HPP
#define INFRASTRUCTURE_GRAPHICS_SHADER_HPP

namespace infrastructure::graphics {

    // requires a vertex / frag pair with the same name
    class Shader {
    public:
        // no copy assignment, no empty assignment
        Shader() = delete;
        Shader (const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;
    protected:
        friend class Graphics;
        explicit Shader(std::string shader_file_name);
        void Setup();
        void Teardown();
    private:
        std::filesystem::path shader_path;
        bool _is_initialized;

    };
}

#endif //INFRASTRUCTURE_GRAPHICS_SHADER_HPP
