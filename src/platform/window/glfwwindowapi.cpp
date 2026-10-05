#include "platform/window/glfwwindowapi.hpp"
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <algorithm>

namespace{
    odin5::platform::window::glfw::window_api_spec& api_from_window(GLFWwindow* window_p) {
        return *static_cast<odin5::platform::window::glfw::window_api_spec*>(glfwGetWindowUserPointer(window_p));
    }
}

odin5::platform::window::glfw::window_api_spec::window_api_spec(odin5::platform::window::window_create_info wci) {
    if (!glfwInit()) {
        odin5::util::throw_except<std::runtime_error>("failed to init glfw");
    }

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

    if (!glfw_.window_p) {
        terminate();
        odin5::util::throw_except<std::runtime_error>("failed to init window");
    }

    if constexpr (odin5::enm::active_graphics_api == odin5::enm::graphics_api_identifiers::opengl) {
        glfwMakeContextCurrent(glfw_.window_p);
    }

    glfwSetWindowUserPointer(glfw_.window_p, this);
    glfwSetFramebufferSizeCallback(glfw_.window_p, [](GLFWwindow* wp, int x, int y){ api_from_window(wp).framebuffer_resized.fire(glm::u32vec2{x, y}); });

    glfwSetWindowPos(glfw_.window_p, wci.window_pos.x, wci.window_pos.y);
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
    if (glfw_.window_p) {
        glfwDestroyWindow(glfw_.window_p);
    }
    glfwTerminate();

    return odin5::enm::err::NONE;
}

glm::u32vec2 odin5::platform::window::glfw::window_api_spec::get_framebuffer_size() {
    int32_t x, y;
    glfwGetFramebufferSize(glfw_.window_p, &x, &y);
    return glm::u32vec2{x, y};
}

odin5::platform::window::glfw::window_api_spec::~window_api_spec() {
    terminate();
}

GLFWwindow* odin5::platform::window::glfw::window_api_spec::get_glfw_native_window() {
    return glfw_.window_p;
}
