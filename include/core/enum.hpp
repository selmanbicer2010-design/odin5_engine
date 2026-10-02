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

    struct operating_system_identifier_t : odin5::util::type_safe_int32_wrapper<operating_system_identifier_t> { using odin5::util::type_safe_int32_wrapper<operating_system_identifier_t>::type_safe_int32_wrapper; };

    namespace operating_system_identifiers {
        constexpr operating_system_identifier_t unknown{0};
        constexpr operating_system_identifier_t win32{1};
        constexpr operating_system_identifier_t win64{2};
        constexpr operating_system_identifier_t linux{3};
        constexpr operating_system_identifier_t macos{4};
    }

    #if defined(_WIN64)
    constexpr operating_system_identifier_t active_operating_system = operating_system_identifiers::win64;
    #elif defined(_WIN32)
    constexpr operating_system_identifier_t active_operating_system = operating_system_identifiers::win32;
    #elif defined(__linux__)
    constexpr operating_system_identifier_t active_operating_system = operating_system_identifiers::linux;
    #elif defined(__APPLE__)
    constexpr operating_system_identifier_t active_operating_system = operating_system_identifiers::macos;
    #else
    constexpr operating_system_identifier_t active_operating_system = operating_system_identifiers::unknown;
    #endif //defined(_WIN64)

    struct error_t : odin5::util::type_safe_int32_wrapper<error_t> { using odin5::util::type_safe_int32_wrapper<error_t>::type_safe_int32_wrapper; };
    using error_ptr_t = odin5::enm::error_t*;

    namespace err {
        constexpr error_t NONE{0};
        constexpr error_t WINDOW_API_ALREADY_TERMINATED{1};
        constexpr error_t FILE_NOT_FOUND{2};
        constexpr error_t FILE_OS_ERROR{3};
        constexpr error_t FILE_COULD_NOT_BE_OPENED{4};
        constexpr error_t FILE_COULD_NOT_BE_READ{5};
    }
}
}
