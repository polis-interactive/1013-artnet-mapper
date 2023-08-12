//
// Created by broosegoose on 8/9/23.
//

#include "uniform.hpp"

#include <utility>

namespace infrastructure::graphics {

    void Uniform::AttachUniforms(UniformVector &uniforms) {
        for (auto &uniform : uniforms) {
            uniform->Set();
        }
    }

    Uniform::Uniform(std::string name): _name(std::move(name)) {}

    void Uniform::Setup(const GLuint &shaderProgram) {
        _location = glGetUniformLocation(shaderProgram, _name.c_str());
        if (_location == -1) {
            throw std::runtime_error("Uniform::Setup not found in shader: " + _name);
        }
    }

    void Uniform::Teardown() {
        _location = -1;
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

    void FloatUniform::Set() const {
        glUniform1f(_location, _value);
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

    void IntUniform::Set() const {
        glUniform1i(_location, _value);
    }


}