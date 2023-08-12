//
// Created by broosegoose on 8/9/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_UNIFORM_HPP
#define INFRASTRUCTURE_GRAPHICS_UNIFORM_HPP

#include "../common.hpp"

namespace infrastructure::graphics {

    class Uniform;
    typedef std::shared_ptr<Uniform> UniformPtr;
    typedef std::vector<UniformPtr> UniformVector;

    class Uniform {
    public:
        static void AttachUniforms(UniformVector &uniforms);
        explicit Uniform (std::string name);
        // no copy assignment, no empty assignment
        Uniform() = delete;
        Uniform (const Uniform&) = delete;
        Uniform& operator= (const Uniform&) = delete;
    protected:
        friend class infrastructure::Graphics;
        virtual void Set() const = 0;
        void Teardown();
    protected:
        friend class Shader;
        void Setup(const GLuint &shaderProgram);
    protected:
        GLint _location = -1;
        const std::string _name;
    };

    class FloatUniform;
    typedef std::shared_ptr<FloatUniform> FloatUniformPtr;

    class FloatUniform : public Uniform {
    public:
        static FloatUniformPtr Create(const std::string& name, const float &default_value);
        FloatUniform(const std::string& name, const float &default_value);
        // no copy assignment, no empty assignment
        FloatUniform() = delete;
        FloatUniform (const FloatUniform&) = delete;
        FloatUniform& operator= (const FloatUniform&) = delete;
    protected:
        friend class infrastructure::Graphics;
        void Set() const final;
        void SetValue(const float &value);
    private:
        float _value = 0.0;
    };

    class IntUniform;
    typedef std::shared_ptr<IntUniform> IntUniformPtr;

    class IntUniform : public Uniform {
    public:
        static IntUniformPtr Create(const std::string& name, const int &default_value);
        IntUniform(const std::string& name, const int &default_value);
        // no copy assignment, no empty assignment
        IntUniform() = delete;
        IntUniform (const IntUniform&) = delete;
        IntUniform& operator= (const IntUniform&) = delete;
    protected:
        friend class infrastructure::Graphics;
        void Set() const final;
        void SetValue(const int &value);
    private:
        int _value = 0.0;
    };



}

#endif //INFRASTRUCTURE_GRAPHICS_UNIFORM_HPP
