#include <cstdint>
#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>
#include <print>
#include <vector>
#include "core/math/spatial.hpp"
#include "platform/graphics/graphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include "platform/window/windowapi.hpp"

static constexpr auto vertices0 = odin5::util::make_inferred_array<odin5::math::spatial::vertex,
    { {0.f, -0.5f, 0.f}, {1.0f, 1.0f, 1.0f}, {} },
    { {0.5f, 0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {} },
    { {-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {} }
    >();

static constexpr auto vertices1 = odin5::util::make_inferred_array<odin5::math::spatial::vertex,
    { {-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}, {} },
    { { 0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {} },
    { { 0.0f,  0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {} }
    >();

int main() {
    odin5::platform::window::active_window_api window_api{odin5::platform::window::window_create_info{}};
    odin5::platform::graphics::active_graphics_api graphics_api{odin5::platform::graphics::graphics_create_info{}, window_api};

    window_api.framebuffer_resized.do_and_connect(window_api.get_framebuffer_size(), [&](glm::u32vec2 framebuffer_size){
        graphics_api.set_viewport_and_scissor(glm::vec4{0.f, 0.f, framebuffer_size});
        graphics_api.framebuffer_resized();
    });

    odin5::platform::graphics::draw_3d_submit_info_t submit{};
    submit.push_back({
        .mesh_idx = graphics_api.upload_mesh(std::vector<odin5::math::spatial::vertex>{vertices0.begin(), vertices0.end()})
    });
    submit.push_back({
        .mesh_idx = graphics_api.upload_mesh(std::vector<odin5::math::spatial::vertex>{vertices1.begin(), vertices1.end()})
    });

    while (!window_api.should_close()) {
        window_api.update();
        graphics_api.draw(submit);
    }
}
