//
// Created by XianY on 2023/2/5.
//

#ifndef MULTI_THREAD_TOOLKIT_BASE_SERIALIZER_HPP
#define MULTI_THREAD_TOOLKIT_BASE_SERIALIZER_HPP

#include <string>
#include <string_view>

namespace mtt::ss {

    template<typename T, typename R>
    class BaseSerializer {
    public:
        virtual R Serialize(const T &data) = 0;

        virtual T Deserialize(const R &data) = 0;

        BaseSerializer() = default;

        virtual ~BaseSerializer() = default;
    };

}

#endif //MULTI_THREAD_TOOLKIT_BASE_SERIALIZER_HPP
