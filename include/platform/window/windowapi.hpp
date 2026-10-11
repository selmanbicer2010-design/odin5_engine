#pragma once
#include "core/enum.hpp"
#include "platform/window/glfwwindowapi.hpp"
#include "platform/window/windowapienum.hpp"
#include <type_traits>

namespace odin5{
namespace platform{
namespace window{
    constexpr auto get_selected_window_api() {
        if constexpr (odin5::platform::window::enm::active_window_api == odin5::platform::window::enm::window_api_identifiers::glfw) {
            return (odin5::platform::window::glfw::window_api*){nullptr};
        }
        if constexpr (odin5::platform::window::enm::active_window_api == odin5::platform::window::enm::window_api_identifiers::sdl3) {
            //return (odin5::platform::window::sdl3::window_api*){nullptr};
        }
        static_assert(odin5::platform::window::enm::active_window_api != odin5::platform::window::enm::window_api_identifiers::unknown);
    }
    struct active_window_api_detail {
        using t = std::remove_pointer_t<decltype(get_selected_window_api())>;
    };
    using active_window_api = active_window_api_detail::t;
}
}
}
