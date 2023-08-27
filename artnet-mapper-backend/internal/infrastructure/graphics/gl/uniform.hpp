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
        static void AttachUniforms(UniformVector &uniforms, const GLuint &shaderProgram);
        static void SetupUniforms(UniformVector &uniforms, const GLuint &shaderProgram);
        explicit Uniform (std::string name);
        // no copy assignment, no empty assignment
        Uniform() = delete;
        Uniform (const Uniform&) = delete;
        Uniform& operator= (const Uniform&) = delete;
        // these should be protected, but you know, inheritance
        virtual void Attach(const GLuint &shaderProgram) const = 0;
        void Teardown();
        void Setup(const GLuint &shader_program);
    protected:
        std::map<GLuint, GLint> _locations;
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

        // these should be protected, but you know, inheritance
        void Attach(const GLuint &shaderProgram) const final;
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

        // these should be protected, but you know, inheritance
        void Attach(const GLuint &shaderProgram) const final;
        void SetValue(const int &value);
    private:
        int _value = 0.0;
    };

    class Float2Uniform;
    typedef std::shared_ptr<Float2Uniform> Float2UniformPtr;

    class Float2Uniform : public Uniform {
    public:
        static Float2UniformPtr Create(
            const std::string& name, const float &default_value_1, const float &default_value_2
        );
        Float2Uniform(
            const std::string& name, const float &default_value_1, const float &default_value_2
        );
        // no copy assignment, no empty assignment
        Float2Uniform() = delete;
        Float2Uniform (const Float2Uniform&) = delete;
        Float2Uniform& operator= (const Float2Uniform&) = delete;

        // these should be protected, but you know, inheritance
        void Attach(const GLuint &shaderProgram) const final;
        void SetValue(const float &value_1, const float &value_2);
    private:
        float _value_1;
        float _value_2;
    };

    class BoolUniform;
    typedef std::shared_ptr<BoolUniform> BoolUniformPtr;

    class BoolUniform : public Uniform {
    public:
        static BoolUniformPtr Create(const std::string& name, const bool &default_value);
        BoolUniform(const std::string& name, const bool &default_value);
        // no copy assignment, no empty assignment
        BoolUniform() = delete;
        BoolUniform (const BoolUniform&) = delete;
        BoolUniform& operator= (const BoolUniform&) = delete;

        // these should be protected, but you know, inheritance
        void Attach(const GLuint &shaderProgram) const final;
        void SetValue(const bool &value);
    private:
        bool _value = false;
    };

}

#endif //INFRASTRUCTURE_GRAPHICS_UNIFORM_HPP
