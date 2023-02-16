//
// Created by XianY on 2023/2/3.
//

#ifndef MULTI_THREAD_TOOLKIT_TOPIC_HPP
#define MULTI_THREAD_TOOLKIT_TOPIC_HPP

#include <string>
#include <list>
#include <map>
#include <mutex>
#include <memory>

namespace mtt::ts{

    template<typename T>
    class Publisher;
    template<typename T>
    class Subscriber;

    template<typename T>
    class Topic{
    public:
        Topic() = default;
        ~Topic() = default;

    private:
        friend class Publisher<T>;
        friend class Subscriber<T>;
        friend class TopicManager;

        std::list<Publisher<T>*> publishers_{};
        std::list<Subscriber<T>*> subscribers_{};

        std::mutex publishers_mutex_{};
        std::mutex subscribers_mutex_{};

        inline static std::map<std::string,std::shared_ptr<Topic<T>>> topics_{};
    };
}

#endif //MULTI_THREAD_TOOLKIT_TOPIC_HPP
