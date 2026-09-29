#pragma once
#include <cinttypes> // IWYU pragma: keep
#include <concepts>
#include <print>

namespace odin5{
namespace util{

    template <int32_t count = 0>
    void error_if(bool cond, const char* tag = nullptr) {
    #ifdef ODIN5_DEBUG
        if (cond) {
            std::println("{}", tag ? tag : "error");
        }
    #endif
    }

    template <typename ret_t, typename... args_t>
    struct func_ptr {
        using t = ret_t(args_t...);
    };

    template <class class_t, typename ret_t, typename... args_t>
    struct method_ptr {
        using t = ret_t(class_t::*)(args_t...);
    };

    template <class class_t>
    struct constexpr_class {
        template <typename... args_t>
        struct constructible_from {
            static constexpr bool v = std::constructible_from<class_t, args_t...>;
        };
        template <typename method_sig_t, typename ret_t, typename... args_t>
        struct method_signature_is {
            static constexpr bool v = std::same_as<method_sig_t, typename method_ptr<class_t, ret_t, args_t...>::t>;
        };
    };

    template <typename inheritor_t, int32_t default_value_v = 0>
    struct type_safe_int32_wrapper {
        constexpr type_safe_int32_wrapper(int32_t v) : value(v) {}
        constexpr type_safe_int32_wrapper(const type_safe_int32_wrapper&) = default;
        constexpr type_safe_int32_wrapper(type_safe_int32_wrapper&&) = default;
        static constexpr int32_t default_value = default_value_v;
        int32_t value = default_value;
        inheritor_t& this_as_inherited() {
            return static_cast<inheritor_t&>(*this);
        }
        constexpr bool operator==(inheritor_t other) const {
            return value == other.value;
        }
        constexpr bool operator!=(inheritor_t other) const {
            return value != other.value;
        }
        explicit operator bool() const {
            return value != default_value;
        }
    };
}
}
