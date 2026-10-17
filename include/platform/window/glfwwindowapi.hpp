#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/ext/vector_uint4_sized.hpp>
#include "core/enum.hpp"
#include "core/event.hpp"
#include "core/utilities.hpp"
#include "platform/window/iwindowapi.hpp"
#include <glm/ext/vector_int2.hpp>
#include <utility>
#include "platform/window/windowapienum.hpp"

namespace odin5 {
namespace platform{
namespace window {
namespace glfw {

    struct fullscreen_info {
        bool decoration_preference = true;
        window::enm::fullscreen_mode_t mode = window::enm::fullscreen_modes::free;
        glm::ivec2 position = {0, 0};
        glm::ivec2 size = {1, 1};
    };

    class window_api_spec;

    struct self_referential_window_info {
        window_api_spec* owner{nullptr};
        window_key_t key{0};
    };

    struct window_t {
        window_t(glm::ivec2 size, const char* name) :
        ptr(glfwCreateWindow(
            size.x,
            size.y,
            name,
            nullptr,
            nullptr
        )) {
            if constexpr (odin5::enm::active_build_mode == odin5::enm::build_mode_identifiers::debug) {
                if (!ptr) {
                    odin5::util::throw_except<std::runtime_error>("failed to create GLFW window");
                }
            }
        }
        window_t(window_t&& other) noexcept
                : ptr(std::exchange(other.ptr, nullptr)),
                  fsi(other.fsi),
                  srwi(other.srwi) {}
        window_t& operator=(window_t&&) = delete;
        window_t(const window_t&) = delete;
        window_t& operator=(const window_t&) = delete;

        GLFWwindow* ptr;
        fullscreen_info fsi;
        self_referential_window_info srwi;

        ~window_t() {
            if (ptr) {
                glfwDestroyWindow(ptr);
            }
        }
    };

    struct monitor_t {
        monitor_t() {};
        monitor_t(GLFWmonitor* ptr) : ptr(ptr), vidmode(ptr ? glfwGetVideoMode(ptr) : nullptr) {
            if constexpr (odin5::enm::active_build_mode == odin5::enm::build_mode_identifiers::debug) {
                if (!ptr) {
                    odin5::util::throw_except<std::runtime_error>("monitor constructor used nullptr");
                }
            }
        }
        GLFWmonitor* ptr = nullptr;
        const GLFWvidmode* vidmode = nullptr;
    };

    struct monitors_t {
        std::array<monitor_t, window::enm::max_monitor_count> array;
        int32_t monitor_count{0};
        monitor_key_t main_monitor{0};
    };

    struct glfw_state {
    public:
        odin5::util::sequential_unordered_map<window_t, window_key_t> windows;
         monitors_t monitors;

    };

    class window_api_spec {
    private:
        glfw_state glfw_{};
        bool terminated_ = false;

    public:
        window_api_spec();

        [[nodiscard]] window_key_t create_window(odin5::platform::window::window_create_info wci);
        void destroy_window(window_key_t wn);

        odin5::enm::error_t update();
        bool should_close(window_key_t wn);
        odin5::enm::error_t terminate();

        [[nodiscard]] glm::ivec2 get_framebuffer_size(window_key_t wn);
        [[nodiscard]] float get_framebuffer_aspect_ratio(window_key_t wn);
        [[nodiscard]] glm::ivec2 get_window_size(window_key_t wn);
        [[nodiscard]] glm::ivec2 get_window_position(window_key_t wn);
        [[nodiscard]] window::enm::fullscreen_mode_t get_fullscreen_mode(window_key_t wn);

        void set_window_size(window_key_t wn, glm::ivec2 size);
        void set_window_position(window_key_t wn, glm::ivec2 position);
        void set_fullscreen_mode(window_key_t wn, window::enm::fullscreen_mode_t state);
        void set_decorated(window_key_t wn, bool state);
        void set_decoration_preference(window_key_t wn, bool state);

        [[nodiscard]] monitor_key_t get_main_monitor();
        [[nodiscard]] glm::ivec2 get_monitor_size(monitor_key_t mn);
        [[nodiscard]] glm::ivec2 get_monitor_position(monitor_key_t mn);
        [[nodiscard]] uint32_t get_monitor_refresh_rate(monitor_key_t mn);

        [[nodiscard]] window::enm::input::action_t key_state(window_key_t wn, window::enm::input::key_t key);
        [[nodiscard]] window::enm::input::action_t mouse_state(window_key_t wn, window::enm::input::mouse_button_t button);
        [[nodiscard]] glm::vec2 get_mouse_position(window_key_t wn);

        odin5::event::basic_event<window_key_t, glm::ivec2> window_move;
        odin5::event::basic_event<window_key_t, glm::ivec2> window_resize;
        odin5::event::basic_event<window_key_t> window_close;
        odin5::event::basic_event<window_key_t> window_refresh;
        odin5::event::basic_event<window_key_t, window::enm::enum_t> window_focus;
        odin5::event::basic_event<window_key_t, window::enm::enum_t> window_iconify;
        odin5::event::basic_event<window_key_t, window::enm::enum_t> window_maximize;
        odin5::event::basic_event<window_key_t, glm::ivec2> framebuffer_resize;
        odin5::event::basic_event<window_key_t, glm::vec2> window_content_scale;

        odin5::event::basic_event<window_key_t, window::enm::input::key_t, window::enm::input::action_t, window::enm::input::mods_t> key;
        odin5::event::basic_event<window_key_t, uint32_t> character;

        odin5::event::basic_event<window_key_t, window::enm::input::mouse_button_t, window::enm::input::action_t, window::enm::input::mods_t> mouse_button;
        odin5::event::basic_event<window_key_t, glm::vec2> cursor_move;
        odin5::event::basic_event<window_key_t, window::enm::enum_t> cursor_enter;
        odin5::event::basic_event<window_key_t, glm::vec2> cursor_scroll;

        odin5::event::basic_event<uint32_t, const char**> file_drop;

        ~window_api_spec();
        [[nodiscard]] window_t& get_glfw_native_window_t(window_key_t wn);
        [[nodiscard]] monitor_t& get_glfw_native_monitor_t(monitor_key_t mn);

    };

    using window_api = odin5::platform::window::window_api<window_api_spec>;
}
}
}
}
