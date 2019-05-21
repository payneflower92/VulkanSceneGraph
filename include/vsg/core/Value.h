#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/Data.h>
#include <vsg/core/type_name.h>

#include <vsg/maths/mat3.h>
#include <vsg/maths/mat4.h>
#include <vsg/maths/vec2.h>
#include <vsg/maths/vec3.h>
#include <vsg/maths/vec4.h>

#include <vsg/io/Input.h>
#include <vsg/io/Output.h>

#define VSG_value(N, T) \
    using N = Value<T>; \
    template<>          \
    constexpr const char* type_name<N>() noexcept { return "vsg::" #N; }

namespace vsg
{
    template<typename T>
    class Value : public Data
    {
    public:
        using value_type = T;

        Value() {}
        Value(const Value& rhs) :
            _value(rhs._value) {}
        explicit Value(const value_type& in_value) :
            _value(in_value) {}

        std::size_t sizeofObject() const noexcept override { return sizeof(Value); }

        // implementation provided by Visitor.h
        void accept(Visitor& visitor) override;
        void accept(ConstVisitor& visitor) const override;

        const char* className() const noexcept override { return type_name<Value>(); }

        void read(Input& input) override
        {
            Data::read(input);
            input.read("Value", _value);
        }

        void write(Output& output) const override
        {
            Data::write(output);
            output.write("Value", _value);
        }

        std::size_t valueSize() const override { return sizeof(value_type); }
        std::size_t valueCount() const override { return 1; }

        std::size_t dataSize() const override { return sizeof(value_type); }

        void* dataPointer() override { return &_value; }
        const void* dataPointer() const override { return &_value; }

        void* dataPointer(size_t) override { return &_value; }
        const void* dataPointer(size_t) const override { return &_value; }

        void* dataRelease() override { return nullptr; }

        std::uint32_t width() const override { return 1; }
        std::uint32_t height() const override { return 1; }
        std::uint32_t depth() const override { return 1; }

        Value& operator=(const Value& rhs)
        {
            _value = rhs._value;
            return *this;
        }
        Value& operator=(const value_type& rhs)
        {
            _value = rhs;
            return *this;
        }

        operator value_type&() { return _value; }
        operator const value_type&() const { return _value; }

        value_type& value() { return _value; }
        const value_type& value() const { return _value; }

    protected:
        virtual ~Value() {}

    private:
        value_type _value;
    };

    template<typename T>
    void Object::setValue(const std::string& key, const T& value)
    {
        using ValueT = Value<T>;
        setObject(key, new ValueT(value));
    }

    template<typename T>
    bool Object::getValue(const std::string& key, T& value) const
    {
        using ValueT = Value<T>;
        const Object* object = getObject(key);
        if (object && (typeid(*object) == typeid(ValueT)))
        {
            const ValueT* vo = static_cast<const ValueT*>(getObject(key));
            value = *vo;
            return true;
        }
        else
        {
            return false;
        }
    }

    VSG_value(stringValue, std::string);
    VSG_value(boolValue, bool);
    VSG_value(intValue, int);
    VSG_value(uintValue, unsigned int);
    VSG_value(floatValue, float);
    VSG_value(doubleValue, double);

    VSG_value(vec2Value, vec2);
    VSG_value(vec3Value, vec3);
    VSG_value(vec4Value, vec4);

    VSG_value(dvec2Value, dvec2);
    VSG_value(dvec3Value, dvec3);
    VSG_value(dvec4Value, dvec4);

    VSG_value(ubvec2Value, ubvec2);
    VSG_value(ubvec3Value, ubvec3);
    VSG_value(ubvec4Value, ubvec4);

    VSG_value(usvec2Value, usvec2);
    VSG_value(usvec3Value, usvec3);
    VSG_value(usvec4Value, usvec4);

    VSG_value(uivec2Value, uivec2);
    VSG_value(uivec3Value, uivec3);
    VSG_value(uivec4Value, uivec4);

    VSG_value(mat3Value, mat3);
    VSG_value(dmat3Value, dmat3);

    VSG_value(mat4Value, mat4);
    VSG_value(dmat4Value, dmat4);

} // namespace vsg
