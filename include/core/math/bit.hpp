#pragma once
#include <cstdint>
#include "core/math/math.hpp"

namespace odin5{
namespace math{
namespace bit{
    constexpr uint64_t KiB = odin5::math::pow(2, 10);
    constexpr uint64_t MiB = odin5::math::pow(2, 20);
    constexpr uint64_t GiB = odin5::math::pow(2, 30);
}
}
}
