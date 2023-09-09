//
// Created by broosegoose on 8/9/23.
//

#include "uniform.hpp"

#include <utility>

namespace infrastructure::graphics {

    void Uniform::AttachUniforms(UniformVector &uniforms, const GLuint &shaderProgram) {
        for (auto &uniform : uniforms) {
            uniform->Attach(shaderProgram);
        }
    }

    void Uniform::SetupUniforms(infrastructure::graphics::UniformVector &uniforms, const GLuint &shaderProgram) {
        for (auto &uniform : uniforms) {
            uniform->Setup(shaderProgram);
        }
    }

    Uniform::Uniform(std::string name): _name(std::move(name)) {}

    void Uniform::Setup(const GLuint &shader_program) {
        auto location = glGetUniformLocation(shader_program, _name.c_str());
        if (location == -1) {
            std::cout << "Uniform::Setup not found in shader: " + _name << std::endl;
        }
        _locations.insert({ shader_program, location });
    }

    void Uniform::Teardown() {
        _locations.clear();
    }

    FloatUniformPtr FloatUniform::Create(const std::string &name, const float &default_value) {
        auto uniform = std::make_shared<FloatUniform>(name, default_value);
        return std::move(uniform);
    }

    FloatUniform::FloatUniform(const std::string &name, const float &default_value):
        Uniform(name),
        _value(default_value)
    {}

    void FloatUniform::SetValue(const float &value) {
        _value = value;
    }

    void FloatUniform::Attach(const GLuint &shader_program) const {
        const auto &uniform_location = _locations.at(shader_program);
        if (uniform_location != -1) {
            glUniform1f(uniform_location, _value);
        }
    }

    IntUniformPtr IntUniform::Create(const std::string &name, const int &default_value) {
        auto uniform = std::make_shared<IntUniform>(name, default_value);
        return std::move(uniform);
    }

    IntUniform::IntUniform(const std::string &name, const int &default_value):
        Uniform(name),
        _value(default_value)
    {}

    void IntUniform::SetValue(const int &value) {
        _value = value;
    }

    void IntUniform::Attach(const GLuint &shader_program) const {
        const auto &uniform_location = _locations.at(shader_program);
        if (uniform_location != -1) {
            glUniform1i(uniform_location, _value);
        }
    }

    Float2UniformPtr Float2Uniform::Create(
        const std::string &name, const float &default_value_1, const float &default_value_2
    ) {
        auto uniform = std::make_shared<Float2Uniform>(name, default_value_1, default_value_2);
        return std::move(uniform);
    }

    Float2Uniform::Float2Uniform(
        const std::string &name, const float &default_value_1, const float &default_value_2
    ):
        Uniform(name),
        _value_1(default_value_1),
        _value_2(default_value_2)
    {}

    void Float2Uniform::Attach(const GLuint &shader_program) const {
        const auto &uniform_location = _locations.at(shader_program);
        if (uniform_location != -1) {
            glUniform2f(uniform_location, _value_1, _value_2);
        }
    }

    BoolUniformPtr BoolUniform::Create(const std::string &name, const bool &default_value) {
        auto uniform = std::make_shared<BoolUniform>(name, default_value);
        return std::move(uniform);
    }

    BoolUniform::BoolUniform(const std::string &name, const bool &default_value):
        Uniform(name),
        _value(default_value)
    {}

    void BoolUniform::SetValue(const bool &value) {
        _value = value;
    }

    void BoolUniform::Attach(const GLuint &shader_program) const {
        const auto &uniform_location = _locations.at(shader_program);
        if (uniform_location != -1) {
            glUniform1i(uniform_location, _value);
        }
    }

}