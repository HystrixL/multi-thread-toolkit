//
// Created by XianY on 2023/2/5.
//

#ifndef MULTI_THREAD_TOOLKIT_JSON_SERIALIZER_H
#define MULTI_THREAD_TOOLKIT_JSON_SERIALIZER_H

#include "base_serializer.hpp"
#include "../../3rdparty/nlohmann_json/single_include/nlohmann/json.hpp"

namespace mtt::ss {

    template<typename T>
    class JsonSerializer : public BaseSerializer<T, std::string> {
    public:
        std::string Serialize(const T &data) override {
            return nlohmann::json(data).dump();
        }

        T Deserialize(const std::string &data) override {
            return nlohmann::json::parse(data).get<T>();
        }

        JsonSerializer() = default;

        ~JsonSerializer() override = default;
    };

}

#endif //MULTI_THREAD_TOOLKIT_JSON_SERIALIZER_H
