#pragma once
#include <array>
#include <chrono>
#include <cinttypes> // IWYU pragma: keep
#include <concepts>
#include <print>
#include <source_location>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace odin5{
namespace util{

    struct null_t {
        template <typename... args_t>
        null_t(args_t...) {}
    };

    template <typename... args_t>
    struct static_portable_pack {};

    template <int32_t the_odr_violation_station = 0>
    double unix_time() {
        return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    template <typename... args_t>
    constexpr void silence_compiler_unused(const args_t&...) {

    };

    template<typename value_type, value_type... args>
    constexpr std::array<value_type, sizeof...(args)> make_inferred_array() {
        return { args... };
    }

    template <typename value_t, typename initializer_t, size_t... indices>
    constexpr std::array<value_t, sizeof...(indices)> construct_array_as(std::index_sequence<indices...>) {
        auto trick_compiler = [](size_t index, initializer_t initialized) constexpr { odin5::util::silence_compiler_unused(index); return initialized; };
        std::array<value_t, sizeof...(indices)> arr{ (trick_compiler(indices, initializer_t{}))... };
        return arr;
    }

    template <typename T>
    constexpr std::string_view type_name() {
        return typeid(T).name();
    }

    template <int32_t count = 0>
    void error_if(bool cond, const char* tag = nullptr) {
    #ifdef ODIN5_DEBUG
        if (cond) {
            std::println("err: {}", tag ? tag : "none set");
        }
    #endif
    }

    template <int32_t count = 0>
    void error(const char* tag = nullptr) {
    #ifdef ODIN5_DEBUG
        std::println("err: {}", tag ? tag : "none set");
    #endif
    }

    template <typename except_t = std::runtime_error>
    void throw_except(const char* tag = nullptr, std::source_location src_loc = std::source_location::current()) {
        std::string msg = std::string{"\""} + std::string{tag ? tag : "none set"} + std::string{"\" of type "} + std::string{odin5::util::type_name<except_t>()} + std::string{" thrown by "} + src_loc.function_name() + std::string{" at line "} + std::to_string(src_loc.line()) + std::string{" in file "} + src_loc.file_name();
        std::println("except: {}", msg.c_str());
        throw except_t{
            msg
        };
    }

    template <typename ret_t, typename... args_t>
    struct func {
        using t = ret_t(args_t...);
    };

    template <typename ret_t, typename... args_t>
    using func_t = typename odin5::util::func<ret_t, args_t...>::t;

    template <typename ret_t, typename... args_t>
    struct func_ptr {
        using t = ret_t(*)(args_t...);
    };

    template <typename ret_t, typename... args_t>
    using func_ptr_t = typename odin5::util::func_ptr<ret_t, args_t...>::t;

    template <class class_t, typename ret_t, typename... args_t>
    struct method_ptr {
        using t = ret_t(class_t::*)(args_t...);
    };

    template <class class_t, typename ret_t, typename... args_t>
    using method_ptr_t = typename odin5::util::method_ptr<class_t, ret_t, args_t...>::t;

    template <class class_t, typename ret_t, typename... args_t>
    struct const_method_ptr {
        using t = ret_t(class_t::*)(args_t...) const;
    };

    template <class class_t, typename ret_t, typename... args_t>
    using const_method_ptr_t = typename odin5::util::const_method_ptr<class_t, ret_t, args_t...>::t;

    template <class class_t, typename method_t, typename ret_t, typename... args_t>
    concept has_method = std::same_as<method_t, method_ptr_t<class_t, ret_t, args_t...>>;

    template <class class_t, typename method_t, typename ret_t, typename... args_t>
    concept has_const_method = std::same_as<method_t, const_method_ptr_t<class_t, ret_t, args_t...>>;

    template <class class_t>
    struct constexpr_class {
        template <typename... args_t>
        struct constructible_from {
            static constexpr bool v = std::constructible_from<class_t, args_t...>;
        };
        template <typename method_t, typename ret_t, typename... args_t>
        struct has_method {
            static constexpr bool v = odin5::util::has_method<class_t, method_t, ret_t, args_t...>;
        };
        template <typename method_t, typename ret_t, typename... args_t>
        struct has_const_method {
            static constexpr bool v = odin5::util::has_const_method<class_t, method_t, ret_t, args_t...>;
        };
        template <typename method_t, typename ret_t, typename... args_t>
        struct has_static_method {
            static constexpr bool v = std::same_as<method_t, func_ptr_t<class_t, ret_t, args_t...>>;
        };
        template <typename member_t, member_t class_t::*member_ptr>
        struct has_member {
            static constexpr bool v = true;
        };
        template <typename>
        struct has_qualified_id { // use on templated functions that you just need to exist
            static constexpr bool v = true;
        };
    };

    template <typename inheritor_t, int32_t default_value_v = 0>
    struct type_safe_int32_wrapper {
        constexpr type_safe_int32_wrapper(int32_t v) : value(v) {}
        constexpr type_safe_int32_wrapper(const type_safe_int32_wrapper&) = default;
        constexpr type_safe_int32_wrapper(type_safe_int32_wrapper&&) = default;
        static constexpr int32_t default_value = default_value_v;
        int32_t value = default_value;
        inheritor_t* this_as_inherited() {
            return static_cast<inheritor_t*>(this);
        }
        constexpr type_safe_int32_wrapper& operator=(const type_safe_int32_wrapper<inheritor_t, default_value_v>& other) {
            value = other.value;
            return *this;
        }
        constexpr type_safe_int32_wrapper operator|(const type_safe_int32_wrapper<inheritor_t, default_value_v>& other) {
            return value | other.value;
        }
        constexpr type_safe_int32_wrapper& operator|=(const type_safe_int32_wrapper<inheritor_t, default_value_v>& other) {
            value |= other.value;
            return *this;
        }
        constexpr bool operator==(inheritor_t other) const {
            return value == other.value;
        }
        constexpr bool operator!=(inheritor_t other) const {
            return value != other.value;
        }
        explicit operator bool() const {
            return value;
        }
        explicit operator int32_t() const {
            return value;
        }
    };

    template <typename val_t>
    struct type_tuple_leaf {
        val_t val;
    };

    template <typename... args_t>
    class type_tuple : public type_tuple_leaf<args_t>... {
    public:
        template <typename val_t>
        val_t& get() {
            return static_cast<type_tuple_leaf<val_t>*>(this)->val;
        }
    };

    template <int32_t index, typename val_t>
    struct index_tuple_leaf {
        val_t val;
    };

    template <typename i_seq, typename... args_t>
    struct index_tuple_impl;

    template <size_t... indices, typename... args_t>
    struct index_tuple_impl<std::index_sequence<indices...>, args_t...> : public index_tuple_leaf<indices, args_t>... {
        template <int32_t index>
        requires (index >= 0) and (index <= sizeof...(indices))
        auto& get() {
            return get_leaf<index>(*this);
        }

        template <int32_t index, typename t>
        t& get_leaf(index_tuple_leaf<index, t>& leaf) {
            return leaf.val;
        }
    };

    template <typename... args_t>
    using index_tuple = index_tuple_impl<std::index_sequence_for<args_t...>, args_t...>;

    template <class value_ty, typename integer_like_t = int32_t>
    class sequential_unordered_map {
    using value_type = value_ty;
    public:
        static constexpr int32_t counter_min = 0;
        static constexpr int32_t invalid_key = counter_min - 1;
    private:
        int32_t counter_ = counter_min;
        std::unordered_map<int32_t, value_type> data_{};
    public:
        const std::unordered_map<int32_t, value_type>& unordered_map() const {
            return data_;
        }
        std::unordered_map<int32_t, value_type>& unordered_map() {
            return data_;
        }
        integer_like_t current_id() {
            return counter_ - 1;
        }
        const value_type& operator[](integer_like_t hash) const {
            return data_.at(static_cast<int32_t>(hash));
        }
        const value_type* find(integer_like_t key) const {
            auto it = data_.find(static_cast<int32_t>(key));
            if (it != data_.end()) {
                return &(it->second);
            }
            return nullptr;
        }
        value_type* find(integer_like_t key) {
            auto it = data_.find(static_cast<int32_t>(key));
            if (it != data_.end()) {
                return &(it->second);
            }
            return nullptr;
        }
        void erase(integer_like_t key) {
            data_.erase(static_cast<int32_t>(key));
        }
        [[nodiscard("value is left dangling without taking key")]]
        integer_like_t push(const value_type& value) {
            int32_t key = counter_++;
            data_.insert({key, value});
            return key;
        }
        [[nodiscard("value is left dangling without taking key")]]
        integer_like_t push(value_type&& value) {
            int32_t key = counter_++;
            data_.emplace(key, std::move(value));
            return key;
        }
    };

    template <typename value_ty>
    class unordered_vector {
    public:
        using value_type = value_ty;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = std::vector<value_type>::iterator;
        using const_iterator = std::vector<value_type>::const_iterator;
        using reverse_iterator = std::vector<value_type>::reverse_iterator;
        using const_reverse_iterator = std::vector<value_type>::const_reverse_iterator;
        [[nodiscard]] iterator begin() { return c.begin(); }
        [[nodiscard]] const_iterator begin() const { return c.begin(); }
        [[nodiscard]] const_iterator cbegin() const { return c.cbegin(); }

        [[nodiscard]] iterator end() { return c.end(); }
        [[nodiscard]] const_iterator end() const { return c.end(); }
        [[nodiscard]] const_iterator cend() const { return c.cend(); }

        [[nodiscard]] reverse_iterator rbegin() { return c.rbegin(); }
        [[nodiscard]] const_reverse_iterator rbegin() const { return c.rbegin(); }
        [[nodiscard]] const_reverse_iterator crbegin() const { return c.crbegin(); }

        [[nodiscard]] reverse_iterator rend() { return c.rend(); }
        [[nodiscard]] const_reverse_iterator rend() const { return c.rend(); }
        [[nodiscard]] const_reverse_iterator crend() const { return c.crend(); }

        [[nodiscard]] size_type data_size() const { return c.size(); }
        [[nodiscard]] size_type map_size() const { return a.size(); }
        [[nodiscard]] bool empty() const { return c.empty(); }

        [[nodiscard]] value_type* data() { return c.data(); }
        [[nodiscard]] const value_type* data() const { return c.data(); }

        void clear() { a.clear(); b.clear(); c.clear(); }

        static constexpr int32_t invalid_index = -1;
        std::vector<int32_t> a;
        std::vector<int32_t> b;
        std::vector<value_type> c;

        [[nodiscard]] reference operator[](size_type i) {
            #ifdef ODIN5_DEBUG
            if (out_of_bounds(i)) {
                odin5::util::throw_except<std::out_of_range>("out of bounds access on unordered_vector");
            }
            #endif
            return c[a[i]];
        }
        [[nodiscard]] const_reference operator[](size_type i) const {
            #ifdef ODIN5_DEBUG
            if (out_of_bounds(i)) {
                odin5::util::throw_except<std::out_of_range>("out of bounds access on unordered_vector");
            }
            #endif
            return c[a[i]];
        }

        int32_t next_handle() const { return static_cast<int32_t>(a.size()); }

        [[nodiscard]]
        int32_t push_back(const value_type& value) {
            int32_t handle = static_cast<int32_t>(a.size());
            a.push_back(static_cast<int32_t>(c.size()));
            b.push_back(handle);
            c.push_back(value);
            return handle;
        }

        [[nodiscard]]
        int32_t push_back(value_type&& value) {
            int32_t handle = static_cast<int32_t>(a.size());
            a.push_back(static_cast<int32_t>(c.size()));
            b.push_back(handle);
            c.push_back(std::move(value));
            return handle;
        }

        bool out_of_bounds(int32_t i) { // This is bad for perfooooorrmmmmmaaaaanccee!
            return (i < 0 || static_cast<size_type>(i) >= a.size() or a[i] == invalid_index);
        }

        bool in_bounds(int32_t i) {
            return !out_of_bounds(i);
        }

        value_type* find(int32_t i) {
            if (out_of_bounds(i)) return nullptr;
            return &(c[a[i]]);
        }

        void erase(int32_t i) {
            if (out_of_bounds(i)) return;
            int32_t ai = a[i];
            int32_t bback = b.back();
            c[ai] = std::move(c.back());
            c.pop_back();
            b[ai] = bback;
            b.pop_back();
            a[bback] = ai;
            a[i] = invalid_index;
        }
    };
}
}
