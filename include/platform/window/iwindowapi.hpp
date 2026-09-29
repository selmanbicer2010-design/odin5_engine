#pragma once
#include "core/enum.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <string>

namespace odin5{
namespace platform{
namespace window{

    struct window_create_info {
        std::string window_name {"ODIN"};
        glm::vec2 window_size {1000.f, 1000.f};
    };

    template <class window_interface_spec>
    struct i_window_api_struct {
        using constexpr_class = odin5::util::constexpr_class<window_interface_spec>;
        static constexpr bool v =
            constexpr_class::template constructible_from<window_create_info>::v &&
            constexpr_class::template method_signature_is<decltype(&window_interface_spec::terminate), odin5::enm::error_t>::v &&
            constexpr_class::template method_signature_is<decltype(&window_interface_spec::update), odin5::enm::error_t>::v &&
            constexpr_class::template method_signature_is<decltype(&window_interface_spec::should_close), bool>::v;
    };

    template <class window_interface_spec>
    concept i_window_api = i_window_api_struct<window_interface_spec>::v;

    template <i_window_api window_api_spec>
    class window_api : public window_api_spec {
        using window_api_spec::window_api_spec;
    };
}
}
}
