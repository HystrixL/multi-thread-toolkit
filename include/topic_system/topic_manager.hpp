//
// Created by XianY on 2023/2/4.
//

#ifndef MULTI_THREAD_TOOLKIT_TOPIC_MANAGER_HPP
#define MULTI_THREAD_TOOLKIT_TOPIC_MANAGER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include "topic.hpp"

namespace mtt::ts {

    class TopicManager {
    public:
        template<typename T>
        using TopicPtr = std::shared_ptr<Topic<T>>;

        template<typename T>
        static TopicPtr<T> Create(std::string_view name) {
            std::unique_lock locker{topic_mutex_};

            return BaseCreate<T>(name);
        }

        template<typename T>
        static TopicPtr<T> Find(std::string_view name) {
            std::unique_lock locker{topic_mutex_};

            return BaseFind<T>(name);
        }

        template<typename T>
        static TopicPtr<T> FindOrCreate(std::string_view name) {
            std::unique_lock locker{topic_mutex_};

            auto iter = BaseFind<T>(name);

            if (iter) return iter;
            else return BaseCreate<T>(name);
        }

        template<typename T>
        static void DeleteTopic(std::string_view name) {
            std::unique_lock locker{topic_mutex_};

            if (!IsTopicExist(name)) return;

            Topic<T>::topics_[name].reset();
            Topic<T>::topics_.erase(name);
            auto pos = std::ranges::find(topic_names_, name);
            topic_names_.erase(pos);
        }

        constexpr explicit TopicManager() noexcept = delete;

        constexpr explicit TopicManager(const TopicManager &) noexcept = delete;

        const constexpr TopicManager &operator=(const auto &) const noexcept = delete;

        constexpr ~TopicManager() noexcept = delete;

    private:

        static bool IsTopicExist(std::string_view name) {
            return std::ranges::find(topic_names_, name) != topic_names_.end();
        }

        template<typename T>
        static TopicPtr<T> BaseCreate(std::string_view name) {
            if (IsTopicExist(name)) return nullptr;

            std::shared_ptr topic = std::make_shared<Topic<T>>();
            Topic<T>::topics_.emplace(name, topic);
            topic_names_.emplace_back(name);

            return topic;
        }

        template<typename T>
        static TopicPtr<T> BaseFind(std::string_view name) {
            if (!IsTopicExist(name)) return nullptr;

            auto iter = Topic<T>::topics_.find(std::string{name});
            if (iter == Topic<T>::topics_.end()) return nullptr;
            else return iter->second;
        }


        inline static std::vector<std::string> topic_names_{};

        inline static std::mutex topic_mutex_{};
    };
}

#endif //MULTI_THREAD_TOOLKIT_TOPIC_MANAGER_HPP
