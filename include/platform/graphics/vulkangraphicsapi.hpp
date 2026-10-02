#pragma once
#include <array>
#include <cstddef>
#include "platform/graphics/igraphicsapi.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace odin5{
namespace platform{
namespace graphics{
namespace vulkan{

    struct vulkan_state {
    public:
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

        vk::raii::CommandPool command_pool{std::nullptr_t{}};
        vk::raii::CommandBuffer command_buffer{std::nullptr_t{}};

        vk::raii::Semaphore present_complete_semaphore{std::nullptr_t{}};
        std::vector<vk::raii::Semaphore> render_finished_semaphores;
        vk::raii::Fence draw_fence{std::nullptr_t{}};

        //uint32_t image_index = 0;

        uint32_t queue_idx = UINT32_MAX;

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
