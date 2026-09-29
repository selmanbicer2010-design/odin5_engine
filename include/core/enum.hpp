#pragma once
#include "core/utilities.hpp"

namespace odin5{
namespace enm{

    struct window_api_identifier_t : odin5::util::type_safe_int32_wrapper<window_api_identifier_t> { using odin5::util::type_safe_int32_wrapper<window_api_identifier_t>::type_safe_int32_wrapper; };
    struct graphics_api_identifier_t : odin5::util::type_safe_int32_wrapper<graphics_api_identifier_t> { using odin5::util::type_safe_int32_wrapper<graphics_api_identifier_t>::type_safe_int32_wrapper; };

    namespace window_api_identifiers {
        constexpr window_api_identifier_t unknown{0};
        constexpr window_api_identifier_t glfw{1};
        constexpr window_api_identifier_t sdl3{2};
    }
    namespace graphics_api_identifiers {
        constexpr graphics_api_identifier_t unknown{0};
        constexpr graphics_api_identifier_t vulkan{1};
        constexpr graphics_api_identifier_t opengl{2};
    }

    constexpr window_api_identifier_t active_window_api = window_api_identifiers::glfw;
    constexpr graphics_api_identifier_t active_graphics_api = graphics_api_identifiers::vulkan;

    struct error_t : odin5::util::type_safe_int32_wrapper<error_t> { using odin5::util::type_safe_int32_wrapper<error_t>::type_safe_int32_wrapper; };

    constexpr error_t ODIN5_ERROR_NONE{0};
    constexpr error_t ODIN5_ERROR_WINDOW_API_ALREADY_TERMINATED{1};
}
}
