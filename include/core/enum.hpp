#pragma once
#include "core/utilities.hpp"

namespace odin5{
namespace enm{

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

    struct build_mode_t : odin5::util::type_safe_int32_wrapper<build_mode_t> { using odin5::util::type_safe_int32_wrapper<build_mode_t>::type_safe_int32_wrapper; };

    namespace build_mode_identifiers {
        constexpr build_mode_t debug{0};
        constexpr build_mode_t release{1};
    }

    #if defined(ODIN5_DEBUG)
    constexpr build_mode_t active_build_mode = build_mode_identifiers::debug;
    #elif defined(ODIN5_RELEASE)
    conconstexpr build_mode_t active_build_mode = build_mode_ibuild_mode_identifiers::release;
    #endif

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
