#pragma once
#include "core/utilities.hpp"
#include <functional>

namespace odin5{
namespace event{

    template <typename... args_t>
    class basic_event {
    private:
        struct connection_t : odin5::util::type_safe_int32_wrapper<connection_t> { using odin5::util::type_safe_int32_wrapper<connection_t>::type_safe_int32_wrapper; };
        using callback_type = std::function<void(args_t...)>;
        using connection_type = connection_t;
        using callbacks_container = odin5::util::unordered_vector<callback_type>;
        callbacks_container callbacks;

    public:
        connection_type connect(callback_type&& cb) {
            return callbacks.push_back(std::move(cb));
        }

        connection_type once(callback_type&& cb) {
            connection_type handle = callbacks.next_handle();
            callbacks.push_back([this, cb, handle](const args_t&... args) mutable {
                cb(args...);
                callbacks.erase(handle);
            });
            return handle;
        }

        void disconnect(connection_type connection) {
            callbacks.erase(connection.value);
        }

        void fire(const args_t&... args) {
            callbacks_container copy = callbacks;
            for (auto& cb : copy) {
                cb(args...);
            }
        }

    };
}
}
