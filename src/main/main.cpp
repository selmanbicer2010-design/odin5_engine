#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>
#include <vector>
#include "core/math/spatial.hpp"
#include "core/utilities.hpp"
#include "platform/graphics/graphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include "platform/window/windowapi.hpp"
#include "platform/window/windowapienum.hpp"

namespace window = odin5::platform::window;
namespace win_enm = window::enm;

constexpr auto vertices0 = odin5::util::make_inferred_array<odin5::math::spatial::vertex,
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {} }, // 0
    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {} }, // 1
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {} }, // 2
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {} }, // 3
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {} }, // 4
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}, {} }, // 5
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {} }, // 6
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}, {} }  // 7
>();

constexpr auto indices0 = odin5::util::make_inferred_array<uint32_t,
    4, 5, 6, 6, 7, 4,   // +Z
    1, 0, 3, 3, 2, 1,   // -Z
    5, 1, 2, 2, 6, 5,   // +X
    0, 4, 7, 7, 3, 0,   // -X
    7, 6, 2, 2, 3, 7,   // +Y
    0, 1, 5, 5, 4, 0    // -Y
>();

int main() {
    odin5::platform::window::active_window_api window_api{{}};
    odin5::platform::graphics::active_graphics_api graphics_api{{}, window_api};

    auto half_monitor = static_cast<glm::vec2>(window_api.get_monitor_size()) * 0.5f;
    window_api.set_window_size(half_monitor);
    window_api.set_window_position(half_monitor * 0.5f);

    window_api.key.connect([&](auto key, auto action, auto){ if (key == win_enm::input::keys::escape and action == win_enm::input::action::press) window_api.terminate(); });

    window_api.key.connect([&](auto key, auto action, auto){
        if (key == win_enm::input::keys::f11 and action == win_enm::input::action::press) {
            window_api.get_fullscreen_mode() == win_enm::fullscreen_modes::free ? window_api.set_fullscreen_mode(win_enm::fullscreen_modes::exclusive) : window_api.set_fullscreen_mode(win_enm::fullscreen_modes::free);
        }
    });

    odin5::math::spatial::camera_3d camera{};
    camera.transform.position = {0.f, 0.f, 2.f};

    window_api.framebuffer_resize.do_and_connect(window_api.get_framebuffer_size(), [&](glm::ivec2 framebuffer_size){
        camera.aspect = window_api.get_framebuffer_aspect_ratio();
        graphics_api.set_viewport_and_scissor(glm::vec4{0.f, 0.f, framebuffer_size});
        graphics_api.framebuffer_resized();
    });

    odin5::platform::graphics::draw_info_3d a{
        .mesh_idx = graphics_api.upload_mesh({vertices0.begin(), vertices0.end()}, {indices0.begin(), indices0.end()})
    };

    odin5::platform::graphics::draw_3d_submit_info_t submit{};
    submit.push_back(a);

    glm::vec2 mouse_last_pos = window_api.get_mouse_position();

    while (!window_api.should_close()) {
        window_api.update();

        float s = 0.01f;

        glm::vec2 mouse_delta = (window_api.get_mouse_position() - mouse_last_pos) * 0.2f;

        glm::quat yaw = glm::angleAxis(glm::radians(static_cast<float>(mouse_delta.x)), glm::vec3{0.f, 1.f, 0.f});
        glm::quat pitch = glm::angleAxis(glm::radians(static_cast<float>(mouse_delta.y)), glm::vec3{1.f, 0.f, 0.f});

        if (window_api.mouse_state(win_enm::input::mouse_button::e2)) camera.transform.orientation = yaw * camera.transform.orientation * pitch;

        mouse_last_pos = window_api.get_mouse_position();

        if (window_api.key_state(win_enm::input::keys::w)) camera.transform.position += camera.transform.z_normal() * s;
        if (window_api.key_state(win_enm::input::keys::a)) camera.transform.position += camera.transform.x_normal() * s * -1.f;
        if (window_api.key_state(win_enm::input::keys::s)) camera.transform.position += camera.transform.z_normal() * s * -1.f;
        if (window_api.key_state(win_enm::input::keys::d)) camera.transform.position += camera.transform.x_normal() * s;
        if (window_api.key_state(win_enm::input::keys::space)) camera.transform.position += camera.transform.y_normal() * s;
        if (window_api.key_state(win_enm::input::keys::left_control)) camera.transform.position += camera.transform.y_normal() * s * -1.f;

        graphics_api.set_proj_3d(camera.proj());
        graphics_api.set_view_3d(camera.view());
        graphics_api.draw(submit);
    }
}
