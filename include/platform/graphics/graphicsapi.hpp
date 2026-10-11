#pragma once
#include "core/enum.hpp"
#include "platform/graphics/vulkangraphicsapi.hpp"
#include <type_traits>
#include "platform/graphics/graphicsapienum.hpp"

namespace odin5{
namespace platform{
namespace graphics{
    constexpr auto get_selected_graphics_api() {
        if constexpr (odin5::platform::graphics::enm::active_graphics_api == odin5::platform::graphics::enm::graphics_api_identifiers::vulkan) {
            return (odin5::platform::graphics::vulkan::graphics_api*){nullptr};
        }
        if constexpr (odin5::platform::graphics::enm::active_graphics_api == odin5::platform::graphics::enm::graphics_api_identifiers::opengl) {
            //return (odin5::platform::graphics::opengl::graphics_api*){nullptr};
        }
        static_assert(odin5::platform::graphics::enm::active_graphics_api != odin5::platform::graphics::enm::graphics_api_identifiers::unknown);
    }
    struct active_graphics_api_detail {
        using t = std::remove_pointer_t<decltype(get_selected_graphics_api())>;
    };
    using active_graphics_api = active_graphics_api_detail::t;
}
}
}
