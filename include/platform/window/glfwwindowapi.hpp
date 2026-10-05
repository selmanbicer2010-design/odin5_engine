#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_uint2_sized.hpp>
#include "core/enum.hpp"
#include "core/event.hpp"
#include "platform/window/iwindowapi.hpp"

namespace odin5 {
namespace platform{
namespace window {
namespace glfw {

    struct glfw_state {
    public:
        GLFWwindow* window_p = nullptr;
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
        glm::u32vec2 get_framebuffer_size();
        odin5::event::basic_event<glm::u32vec2> framebuffer_resized;
        ~window_api_spec();
        GLFWwindow* get_glfw_native_window();
    };

    using window_api = odin5::platform::window::window_api<window_api_spec>;
}
}
}
}
