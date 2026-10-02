#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>
#include "core/utilities.hpp"
#include "platform/window/windowapi.hpp"
#include <glm/glm.hpp>

namespace odin5{
namespace platform{
namespace graphics{

    struct graphics_create_info {
        std::string application_name = "NONE_SET";
        std::string engine_name = "ODIN5";

    };

    struct graphics_dynamic_info {
        glm::vec4 viewport {0.f, 0.f, 0.f, 0.f};
        glm::vec4 scissor {0.f, 0.f, 0.f, 0.f};
        glm::vec4 clear_color {};

    };

    template <class graphics_interface_spec>
    struct i_graphics_api {
        using constexpr_graphics_interface_spec = odin5::util::constexpr_class<graphics_interface_spec>;
        static constexpr bool v =
            constexpr_graphics_interface_spec::template constructible_from<graphics_create_info, odin5::platform::window::active_window_api&>::v &&
            constexpr_graphics_interface_spec::template has_member<graphics_dynamic_info, &graphics_interface_spec::dynamic_info>::v &&
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_viewport), void, glm::vec4>::v &&
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_scissor), void, glm::vec4>::v &&
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_viewport_and_scissor), void, glm::vec4>::v &&
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_clear_color), void, glm::vec4>::v;
    };

    template <class graphics_interface_spec>
    concept i_graphics_api_v = i_graphics_api<graphics_interface_spec>::v;

    template <i_graphics_api_v graphics_api_spec>
    class graphics_api : public graphics_api_spec {
        using graphics_api_spec::graphics_api_spec;
    };
}
}
}
