#include <print>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "platform/graphics/graphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include "platform/window/glfwwindowapi.hpp"
#include "platform/window/windowapi.hpp"

int main() {
    odin5::platform::window::active_window_api window_api{odin5::platform::window::window_create_info{}};
    odin5::platform::graphics::active_graphics_api graphics_api{odin5::platform::graphics::graphics_create_info{}, window_api};

    while (!window_api.should_close()) {
        window_api.update();
    }
}
