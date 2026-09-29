#pragma once
#include <array>
#include <cstddef>
#include "platform/graphics/igraphicsapi.hpp"
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

namespace odin5{
namespace platform{
namespace graphics{
namespace vulkan{

    struct vulkan_state {
    public:
        #ifdef ODIN5_DEBUG
        static constexpr bool using_validation_layers = true;
        static constexpr std::array<const char*, 1> validation_layers = {"VK_LAYER_KHRONOS_validation"};
        #elifdef ODIN5_RELEASE
        static constexpr bool using_validation_layers = false;
        static constexpr std::array<const char*, 0> validation_layers = {};
        #endif

        vk::raii::Context context{};
        vk::raii::Instance instance{std::nullptr_t{}};
        vk::raii::PhysicalDevice physical_device{std::nullptr_t{}};
        vk::raii::Device device{std::nullptr_t{}};

    };

    class graphics_api_spec {
    private:
        vulkan_state vulkan_{};

    public:
        graphics_api_spec(odin5::platform::graphics::graphics_create_info gci, odin5::platform::window::active_window_api& window_api);

        ~graphics_api_spec();
    };

    using graphics_api = odin5::platform::graphics::graphics_api<graphics_api_spec>;
}
}
}
}
