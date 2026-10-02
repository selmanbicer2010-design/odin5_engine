#include "platform/window/glfwwindowapi.hpp"
#include "core/enum.hpp"
#include <GLFW/glfw3.h>

odin5::platform::window::glfw::window_api_spec::window_api_spec(odin5::platform::window::window_create_info wci) {
    odin5::util::error_if(!glfwInit(), "failed to init glfw");

    if constexpr (odin5::enm::active_graphics_api != odin5::enm::graphics_api_identifiers::opengl) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    glfw_.window_p = glfwCreateWindow(
        wci.window_size.x,
        wci.window_size.y,
        wci.window_name.c_str(),
        nullptr,
        nullptr
    );

    odin5::util::error_if(!glfw_.window_p, "failed to init window");

    if (!glfw_.window_p) {
        terminate();
        return;
    }

    if constexpr (odin5::enm::active_graphics_api == odin5::enm::graphics_api_identifiers::opengl) {
        glfwMakeContextCurrent(glfw_.window_p);
    }
}

bool odin5::platform::window::glfw::window_api_spec::should_close() {
    return glfwWindowShouldClose(glfw_.window_p);
}

odin5::enm::error_t odin5::platform::window::glfw::window_api_spec::update() {
    glfwPollEvents();
    return odin5::enm::err::NONE;
}

odin5::enm::error_t odin5::platform::window::glfw::window_api_spec::terminate() {
    if (terminated_)
        return odin5::enm::err::WINDOW_API_ALREADY_TERMINATED;

    terminated_ = true;
    glfwDestroyWindow(glfw_.window_p);
    glfwTerminate();

    return odin5::enm::err::NONE;
}

glm::vec2 odin5::platform::window::glfw::window_api_spec::get_framebuffer_size() {
    int32_t x, y;
    glfwGetFramebufferSize(glfw_.window_p, &x, &y);
    return {x, y};
}

odin5::platform::window::glfw::window_api_spec::~window_api_spec() {
    terminate();
}

GLFWwindow* odin5::platform::window::glfw::window_api_spec::get_glfw_native_window() {
    return glfw_.window_p;
}
