//
// Created by XianY on 2023/2/3.
//

#ifndef MTT_SV_HPP
#define MTT_SV_HPP

#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <algorithm>

namespace mtt::sv {

    template<typename T>
    class VariableManager {
    public:

        template<class ...Ts>
        static std::shared_ptr<T> Create(std::string_view name, Ts &&... args) {
            std::unique_lock locker{mutex_};

            return BaseCreate(name, std::forward<Ts>(args)...);

        }

        static std::shared_ptr<T> Find(std::string_view name) {
            std::unique_lock locker{mutex_};

            return BaseFind(name);
        }

        template<class ...Ts>
        static std::shared_ptr<T> FindOrCreate(std::string_view name, Ts &&... args) {
            std::unique_lock locker{mutex_};
            auto iter = BaseFind(name);

            if (iter) return iter;
            else return BaseCreate(name, std::forward<Ts>(args)...);
        }

        static std::vector<std::string> GetNames() {
            std::unique_lock locker{mutex_};

            std::vector<std::string> names{};

            std::ranges::for_each(map_, [&](const std::pair<std::string, std::weak_ptr<T>> &elem) {
                names.emplace_back(elem.first);
            });

            return names;
        }

        VariableManager() = delete;

        ~VariableManager() = default;

    private:

        inline static std::map<std::string, std::shared_ptr<T>> map_{};

        inline static std::mutex mutex_{};

        template<class ...Ts>
        static std::shared_ptr<T> BaseCreate(const std::string_view &name, Ts &&... args) {
            if (map_.contains(std::string{name})) return nullptr;

            std::shared_ptr variable_ptr = std::make_shared<T>(std::forward<Ts>(args)...);
            map_.emplace(std::string(name), variable_ptr);

            return variable_ptr;
        }

        static std::shared_ptr<T> BaseFind(const std::string_view &name) {
            auto iter = map_.find(std::string{name});
            if (iter == map_.end()) return nullptr;
            else return iter->second;
        }

    };
}

#endif //MTT_SV_HPP
