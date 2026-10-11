#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>
#include "core/math/spatial.hpp"
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

        glm::mat4 view_3d {1.f};
        glm::mat4 proj_3d {1.f};

    };

    struct mesh_idx_t : odin5::util::type_safe_int32_wrapper<mesh_idx_t> { using odin5::util::type_safe_int32_wrapper<mesh_idx_t>::type_safe_int32_wrapper; };

    struct draw_info_3d {
        mesh_idx_t mesh_idx;
    };

    using draw_3d_submit_info_t = std::vector<draw_info_3d>;
    using draw_3d_submit_info_constref_t = const draw_3d_submit_info_t&;

    template <class graphics_interface_spec>
    struct i_graphics_api {
        using constexpr_graphics_interface_spec = odin5::util::constexpr_class<graphics_interface_spec>;
        static constexpr bool v =
            constexpr_graphics_interface_spec::template constructible_from<graphics_create_info, odin5::platform::window::active_window_api&>::v and
            constexpr_graphics_interface_spec::template has_member<graphics_dynamic_info, &graphics_interface_spec::dynamic_info>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_viewport), void, glm::vec4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_scissor), void, glm::vec4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_viewport_and_scissor), void, glm::vec4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_view_3d), void, glm::mat4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_proj_3d), void, glm::mat4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::set_clear_color), void, glm::vec4>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::draw), void, draw_3d_submit_info_constref_t>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::framebuffer_resized), void>::v and
            constexpr_graphics_interface_spec::template has_method<decltype(&graphics_interface_spec::upload_mesh), mesh_idx_t, const std::vector<odin5::math::spatial::vertex>&, const std::vector<uint32_t>&>::v;
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
