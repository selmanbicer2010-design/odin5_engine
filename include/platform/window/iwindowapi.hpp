#pragma once
#include "core/enum.hpp"
#include "core/event.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <string>

namespace odin5{
namespace platform{
namespace window{

    struct window_create_info {
        std::string window_name {"ODIN"};
        glm::u32vec2 window_size {1000, 1000};
        glm::u32vec2 window_pos {0, 32};
    };

    template <class window_interface_spec>
    struct i_window_api {
        using constexpr_window_interface_spec = odin5::util::constexpr_class<window_interface_spec>;
        static constexpr bool v =
            constexpr_window_interface_spec::template constructible_from<window_create_info>::v &&
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::terminate), odin5::enm::error_t>::v &&
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::update), odin5::enm::error_t>::v &&
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::should_close), bool>::v &&
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_framebuffer_size), glm::u32vec2>::v &&
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::u32vec2>, &window_interface_spec::framebuffer_resized>::v;
    };

    template <class window_interface_spec>
    concept i_window_api_v = i_window_api<window_interface_spec>::v;

    template <i_window_api_v window_api_spec>
    class window_api : public window_api_spec {
        using window_api_spec::window_api_spec;
    };
}
}
}
