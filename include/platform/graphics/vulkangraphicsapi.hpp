#pragma once
#include <array>
#include <cstddef>
#include "core/utilities.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS 1
#include <vulkan/vulkan_raii.hpp>
#include <utility>
#include "core/math/spatial.hpp"

namespace odin5{
namespace platform{
namespace graphics{
namespace vulkan{

    struct gpu_memory_manager {};

    struct vulkan_state {
    public:
        static constexpr int32_t MAX_FRAMES_IN_FLIGHT = 2;
        static constexpr int32_t FIF = MAX_FRAMES_IN_FLIGHT;

        vk::raii::Context context{};
        vk::raii::Instance instance{std::nullptr_t{}};

        vk::raii::PhysicalDevice physical_device{std::nullptr_t{}};
        vk::raii::Device device{std::nullptr_t{}};

        vk::raii::SurfaceKHR surface{std::nullptr_t{}};
        vk::raii::SwapchainKHR swap_chain{std::nullptr_t{}};
        std::vector<vk::Image> swap_chain_images;
        vk::SurfaceFormatKHR swap_chain_format;
        vk::Extent2D swap_chain_extent;
        std::vector<vk::raii::ImageView> swap_chain_image_views;

        vk::raii::Queue queue{std::nullptr_t{}};

        vk::raii::PipelineLayout pipeline_layout{std::nullptr_t{}};
        vk::raii::Pipeline graphics_pipeline{std::nullptr_t{}};

        vk::raii::Buffer vertex_buffer{std::nullptr_t{}};
        vk::raii::DeviceMemory device_memory{std::nullptr_t{}};

        vk::raii::CommandPool command_pool{std::nullptr_t{}};
        std::array<vk::raii::CommandBuffer, FIF> command_buffers {odin5::util::construct_array_as<vk::raii::CommandBuffer, std::nullptr_t>(std::make_index_sequence<FIF>{})};

        std::array<vk::raii::Semaphore, FIF> present_complete_semaphores {odin5::util::construct_array_as<vk::raii::Semaphore, std::nullptr_t>(std::make_index_sequence<FIF>{})};
        std::vector<vk::raii::Semaphore> render_finished_semaphores{};
        std::array<vk::raii::Fence, FIF> flight_fences {odin5::util::construct_array_as<vk::raii::Fence, std::nullptr_t>(std::make_index_sequence<FIF>{})};

        uint32_t frame_index = 0;
        uint32_t image_index = 0;

        uint32_t queue_idx = UINT32_MAX;

        static constexpr vk::VertexInputBindingDescription vertex_binding_description = {.binding = 0, .stride = sizeof(odin5::math::spatial::vertex), .inputRate = vk::VertexInputRate::eVertex};

        static constexpr std::array<vk::VertexInputAttributeDescription, 3> vertex_attribute_descriptions = {{
            {.location = 0, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, position)},
            {.location = 1, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, normal)},
            {.location = 2, .binding = 0, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, uv)}
            }};

        #ifdef ODIN5_DEBUG
        static constexpr bool using_validation_layers = true;
        static constexpr std::array<const char*, 1> validation_layers = {"VK_LAYER_KHRONOS_validation"};
        vk::raii::DebugUtilsMessengerEXT debug_msgr{std::nullptr_t{}};
        #elifdef ODIN5_RELEASE
        static constexpr bool using_validation_layers = false;
        static constexpr std::array<const char*, 0> validation_layers = {};
        vk::raii::DebugUtilsMessengerEXT debug_msgr{std::nullptr_t{}};
        #endif
    };

    class graphics_api_spec {
    private:
        vulkan_state vulkan_{};
        odin5::platform::window::active_window_api* window_api_p = nullptr;

    public:
        graphics_api_spec(odin5::platform::graphics::graphics_create_info gci, odin5::platform::window::active_window_api& window_api);
        void set_viewport(glm::vec4 vp);
        void set_scissor(glm::vec4 sc);
        void set_viewport_and_scissor(glm::vec4 vp);
        void set_clear_color(glm::vec4 clear);
        void draw();
        void framebuffer_resized();
        ~graphics_api_spec();

        odin5::platform::graphics::graphics_dynamic_info dynamic_info;
    };

    using graphics_api = odin5::platform::graphics::graphics_api<graphics_api_spec>;
}
}
}
}
