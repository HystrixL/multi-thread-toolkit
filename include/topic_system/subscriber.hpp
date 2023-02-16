//
// Created by XianY on 2023/2/3.
//

#ifndef MULTI_THREAD_TOOLKIT_SUBSCRIBER_HPP
#define MULTI_THREAD_TOOLKIT_SUBSCRIBER_HPP

#include <string>
#include <algorithm>
#include <ranges>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "topic_manager.hpp"

namespace mtt::ts {
    template<template<class...> class Target, class T>
    inline constexpr bool is_template_of_v = false;
    template<template<class...> class Target, class...Args>
    inline constexpr bool is_template_of_v<Target, Target<Args...>> = true;

    template<typename T>
    concept TimePoint = is_template_of_v<std::chrono::time_point, T>;

    template<typename T>
    class Subscriber {
    public:

        /**
         * @brief 创建一个订阅者
         * @details 初始化一个订阅者并绑定到指定名称的话题上
         * @param name 话题名称
         * @param length 消息队列最大长度
         */
        explicit Subscriber(std::string_view name, std::size_t length) : queue_length_(length) {
            Register(name);
        }

        Subscriber(Subscriber<T> &) = delete;

        Subscriber(Subscriber<T> &&) = delete;

        /**
         * @brief 获取一条消息
         * @details 获取一条消息。当消息队列为空时，将死锁该订阅器直到队列中存在消息
         * @return 获取到的消息
         */
        T Pop() {
            std::unique_lock locker{message_queue_mutex_};

            pop_locker_.wait(locker, [this]() { return !message_queue_.empty(); });

            return BasePop().value();
        }

        /**
         * @brief 尝试获取一条消息
         * @details 尝试获取一条消息。当消息队列为空时，将返回空值
         * @return 获取到的消息，可能为空
         */
        std::optional<T> TryPop() {
            std::unique_lock locker{message_queue_mutex_};

            return BasePop();
        }

        /**
         * @brief 在指定时间内尝试获取一条消息
         * @details 获取一条消息。当消息队列为空时，将等待指定时间直到队列中存在消息
         * @param ms 等待时间，单位为毫秒
         * @return 获取到的消息，可能为空
         */
        std::optional<T> PopFor(const size_t &ms) {
            std::unique_lock locker{message_queue_mutex_};

            pop_locker_.wait_for(locker, std::chrono::milliseconds(ms), [this]() { return !message_queue_.empty(); });

            return BasePop();
        }

        /**
         * @brief 在指定时间点前尝试获取一条消息
         * @details 获取一条消息。当消息队列为空时，将等待指定时间直到队列中存在消息
         * @param point 等待时间点
         * @return 获取到的消息，可能为空
         */
        std::optional<T> PopUntil(const TimePoint auto &point) {
            std::unique_lock locker{message_queue_mutex_};

            pop_locker_.wait_until(locker, point, [this]() { return !message_queue_.empty(); });

            return BasePop();
        }

        ~Subscriber() {
            Unregister();
        }

    private:
        friend class Publisher<T>;

        std::queue<T> message_queue_{};
        std::size_t queue_length_{};
        TopicManager::TopicPtr<T> binding_topic_{};
        std::condition_variable pop_locker_{};
        std::mutex message_queue_mutex_{};

        /**
         * @brief 绑定到指定名称的话题上
         * @param topic_name 话题名称
         */
        void Register(std::string_view topic_name) {
            binding_topic_ = TopicManager::FindOrCreate<T>(topic_name);

            std::unique_lock locker{binding_topic_->subscribers_mutex_};

            binding_topic_->subscribers_.emplace_front(this);
        }

        /**
         * @brief 从话题上解绑
         */
        void Unregister() {
            std::unique_lock locker{binding_topic_->subscribers_mutex_};

            binding_topic_->subscribers_.remove(this);
            binding_topic_.reset();
        }

        /**
         * @brief 向消息队列中写入一条消息
         * @details 向消息队列中写入一条消息。当消息队列已满时，将丢弃队列中最早的消息
         * @param message 消息
         */
        void Write(const T &message) {
            std::unique_lock locker{message_queue_mutex_};
            if (queue_length_ > 0 && message_queue_.size() >= queue_length_)
                message_queue_.pop();
            message_queue_.push(message);
        }

        /**
         * @brief 唤醒等待中的订阅者
         */
        void NotifyOne() {
            pop_locker_.notify_one();
        }

        /**
         * @brief 获取一条消息
         * @details 获取一条消息。当消息队列为空时，将返回空值
         * @return 获取到的消息，可能为空
         */
        std::optional<T> BasePop() {
            if (message_queue_.empty()) return std::nullopt;

            auto message = std::move(message_queue_.front());
            message_queue_.pop();
            return message;
        }
    };
}


#endif //MULTI_THREAD_TOOLKIT_SUBSCRIBER_HPP
