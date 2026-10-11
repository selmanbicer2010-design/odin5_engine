#pragma once
#include <cstdint>

namespace odin5{
namespace math{
    constexpr uint64_t align_up_to(uint64_t base, uint64_t up_to) {
        return (base + up_to - 1) / up_to * up_to;
    }
    template <typename num_t>
    constexpr num_t pow(num_t base, int64_t exponent) {
        if (exponent == 0) {
            return 1;
        }
        if (exponent == 1) {
            return base;
        }
        num_t result = base;
        for (int64_t i {1}; i < exponent; i++) {
            result *= base;
        }
        return result;
    }
    constexpr int64_t factorial(int64_t x) {
        int64_t applied = 1;
        for (int64_t i{0}; i < x - 1; i++) {
            applied *= x - i;
        }
        return applied;
    }
    namespace detail{
        constexpr float trig_taylor_series_component(float x, int64_t degree) {
            return odin5::math::pow(x, degree) / static_cast<float>(odin5::math::factorial(degree));
        }
    }
    constexpr float sin(float x, int64_t precision = 5) {
        bool sign_flag = false;
        float result = x;
        for (int64_t i = 3; i < precision * 2 + 3; i += 2) {
            int64_t sign = sign_flag * 2 - 1;
            result += detail::trig_taylor_series_component(x, i) * sign;
            sign_flag = !sign_flag;
        }
        return result;
    }
    constexpr float cos(float x, int64_t precision = 5) {
        bool sign_flag = false;
        float result = 1;
        for (int64_t i = 2; i < precision * 2 + 2; i += 2) {
            int64_t sign = sign_flag * 2 - 1;
            result += detail::trig_taylor_series_component(x, i) * sign;
            sign_flag = !sign_flag;
        }
        return result;
    }
    constexpr float tan(float x, int64_t precision = 5) {
        return sin(x, precision) / cos(x, precision);
    }
}
}

constexpr float aaa = odin5::math::cos(10.f);
