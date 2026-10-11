#pragma once
#include "core/enum.hpp"
#include "core/event.hpp"
#include <cstdint>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <string>
#include <glm/ext/vector_int2.hpp>
#include "platform/window/windowapienum.hpp"

namespace odin5{
namespace platform{
namespace window{

    struct window_create_info {
        std::string window_name {"ODIN"};
        glm::ivec2 window_size {1000, 1000};
        glm::ivec2 window_pos {100, 100};
        window::enm::fullscreen_mode_t fullscreen = window::enm::fullscreen_modes::free;
        bool decorated = true;

    };

    template <class window_interface_spec>
    struct i_window_api {
        using constexpr_window_interface_spec = odin5::util::constexpr_class<window_interface_spec>;
        static constexpr bool v =
            constexpr_window_interface_spec::template constructible_from<window_create_info>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::should_close), bool>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::update), odin5::enm::error_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::terminate), odin5::enm::error_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_framebuffer_size), glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_framebuffer_aspect_ratio), float>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_window_size), glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_window_position), glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_monitor_size), glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_monitor_position), glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_monitor_refresh_rate), uint32_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::get_fullscreen), window::enm::fullscreen_mode_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::set_window_size), void, glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::set_window_position), void, glm::ivec2>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::set_fullscreen), void, window::enm::fullscreen_mode_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::set_decorated), void, bool>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::set_decoration_preference), void, bool>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::key_state), window::enm::input::action_t, window::enm::input::key_t>::v and
            constexpr_window_interface_spec::template has_method<decltype(&window_interface_spec::mouse_state), window::enm::input::action_t, window::enm::input::mouse_button_t>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::ivec2>, &window_interface_spec::window_move>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::ivec2>, &window_interface_spec::window_resize>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<>, &window_interface_spec::window_close>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<>, &window_interface_spec::window_refresh>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::enum_t>, &window_interface_spec::window_focus>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::enum_t>, &window_interface_spec::window_iconify>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::enum_t>, &window_interface_spec::window_maximize>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::ivec2>, &window_interface_spec::framebuffer_resize>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::vec2>, &window_interface_spec::window_content_scale>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::input::key_t, window::enm::input::action_t, window::enm::input::mods_t>, &window_interface_spec::key>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<uint32_t>, &window_interface_spec::character>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::input::mouse_button_t, window::enm::input::action_t, window::enm::input::mods_t>, &window_interface_spec::mouse_button>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::vec2>, &window_interface_spec::cursor_move>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<window::enm::enum_t>, &window_interface_spec::cursor_enter>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<glm::vec2>, &window_interface_spec::cursor_scroll>::v and
            constexpr_window_interface_spec::template has_member<odin5::event::basic_event<uint32_t, const char**>, &window_interface_spec::file_drop>::v;
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
