#pragma once
#include <any>
#include <concepts>
#include <string>
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include "platform/window/windowapi.hpp"

namespace odin5{
namespace platform{
namespace graphics{

    struct graphics_create_info {
        std::string application_name = "NONE_SET";
        std::string engine_name = "ODIN5";

    };

    template <class graphics_interface_spec>
    struct i_graphics_api_struct {
        using constexpr_class = odin5::util::constexpr_class<graphics_interface_spec>;
        static constexpr bool v =
            constexpr_class::template constructible_from<graphics_create_info, odin5::platform::window::active_window_api&>::v;
            //constexpr_class::template method_signature_is<decltype(&graphics_interface_spec::foo), void, int>::v;
    };

    template <class graphics_interface_spec>
    concept i_graphics_api = i_graphics_api_struct<graphics_interface_spec>::v;

    template <i_graphics_api graphics_api_spec>
    class graphics_api : public graphics_api_spec {
        using graphics_api_spec::graphics_api_spec;
    };
}
}
}
