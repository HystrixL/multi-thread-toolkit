//
// Created by XianY on 2023/2/5.
//

#ifndef MULTI_THREAD_TOOLKIT_BINARY_SERIALIZER_HPP
#define MULTI_THREAD_TOOLKIT_BINARY_SERIALIZER_HPP

#include "base_serializer.hpp"
#include "array"
#include "string"
#include <memory>

namespace mtt::ss {

    template<typename T>
    using BinaryStream = std::array<unsigned char, sizeof(T)>;

    template<typename T>
    class BinarySerializer : public BaseSerializer<T, BinaryStream<T>> {
    public:
        BinaryStream<T> Serialize(const T &data) override {
            BinaryStream<T> buffer{};
            memmove(&buffer, &data, sizeof(data));
            return buffer;
        }

        T Deserialize(const BinaryStream<T> &data) override {
            T result{};

            memmove(&result, &data, sizeof(data));
            return result;
        }

        /**
         * 基于C++原生二进制流的序列化器，效率更高。
         * 该序列器仅适用于C原生数据类型，不适用于自定义类型。
         * 该序列器仅支持值类型，不支持引用类型及指针。
         * 建议用于处理C风格的结构体，而不是C++风格的类。
         */
        BinarySerializer() = default;

        ~BinarySerializer() override = default;
    };

}

#endif //MULTI_THREAD_TOOLKIT_BINARY_SERIALIZER_HPP
