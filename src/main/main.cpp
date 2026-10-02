#include <glm/glm.hpp>
#include "core/file.hpp"
#include "core/utilities.hpp"
#include "platform/graphics/graphicsapi.hpp"
#include "platform/window/windowapi.hpp"
#include <iostream>

int main() {
    odin5::platform::window::active_window_api window_api{odin5::platform::window::window_create_info{}};
    odin5::platform::graphics::active_graphics_api graphics_api{odin5::platform::graphics::graphics_create_info{}, window_api};

    while (!window_api.should_close()) {
        window_api.update();
        graphics_api.set_viewport_and_scissor(glm::vec4{0.f, 0.f, window_api.get_framebuffer_size()});
        graphics_api.draw();
    }
}
