#pragma once
#include <GLFW/glfw3.h>
#include "core/utilities.hpp"
#include "core/enum.hpp"
#include "platform/window/iwindowapi.hpp"

namespace odin5 {
namespace platform{
namespace window {
namespace glfw {

    struct glfw_state {
    public:
        GLFWwindow* window_p;
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
        ~window_api_spec();

        GLFWwindow* get_glfw_native_window();
    };

    using window_api = odin5::platform::window::window_api<window_api_spec>;
}
}
}
}
