#pragma once
#include "core/utilities.hpp"

namespace odin5{
namespace platform{
namespace graphics{
namespace enm{

    struct graphics_api_identifier_t : odin5::util::type_safe_int32_wrapper<graphics_api_identifier_t> { using odin5::util::type_safe_int32_wrapper<graphics_api_identifier_t>::type_safe_int32_wrapper; };

    namespace graphics_api_identifiers {
        constexpr graphics_api_identifier_t unknown{0};
        constexpr graphics_api_identifier_t vulkan{1};
        constexpr graphics_api_identifier_t opengl{2};
    }

    constexpr graphics_api_identifier_t active_graphics_api = graphics_api_identifiers::vulkan;

}
}
}
}
