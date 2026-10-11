#include "platform/window/glfwwindowapi.hpp"
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <stdexcept>
#include "platform/graphics/graphicsapienum.hpp"
#include "platform/window/windowapienum.hpp"

namespace{
    odin5::platform::window::glfw::window_api_spec& api_from_window(GLFWwindow* window_p) {
        return *static_cast<odin5::platform::window::glfw::window_api_spec*>(glfwGetWindowUserPointer(window_p));
    }

    void make_window_fullscreen_exclusive(GLFWwindow* window_p, GLFWmonitor* monitor_p, const GLFWvidmode* vidmode_p) {
        glfwSetWindowMonitor(window_p, monitor_p, 0, 0, vidmode_p->width, vidmode_p->height, vidmode_p->refreshRate);
    }

    void remove_window_fullscreen_exclusive(GLFWwindow* window_p, glm::ivec2 pos, glm::ivec2 size) {
        glfwSetWindowMonitor(window_p, nullptr, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
    }

    constexpr odin5::platform::window::enm::input::key_t glfw_to_key(int glfw_key) {
        using namespace odin5::platform::window::enm::input::keys;

        switch (glfw_key) {
            case GLFW_KEY_UNKNOWN: return unknown;

            case GLFW_KEY_SPACE: return space;
            case GLFW_KEY_APOSTROPHE: return apostrophe;
            case GLFW_KEY_COMMA: return comma;
            case GLFW_KEY_MINUS: return minus;
            case GLFW_KEY_PERIOD: return period;
            case GLFW_KEY_SLASH: return slash;

            case GLFW_KEY_0: return key_0;
            case GLFW_KEY_1: return key_1;
            case GLFW_KEY_2: return key_2;
            case GLFW_KEY_3: return key_3;
            case GLFW_KEY_4: return key_4;
            case GLFW_KEY_5: return key_5;
            case GLFW_KEY_6: return key_6;
            case GLFW_KEY_7: return key_7;
            case GLFW_KEY_8: return key_8;
            case GLFW_KEY_9: return key_9;

            case GLFW_KEY_A: return a;
            case GLFW_KEY_B: return b;
            case GLFW_KEY_C: return c;
            case GLFW_KEY_D: return d;
            case GLFW_KEY_E: return e;
            case GLFW_KEY_F: return f;
            case GLFW_KEY_G: return g;
            case GLFW_KEY_H: return h;
            case GLFW_KEY_I: return i;
            case GLFW_KEY_J: return j;
            case GLFW_KEY_K: return k;
            case GLFW_KEY_L: return l;
            case GLFW_KEY_M: return m;
            case GLFW_KEY_N: return n;
            case GLFW_KEY_O: return o;
            case GLFW_KEY_P: return p;
            case GLFW_KEY_Q: return q;
            case GLFW_KEY_R: return r;
            case GLFW_KEY_S: return s;
            case GLFW_KEY_T: return t;
            case GLFW_KEY_U: return u;
            case GLFW_KEY_V: return v;
            case GLFW_KEY_W: return w;
            case GLFW_KEY_X: return x;
            case GLFW_KEY_Y: return y;
            case GLFW_KEY_Z: return z;

            case GLFW_KEY_SEMICOLON: return semicolon;
            case GLFW_KEY_EQUAL: return equal;
            case GLFW_KEY_LEFT_BRACKET: return left_bracket;
            case GLFW_KEY_BACKSLASH: return backslash;
            case GLFW_KEY_RIGHT_BRACKET: return right_bracket;
            case GLFW_KEY_GRAVE_ACCENT: return grave_accent;

            case GLFW_KEY_WORLD_1: return world_1;
            case GLFW_KEY_WORLD_2: return world_2;

            case GLFW_KEY_ESCAPE: return escape;
            case GLFW_KEY_ENTER: return enter;
            case GLFW_KEY_TAB: return tab;
            case GLFW_KEY_BACKSPACE: return backspace;
            case GLFW_KEY_INSERT: return insert;
            case GLFW_KEY_DELETE: return delete_key;

            case GLFW_KEY_RIGHT: return right;
            case GLFW_KEY_LEFT: return left;
            case GLFW_KEY_DOWN: return down;
            case GLFW_KEY_UP: return up;

            case GLFW_KEY_PAGE_UP: return page_up;
            case GLFW_KEY_PAGE_DOWN: return page_down;
            case GLFW_KEY_HOME: return home;
            case GLFW_KEY_END: return end;

            case GLFW_KEY_CAPS_LOCK: return caps_lock;
            case GLFW_KEY_SCROLL_LOCK: return scroll_lock;
            case GLFW_KEY_NUM_LOCK: return num_lock;
            case GLFW_KEY_PRINT_SCREEN: return print_screen;
            case GLFW_KEY_PAUSE: return pause;

            case GLFW_KEY_F1: return f1;
            case GLFW_KEY_F2: return f2;
            case GLFW_KEY_F3: return f3;
            case GLFW_KEY_F4: return f4;
            case GLFW_KEY_F5: return f5;
            case GLFW_KEY_F6: return f6;
            case GLFW_KEY_F7: return f7;
            case GLFW_KEY_F8: return f8;
            case GLFW_KEY_F9: return f9;
            case GLFW_KEY_F10: return f10;
            case GLFW_KEY_F11: return f11;
            case GLFW_KEY_F12: return f12;
            case GLFW_KEY_F13: return f13;
            case GLFW_KEY_F14: return f14;
            case GLFW_KEY_F15: return f15;
            case GLFW_KEY_F16: return f16;
            case GLFW_KEY_F17: return f17;
            case GLFW_KEY_F18: return f18;
            case GLFW_KEY_F19: return f19;
            case GLFW_KEY_F20: return f20;
            case GLFW_KEY_F21: return f21;
            case GLFW_KEY_F22: return f22;
            case GLFW_KEY_F23: return f23;
            case GLFW_KEY_F24: return f24;
            case GLFW_KEY_F25: return f25;

            case GLFW_KEY_KP_0: return kp_0;
            case GLFW_KEY_KP_1: return kp_1;
            case GLFW_KEY_KP_2: return kp_2;
            case GLFW_KEY_KP_3: return kp_3;
            case GLFW_KEY_KP_4: return kp_4;
            case GLFW_KEY_KP_5: return kp_5;
            case GLFW_KEY_KP_6: return kp_6;
            case GLFW_KEY_KP_7: return kp_7;
            case GLFW_KEY_KP_8: return kp_8;
            case GLFW_KEY_KP_9: return kp_9;

            case GLFW_KEY_KP_DECIMAL: return kp_decimal;
            case GLFW_KEY_KP_DIVIDE: return kp_divide;
            case GLFW_KEY_KP_MULTIPLY: return kp_multiply;
            case GLFW_KEY_KP_SUBTRACT: return kp_subtract;
            case GLFW_KEY_KP_ADD: return kp_add;
            case GLFW_KEY_KP_ENTER: return kp_enter;
            case GLFW_KEY_KP_EQUAL: return kp_equal;

            case GLFW_KEY_LEFT_SHIFT: return left_shift;
            case GLFW_KEY_LEFT_CONTROL: return left_control;
            case GLFW_KEY_LEFT_ALT: return left_alt;
            case GLFW_KEY_LEFT_SUPER: return left_super;

            case GLFW_KEY_RIGHT_SHIFT: return right_shift;
            case GLFW_KEY_RIGHT_CONTROL: return right_control;
            case GLFW_KEY_RIGHT_ALT: return right_alt;
            case GLFW_KEY_RIGHT_SUPER: return right_super;

            case GLFW_KEY_MENU: return menu;

            default: return unknown;
        }
    }
    constexpr odin5::platform::window::enm::input::action_t glfw_to_action(int glfw_action) {
        using namespace odin5::platform::window::enm::input::action;

        switch (glfw_action) {
            case GLFW_RELEASE: return release;
            case GLFW_PRESS: return press;
            case GLFW_REPEAT: return repeat;
            default: return unknown;
        }
    }
    constexpr odin5::platform::window::enm::input::mouse_button_t glfw_to_mouse_button(int glfw_button) {
        using namespace odin5::platform::window::enm::input::mouse_button;

        switch (glfw_button) {
            case GLFW_MOUSE_BUTTON_1: return e1;
            case GLFW_MOUSE_BUTTON_2: return e2;
            case GLFW_MOUSE_BUTTON_3: return e3;
            case GLFW_MOUSE_BUTTON_4: return e4;
            case GLFW_MOUSE_BUTTON_5: return e5;
            default: return unknown;
        }
    }
    constexpr odin5::platform::window::enm::input::mods_t glfw_to_mods(int glfw_mods) {
        using namespace odin5::platform::window::enm::input::mods;
        odin5::platform::window::enm::input::mods_t ret{0b0};
        ret |= (
            shift.value * (static_cast<bool>(glfw_mods & GLFW_MOD_SHIFT)) |
            control.value * (static_cast<bool>(glfw_mods & GLFW_MOD_CONTROL)) |
            alt.value * (static_cast<bool>(glfw_mods & GLFW_MOD_ALT)) |
            super.value * (static_cast<bool>(glfw_mods & GLFW_MOD_SUPER)) |
            caps_lock.value * (static_cast<bool>(glfw_mods & GLFW_MOD_CAPS_LOCK)) |
            num_lock.value * (static_cast<bool>(glfw_mods & GLFW_MOD_NUM_LOCK))
        );
        return ret;
    }

    constexpr int key_to_glfw(odin5::platform::window::enm::input::key_t key) {
        using namespace odin5::platform::window::enm::input::keys;

        switch (key.value) {
            case unknown.value: return GLFW_KEY_UNKNOWN;

            case space.value: return GLFW_KEY_SPACE;
            case apostrophe.value: return GLFW_KEY_APOSTROPHE;
            case comma.value: return GLFW_KEY_COMMA;
            case minus.value: return GLFW_KEY_MINUS;
            case period.value: return GLFW_KEY_PERIOD;
            case slash.value: return GLFW_KEY_SLASH;

            case key_0.value: return GLFW_KEY_0;
            case key_1.value: return GLFW_KEY_1;
            case key_2.value: return GLFW_KEY_2;
            case key_3.value: return GLFW_KEY_3;
            case key_4.value: return GLFW_KEY_4;
            case key_5.value: return GLFW_KEY_5;
            case key_6.value: return GLFW_KEY_6;
            case key_7.value: return GLFW_KEY_7;
            case key_8.value: return GLFW_KEY_8;
            case key_9.value: return GLFW_KEY_9;

            case a.value: return GLFW_KEY_A;
            case b.value: return GLFW_KEY_B;
            case c.value: return GLFW_KEY_C;
            case d.value: return GLFW_KEY_D;
            case e.value: return GLFW_KEY_E;
            case f.value: return GLFW_KEY_F;
            case g.value: return GLFW_KEY_G;
            case h.value: return GLFW_KEY_H;
            case i.value: return GLFW_KEY_I;
            case j.value: return GLFW_KEY_J;
            case k.value: return GLFW_KEY_K;
            case l.value: return GLFW_KEY_L;
            case m.value: return GLFW_KEY_M;
            case n.value: return GLFW_KEY_N;
            case o.value: return GLFW_KEY_O;
            case p.value: return GLFW_KEY_P;
            case q.value: return GLFW_KEY_Q;
            case r.value: return GLFW_KEY_R;
            case s.value: return GLFW_KEY_S;
            case t.value: return GLFW_KEY_T;
            case u.value: return GLFW_KEY_U;
            case v.value: return GLFW_KEY_V;
            case w.value: return GLFW_KEY_W;
            case x.value: return GLFW_KEY_X;
            case y.value: return GLFW_KEY_Y;
            case z.value: return GLFW_KEY_Z;

            case semicolon.value: return GLFW_KEY_SEMICOLON;
            case equal.value: return GLFW_KEY_EQUAL;
            case left_bracket.value: return GLFW_KEY_LEFT_BRACKET;
            case backslash.value: return GLFW_KEY_BACKSLASH;
            case right_bracket.value: return GLFW_KEY_RIGHT_BRACKET;
            case grave_accent.value: return GLFW_KEY_GRAVE_ACCENT;

            case world_1.value: return GLFW_KEY_WORLD_1;
            case world_2.value: return GLFW_KEY_WORLD_2;

            case escape.value: return GLFW_KEY_ESCAPE;
            case enter.value: return GLFW_KEY_ENTER;
            case tab.value: return GLFW_KEY_TAB;
            case backspace.value: return GLFW_KEY_BACKSPACE;
            case insert.value: return GLFW_KEY_INSERT;
            case delete_key.value: return GLFW_KEY_DELETE;

            case right.value: return GLFW_KEY_RIGHT;
            case left.value: return GLFW_KEY_LEFT;
            case down.value: return GLFW_KEY_DOWN;
            case up.value: return GLFW_KEY_UP;

            case page_up.value: return GLFW_KEY_PAGE_UP;
            case page_down.value: return GLFW_KEY_PAGE_DOWN;
            case home.value: return GLFW_KEY_HOME;
            case end.value: return GLFW_KEY_END;

            case caps_lock.value: return GLFW_KEY_CAPS_LOCK;
            case scroll_lock.value: return GLFW_KEY_SCROLL_LOCK;
            case num_lock.value: return GLFW_KEY_NUM_LOCK;
            case print_screen.value: return GLFW_KEY_PRINT_SCREEN;
            case pause.value: return GLFW_KEY_PAUSE;

            case f1.value: return GLFW_KEY_F1;
            case f2.value: return GLFW_KEY_F2;
            case f3.value: return GLFW_KEY_F3;
            case f4.value: return GLFW_KEY_F4;
            case f5.value: return GLFW_KEY_F5;
            case f6.value: return GLFW_KEY_F6;
            case f7.value: return GLFW_KEY_F7;
            case f8.value: return GLFW_KEY_F8;
            case f9.value: return GLFW_KEY_F9;
            case f10.value: return GLFW_KEY_F10;
            case f11.value: return GLFW_KEY_F11;
            case f12.value: return GLFW_KEY_F12;
            case f13.value: return GLFW_KEY_F13;
            case f14.value: return GLFW_KEY_F14;
            case f15.value: return GLFW_KEY_F15;
            case f16.value: return GLFW_KEY_F16;
            case f17.value: return GLFW_KEY_F17;
            case f18.value: return GLFW_KEY_F18;
            case f19.value: return GLFW_KEY_F19;
            case f20.value: return GLFW_KEY_F20;
            case f21.value: return GLFW_KEY_F21;
            case f22.value: return GLFW_KEY_F22;
            case f23.value: return GLFW_KEY_F23;
            case f24.value: return GLFW_KEY_F24;
            case f25.value: return GLFW_KEY_F25;

            case kp_0.value: return GLFW_KEY_KP_0;
            case kp_1.value: return GLFW_KEY_KP_1;
            case kp_2.value: return GLFW_KEY_KP_2;
            case kp_3.value: return GLFW_KEY_KP_3;
            case kp_4.value: return GLFW_KEY_KP_4;
            case kp_5.value: return GLFW_KEY_KP_5;
            case kp_6.value: return GLFW_KEY_KP_6;
            case kp_7.value: return GLFW_KEY_KP_7;
            case kp_8.value: return GLFW_KEY_KP_8;
            case kp_9.value: return GLFW_KEY_KP_9;

            case kp_decimal.value: return GLFW_KEY_KP_DECIMAL;
            case kp_divide.value: return GLFW_KEY_KP_DIVIDE;
            case kp_multiply.value: return GLFW_KEY_KP_MULTIPLY;
            case kp_subtract.value: return GLFW_KEY_KP_SUBTRACT;
            case kp_add.value: return GLFW_KEY_KP_ADD;
            case kp_enter.value: return GLFW_KEY_KP_ENTER;
            case kp_equal.value: return GLFW_KEY_KP_EQUAL;

            case left_shift.value: return GLFW_KEY_LEFT_SHIFT;
            case left_control.value: return GLFW_KEY_LEFT_CONTROL;
            case left_alt.value: return GLFW_KEY_LEFT_ALT;
            case left_super.value: return GLFW_KEY_LEFT_SUPER;

            case right_shift.value: return GLFW_KEY_RIGHT_SHIFT;
            case right_control.value: return GLFW_KEY_RIGHT_CONTROL;
            case right_alt.value: return GLFW_KEY_RIGHT_ALT;
            case right_super.value: return GLFW_KEY_RIGHT_SUPER;

            case menu.value: return GLFW_KEY_MENU;

            default: return GLFW_KEY_UNKNOWN;
        }
    }
    constexpr int mouse_button_to_glfw(odin5::platform::window::enm::input::mouse_button_t button) {
        using namespace odin5::platform::window::enm::input::mouse_button;

        switch (button.value) {
            case e1.value: return GLFW_MOUSE_BUTTON_1;
            case e2.value: return GLFW_MOUSE_BUTTON_2;
            case e3.value: return GLFW_MOUSE_BUTTON_3;
            case e4.value: return GLFW_MOUSE_BUTTON_4;
            case e5.value: return GLFW_MOUSE_BUTTON_5;
            default: return GLFW_MOUSE_BUTTON_1;
        }
    }
}

odin5::platform::window::glfw::window_api_spec::window_api_spec(odin5::platform::window::window_create_info wci) {
    if (!glfwInit()) {
        odin5::util::throw_except<std::runtime_error>("failed to init glfw");
    }

    if constexpr (odin5::platform::graphics::enm::active_graphics_api != odin5::platform::graphics::enm::graphics_api_identifiers::opengl) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    glfwWindowHint(GLFW_DECORATED, wci.decorated);

    glfw_.monitor = glfwGetPrimaryMonitor();

    glfw_.window = glfwCreateWindow(
        wci.window_size.x,
        wci.window_size.y,
        wci.window_name.c_str(),
        nullptr,
        nullptr
    );

    if (!glfw_.window.ptr) {
        terminate();
        odin5::util::throw_except<std::runtime_error>("failed to init window");
    }

    if constexpr (odin5::platform::graphics::enm::active_graphics_api == odin5::platform::graphics::enm::graphics_api_identifiers::opengl) {
        glfwMakeContextCurrent(glfw_.window.ptr);
    }

    glfwSetWindowUserPointer(glfw_.window.ptr, this);
    glfwSetWindowPosCallback(glfw_.window.ptr, [](GLFWwindow* wp, int x, int y){ api_from_window(wp).window_move.fire({x, y}); });
    glfwSetWindowSizeCallback(glfw_.window.ptr, [](GLFWwindow* wp, int x, int y){ api_from_window(wp).window_resize.fire({x, y}); });
    glfwSetWindowCloseCallback(glfw_.window.ptr, [](GLFWwindow* wp){ api_from_window(wp).window_close.fire(); });
    glfwSetWindowRefreshCallback(glfw_.window.ptr, [](GLFWwindow* wp){ api_from_window(wp).window_refresh.fire(); });
    glfwSetWindowFocusCallback(glfw_.window.ptr, [](GLFWwindow* wp, int focus){ api_from_window(wp).window_focus.fire(focus == GLFW_FOCUSED ? window::enm::focused : window::enm::unfocused); });
    glfwSetWindowIconifyCallback(glfw_.window.ptr, [](GLFWwindow* wp, int iconify){ api_from_window(wp).window_iconify.fire(iconify == GLFW_ICONIFIED ? window::enm::iconified : window::enm::uniconified); });
    glfwSetWindowMaximizeCallback(glfw_.window.ptr, [](GLFWwindow* wp, int maximize){ api_from_window(wp).window_iconify.fire(maximize == GLFW_MAXIMIZED ? window::enm::maximized : window::enm::unmaximized); });
    glfwSetFramebufferSizeCallback(glfw_.window.ptr, [](GLFWwindow* wp, int x, int y){ api_from_window(wp).framebuffer_resize.fire(glm::ivec2{x, y}); });
    glfwSetWindowContentScaleCallback(glfw_.window.ptr, [](GLFWwindow* wp, float x, float y){ api_from_window(wp).window_content_scale.fire(glm::vec2{x, y}); });

    glfwSetKeyCallback(glfw_.window.ptr, [](GLFWwindow* wp, int key, int, int action, int mods){ api_from_window(wp).key.fire(glfw_to_key(key), glfw_to_action(action), glfw_to_mods(mods)); });
    glfwSetCharCallback(glfw_.window.ptr, [](GLFWwindow* wp, uint32_t c){ api_from_window(wp).character.fire(c); });
    glfwSetMouseButtonCallback(glfw_.window.ptr, [](GLFWwindow* wp, int button, int action, int mods){ api_from_window(wp).mouse_button.fire(glfw_to_mouse_button(button), glfw_to_action(action), glfw_to_mods(mods)); });
    glfwSetCursorPosCallback(glfw_.window.ptr, [](GLFWwindow* wp, double x, double y){ api_from_window(wp).cursor_move.fire({x, y}); });
    glfwSetCursorEnterCallback(glfw_.window.ptr, [](GLFWwindow* wp, int entered){ api_from_window(wp).cursor_enter.fire(entered == GLFW_TRUE ? window::enm::enter : window::enm::exit); });
    glfwSetScrollCallback(glfw_.window.ptr, [](GLFWwindow* wp, double x, double y){ api_from_window(wp).cursor_scroll.fire({x, y}); });

    glfwSetDropCallback(glfw_.window.ptr, [](GLFWwindow* wp, int count, const char** paths){ api_from_window(wp).file_drop.fire(count, paths); });

    glfw_.decoration_preference = wci.decorated;
    set_decorated(wci.decorated);
    set_window_position(wci.window_pos);
    set_fullscreen(wci.fullscreen);
}

bool odin5::platform::window::glfw::window_api_spec::should_close() {
    return glfwWindowShouldClose(glfw_.window.ptr) or terminated_;
}

odin5::enm::error_t odin5::platform::window::glfw::window_api_spec::update() {
    glfwPollEvents();
    return odin5::enm::err::NONE;
}

odin5::enm::error_t odin5::platform::window::glfw::window_api_spec::terminate() {
    if (terminated_)
        return odin5::enm::err::WINDOW_API_ALREADY_TERMINATED;

    terminated_ = true;
    if (glfw_.window.ptr) {
        glfwDestroyWindow(glfw_.window.ptr);
    }
    glfwTerminate();

    return odin5::enm::err::NONE;
}

glm::ivec2 odin5::platform::window::glfw::window_api_spec::get_framebuffer_size() {
    int32_t x, y;
    glfwGetFramebufferSize(glfw_.window.ptr, &x, &y);
    return glm::ivec2{x, y};
}

float odin5::platform::window::glfw::window_api_spec::get_framebuffer_aspect_ratio() {
    glm::ivec2 framebuffer_size = get_framebuffer_size();
    if (framebuffer_size.y == 0u) {
        return 1.f;
    }
    return static_cast<float>(framebuffer_size.x) / static_cast<float>(framebuffer_size.y);
}

glm::ivec2 odin5::platform::window::glfw::window_api_spec::get_window_size() {
    int32_t x, y;
    glfwGetWindowSize(glfw_.window.ptr, &x, &y);
    return {x, y};
}

glm::ivec2 odin5::platform::window::glfw::window_api_spec::get_window_position() {
    int32_t x, y;
    glfwGetWindowPos(glfw_.window.ptr, &x, &y);
    return {x, y};
}

glm::ivec2 odin5::platform::window::glfw::window_api_spec::get_monitor_size() {
    return {glfw_.monitor.vidmode->width, glfw_.monitor.vidmode->height};
}

glm::ivec2 odin5::platform::window::glfw::window_api_spec::get_monitor_position() {
    int32_t x, y;
    glfwGetMonitorPos(glfw_.monitor.ptr, &x, &y);
    return {x, y};
}

uint32_t odin5::platform::window::glfw::window_api_spec::get_monitor_refresh_rate() {
    return glfw_.monitor.vidmode->refreshRate;
}

odin5::platform::window::enm::fullscreen_mode_t odin5::platform::window::glfw::window_api_spec::get_fullscreen() {
    return glfw_.fsi.mode;
}

void odin5::platform::window::glfw::window_api_spec::set_window_size(glm::ivec2 size) {
    glfwSetWindowSize(glfw_.window.ptr, size.x, size.y);
}

void odin5::platform::window::glfw::window_api_spec::set_window_position(glm::ivec2 position) {
    glfwSetWindowPos(glfw_.window.ptr, position.x, position.y);
}

void odin5::platform::window::glfw::window_api_spec::set_fullscreen(window::enm::fullscreen_mode_t state) {
    if (glfw_.fsi.mode == state) return;

    if (glfw_.fsi.mode == window::enm::fullscreen_modes::free) {
        glfw_.fsi.position = get_window_position();
        glfw_.fsi.size = get_window_size();
        if (state == window::enm::fullscreen_modes::exclusive) {
            make_window_fullscreen_exclusive(glfw_.window.ptr, glfw_.monitor.ptr, glfw_.monitor.vidmode);
        }
        else if (state == window::enm::fullscreen_modes::borderless) {
            set_decorated(false);
            set_window_position(get_monitor_position());
            set_window_size(get_monitor_size());
        }
    }
    else if (glfw_.fsi.mode == window::enm::fullscreen_modes::exclusive) {
        if (state == window::enm::fullscreen_modes::free) {
            remove_window_fullscreen_exclusive(glfw_.window.ptr, glfw_.fsi.position, glfw_.fsi.size);
        }
        if (state == window::enm::fullscreen_modes::borderless) {
            remove_window_fullscreen_exclusive(glfw_.window.ptr, glfw_.fsi.position, glfw_.fsi.size);
            set_decorated(false);
            set_window_position(get_monitor_position());
            set_window_size(get_monitor_size());
        }
    }
    else if (glfw_.fsi.mode == window::enm::fullscreen_modes::borderless) {
        if (state == window::enm::fullscreen_modes::free) {
            set_decorated(glfw_.decoration_preference);
            set_window_position(glfw_.fsi.position);
            set_window_size(glfw_.fsi.size);
        }
        if (state == window::enm::fullscreen_modes::exclusive) {
            set_decorated(glfw_.decoration_preference);
            make_window_fullscreen_exclusive(glfw_.window.ptr, glfw_.monitor.ptr, glfw_.monitor.vidmode);
        }
    }
    glfw_.fsi.mode = state;
}

void odin5::platform::window::glfw::window_api_spec::set_decorated(bool state) {
    glfwSetWindowAttrib(glfw_.window.ptr, GLFW_DECORATED, state);
}

void odin5::platform::window::glfw::window_api_spec::set_decoration_preference(bool state) {
    glfw_.decoration_preference = state;
}

odin5::platform::window::enm::input::action_t odin5::platform::window::glfw::window_api_spec::key_state(window::enm::input::key_t key) {
    return glfw_to_action(glfwGetKey(glfw_.window.ptr, key_to_glfw(key)));
}

odin5::platform::window::enm::input::action_t odin5::platform::window::glfw::window_api_spec::mouse_state(window::enm::input::mouse_button_t button) {
    return glfw_to_action(glfwGetKey(glfw_.window.ptr, mouse_button_to_glfw(button)));
}

odin5::platform::window::glfw::window_api_spec::~window_api_spec() {
    terminate();
}

odin5::platform::window::glfw::window_t odin5::platform::window::glfw::window_api_spec::get_glfw_native_window() {
    return glfw_.window.ptr;
}

odin5::platform::window::glfw::monitor_t odin5::platform::window::glfw::window_api_spec::get_glfw_native_monitor() {
    return glfw_.monitor.ptr;
}
