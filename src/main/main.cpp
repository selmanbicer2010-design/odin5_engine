#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>
#include <print>
#include "platform/graphics/graphicsapi.hpp"
#include "platform/window/windowapi.hpp"

int main() {
    odin5::platform::window::active_window_api window_api{odin5::platform::window::window_create_info{}};
    odin5::platform::graphics::active_graphics_api graphics_api{odin5::platform::graphics::graphics_create_info{}, window_api};

    window_api.framebuffer_resized.do_and_connect(window_api.get_framebuffer_size(), [&](glm::u32vec2 framebuffer_size){
        graphics_api.set_viewport_and_scissor(glm::vec4{0.f, 0.f, framebuffer_size});
        graphics_api.framebuffer_resized();
    });

    while (!window_api.should_close()) {
        window_api.update();
        graphics_api.draw();
    }
}
