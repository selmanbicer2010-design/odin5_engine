#pragma once
#include "core/utilities.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>

namespace odin5{
namespace event{

    struct connection_t : odin5::util::type_safe_int32_wrapper<connection_t> { using odin5::util::type_safe_int32_wrapper<connection_t>::type_safe_int32_wrapper; };
    template <typename... args_t>
    using callback_t = std::function<void(args_t...)>;

    template <typename event_spec>
    struct i_event {
        using constexpr_event_spec = odin5::util::constexpr_class<event_spec>;
        static constexpr bool v =
        constexpr_event_spec::template has_qualified_id<decltype(&event_spec::do_and_connect)>::v &&
        constexpr_event_spec::template has_qualified_id<decltype(&event_spec::connect)>::v &&
        constexpr_event_spec::template has_qualified_id<decltype(&event_spec::once)>::v &&
        constexpr_event_spec::template has_qualified_id<decltype(&event_spec::disconnect)>::v &&
        constexpr_event_spec::template has_qualified_id<decltype(&event_spec::fire)>::v;
    };

    template <typename event_spec>
    concept i_event_v = i_event<event_spec>::v;

    template <typename... args_t>
    class basic_event {
    public:
        using callback_type = callback_t<args_t...>;
        using connection_type = connection_t;
    private:
        using callbacks_container = odin5::util::unordered_vector<callback_type>;
        callbacks_container callbacks;

    public:

        connection_type do_and_connect(const args_t&... args, callback_type&& cb) {
            cb(args...);
            return callbacks.push_back(std::move(cb));
        }

        connection_type connect(callback_type&& cb) {
            return callbacks.push_back(std::move(cb));
        }

        connection_type once(callback_type&& cb) {
            connection_type handle = callbacks.next_handle();
            callbacks.push_back([this, cb, handle](const args_t&... args) mutable {
                callbacks.erase(handle);
                cb(args...);
            });
            return handle;
        }

        callback_type disconnect(connection_type connection) {
            auto cb = callbacks[connection];
            callbacks.erase(connection.value);
            return cb;
        }

        void fire(const args_t&... args) {
            callbacks_container copy = callbacks;
            for (auto& cb : copy) {
                cb(args...);
            }
        }

    };

    template <typename... args_t>
    class sequenced_event {
    public:
        using callback_type = callback_t<args_t...>;
        using connection_type = connection_t;

        static constexpr int64_t first = INT64_MIN;
        static constexpr int64_t last = INT64_MAX;
    private:
        struct callback_info {
            callback_type callback;
            int64_t order = 0;
        };
        using callbacks_container = odin5::util::unordered_vector<callback_info>;
        callbacks_container callbacks;

    public:

        connection_type do_and_connect(const args_t&... args, callback_type&& cb, int64_t order) {
            cb(args...);
            return callbacks.push_back({std::move(cb), order});
        }

        connection_type connect(callback_type&& cb, int64_t order) {
            return callbacks.push_back({std::move(cb), order});
        }

        connection_type once(callback_type&& cb, int64_t order) {
            connection_type handle = callbacks.next_handle();
            callbacks.push_back({[this, cb, handle](const args_t&... args) mutable {
                callbacks.erase(handle);
                cb(args...);
            }, order});
            return handle;
        }

        callback_type disconnect(connection_type connection) {
            auto cb = callbacks[connection];
            callbacks.erase(connection.value);
            return cb.callback;
        }

        void fire(const args_t&... args) {
            callbacks_container ordered = callbacks;
            std::ranges::sort(ordered, [](const auto& a, const auto& b){
                return a.order < b.order;
            });
            for (auto& cb : ordered) {
                cb.callback(args...);
            }
        }

    };

}
}
