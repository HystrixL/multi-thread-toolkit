//
// Created by XianY on 2023/2/3.
//

#ifndef MULTI_THREAD_TOOLKIT_PUBLISHER_HPP
#define MULTI_THREAD_TOOLKIT_PUBLISHER_HPP

#include <string>
#include <algorithm>
#include <ranges>
#include "topic_manager.hpp"

namespace mtt::ts {
    template<typename T>
    class Publisher {
    public:

        explicit Publisher(std::string_view topic_name) {
            Register(topic_name);
        }

        Publisher(Publisher<T> &) = delete;

        Publisher(Publisher<T> &&) = delete;

        void Push(const T &message) {
            std::unique_lock locker{binding_topic_->subscribers_mutex_};
            std::ranges::for_each(binding_topic_->subscribers_, [&message](Subscriber<T> *subscriber) {
                subscriber->Write(message);
                subscriber->NotifyOne();
            });
        }

        ~Publisher() {
            Unregister();
        }

    private:

        void Register(std::string_view topic_name) {
            binding_topic_ = TopicManager::FindOrCreate<T>(topic_name);

            std::unique_lock locker{binding_topic_->publishers_mutex_};

            binding_topic_->publishers_.emplace_front(this);
        }

        void Unregister() {
            std::unique_lock locker{binding_topic_->publishers_mutex_};

            binding_topic_->publishers_.remove(this);
            binding_topic_.reset();
        }

        TopicManager::TopicPtr<T> binding_topic_{};

    };
}

#endif //MULTI_THREAD_TOOLKIT_PUBLISHER_HPP
