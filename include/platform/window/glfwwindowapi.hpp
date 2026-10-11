#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/ext/vector_uint4_sized.hpp>
#include "core/enum.hpp"
#include "core/event.hpp"
#include "platform/window/iwindowapi.hpp"
#include <glm/ext/vector_int2.hpp>
#include "platform/window/windowapienum.hpp"

namespace odin5 {
namespace platform{
namespace window {
namespace glfw {

    struct window_t {
        window_t(GLFWwindow* ptr) : ptr(ptr) {}
        GLFWwindow* ptr = nullptr;
    };

    struct monitor_t {
        monitor_t(GLFWmonitor* ptr) : ptr(ptr), vidmode(ptr ? glfwGetVideoMode(ptr) : nullptr) { }
        GLFWmonitor* ptr = nullptr;
        const GLFWvidmode* vidmode = nullptr;
    };

    struct fullscreen_info {
        window::enm::fullscreen_mode_t mode = window::enm::fullscreen_modes::free;
        glm::ivec2 position = {0u, 0u};
        glm::ivec2 size = {1u, 1u};
    };

    struct glfw_state {
    public:
        window_t window {nullptr};
        monitor_t monitor {nullptr};

        fullscreen_info fsi;
        bool decoration_preference = true;

    };

    class window_api_spec {
    private:
        glfw_state glfw_{};
        bool terminated_ = false;

    public:
        window_api_spec(odin5::platform::window::window_create_info wci);
        bool should_close();
        odin5::enm::error_t update();
        odin5::enm::error_t terminate();
        [[nodiscard]] glm::ivec2 get_framebuffer_size();
        [[nodiscard]] float get_framebuffer_aspect_ratio();
        [[nodiscard]] glm::ivec2 get_window_size();
        [[nodiscard]] glm::ivec2 get_window_position();
        [[nodiscard]] glm::ivec2 get_monitor_size();
        [[nodiscard]] glm::ivec2 get_monitor_position();
        [[nodiscard]] uint32_t get_monitor_refresh_rate();
        [[nodiscard]] window::enm::fullscreen_mode_t get_fullscreen();
        void set_window_size(glm::ivec2 size);
        void set_window_position(glm::ivec2 position);
        void set_fullscreen(window::enm::fullscreen_mode_t state);
        void set_decorated(bool state);
        void set_decoration_preference(bool state);

        window::enm::input::action_t key_state(window::enm::input::key_t key);
        window::enm::input::action_t mouse_state(window::enm::input::mouse_button_t button);

        odin5::event::basic_event<glm::ivec2> window_move;
        odin5::event::basic_event<glm::ivec2> window_resize;
        odin5::event::basic_event<> window_close;
        odin5::event::basic_event<> window_refresh;
        odin5::event::basic_event<window::enm::enum_t> window_focus;
        odin5::event::basic_event<window::enm::enum_t> window_iconify;
        odin5::event::basic_event<window::enm::enum_t> window_maximize;
        odin5::event::basic_event<glm::ivec2> framebuffer_resize;
        odin5::event::basic_event<glm::vec2> window_content_scale;

        odin5::event::basic_event<window::enm::input::key_t, window::enm::input::action_t, window::enm::input::mods_t> key;
        odin5::event::basic_event<uint32_t> character;

        odin5::event::basic_event<window::enm::input::mouse_button_t, window::enm::input::action_t, window::enm::input::mods_t> mouse_button;
        odin5::event::basic_event<glm::vec2> cursor_move;
        odin5::event::basic_event<window::enm::enum_t> cursor_enter;
        odin5::event::basic_event<glm::vec2> cursor_scroll;

        odin5::event::basic_event<uint32_t, const char**> file_drop;

        ~window_api_spec();
        [[nodiscard]] window_t get_glfw_native_window();
        [[nodiscard]] monitor_t get_glfw_native_monitor();

    };

    using window_api = odin5::platform::window::window_api<window_api_spec>;
}
}
}
}
