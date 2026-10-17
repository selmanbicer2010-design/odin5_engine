#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>
#include <thread>
#include "platform/graphics/graphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include "platform/window/iwindowapi.hpp"
#include "platform/window/windowapi.hpp"
#include "platform/window/windowapienum.hpp"
#include <iostream>

namespace window = odin5::platform::window;
namespace win_enm = window::enm;
namespace graphics = odin5::platform::graphics;

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
    odin5::platform::window::active_window_api window_api{};
    odin5::platform::graphics::active_graphics_api graphics_api{{}, window_api};
    odin5::math::spatial::camera_3d camera{};
    camera.transform.position = {0.f, 0.f, 2.f};

    window::monitor_key_t monitor = window_api.get_main_monitor();

    window::window_key_t wn0 = window_api.create_window({});
    graphics::surface_key_t sf0 = graphics_api.create_surface(wn0);

    window::window_key_t wn1 = window_api.create_window({});
    graphics::surface_key_t sf1 = graphics_api.create_surface(wn1);

    // auto half_monitor = static_cast<glm::vec2>(window_api.get_monitor_size(monitor)) * 0.5f;
    // window_api.set_window_size(window, half_monitor);
    // window_api.set_window_position(window, half_monitor * 0.5f);

    window_api.key.connect([&](auto wn, auto key, auto action, auto){ if (wn == wn0 and key == win_enm::input::keys::escape and action == win_enm::input::action::press) window_api.terminate(); });

    window_api.key.connect([&](auto wn, auto key, auto action, auto){
        if (wn == wn0 and key == win_enm::input::keys::f11 and action == win_enm::input::action::press) {
            window_api.get_fullscreen_mode(wn0) == win_enm::fullscreen_modes::free ? window_api.set_fullscreen_mode(wn0, win_enm::fullscreen_modes::exclusive) : window_api.set_fullscreen_mode(wn0, win_enm::fullscreen_modes::free);
        }
    });

    window_api.framebuffer_resize.do_and_connect(wn0, window_api.get_framebuffer_size(wn0), [&](auto key, glm::ivec2 framebuffer_size){
        if (key == wn0) {
            camera.aspect = window_api.get_framebuffer_aspect_ratio(wn0);
            graphics_api.set_viewport_and_scissor(sf0, glm::vec4{0.f, 0.f, framebuffer_size});
            graphics_api.framebuffer_resized(sf0);
        };
        if (key == wn1) {
            camera.aspect = window_api.get_framebuffer_aspect_ratio(wn1);
            graphics_api.set_viewport_and_scissor(sf1, glm::vec4{0.f, 0.f, framebuffer_size});
            graphics_api.framebuffer_resized(sf1);
        };
    });

    window_api.window_close.connect([&](auto wn){
        if (wn != wn0) {
            window_api.destroy_window(wn);
        }
    });

    odin5::platform::graphics::draw_info_3d a{
        .mesh_idx = graphics_api.upload_mesh({vertices0.begin(), vertices0.end()}, {indices0.begin(), indices0.end()})
    };

    odin5::platform::graphics::draw_3d_submit_info_t submit{};
    submit.push_back(a);

    glm::vec2 mouse_last_pos = window_api.get_mouse_position(wn0);

    while (!window_api.should_close(wn0)) {
        window_api.update();
        float s = 0.01f;

        glm::vec2 mouse_delta = (window_api.get_mouse_position(wn0) - mouse_last_pos) * 0.2f;

        glm::quat yaw = glm::angleAxis(glm::radians(static_cast<float>(mouse_delta.x)), glm::vec3{0.f, 1.f, 0.f});
        glm::quat pitch = glm::angleAxis(glm::radians(static_cast<float>(mouse_delta.y)), glm::vec3{1.f, 0.f, 0.f});

         if (window_api.mouse_state(wn0, win_enm::input::mouse_button::e2)) camera.transform.orientation = yaw * camera.transform.orientation * pitch;

        mouse_last_pos = window_api.get_mouse_position(wn0);

        if (window_api.key_state(wn0, win_enm::input::keys::w)) camera.transform.position += camera.transform.z_normal() * s;
        if (window_api.key_state(wn0, win_enm::input::keys::a)) camera.transform.position += camera.transform.x_normal() * s * -1.f;
        if (window_api.key_state(wn0, win_enm::input::keys::s)) camera.transform.position += camera.transform.z_normal() * s * -1.f;
        if (window_api.key_state(wn0, win_enm::input::keys::d)) camera.transform.position += camera.transform.x_normal() * s;
        if (window_api.key_state(wn0, win_enm::input::keys::space)) camera.transform.position += camera.transform.y_normal() * s;
        if (window_api.key_state(wn0, win_enm::input::keys::left_control)) camera.transform.position += camera.transform.y_normal() * s * -1.f;

        graphics_api.set_proj_3d(sf0, camera.proj());
        graphics_api.set_view_3d(sf0, camera.view());
        graphics_api.set_proj_3d(sf1, camera.proj());
        graphics_api.set_view_3d(sf1, camera.view());
        graphics_api.draw(sf0, submit);
        graphics_api.draw(sf1, submit);
    }
}
