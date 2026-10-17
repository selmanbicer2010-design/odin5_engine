#pragma once
#include "core/math/bit.hpp"
#include "platform/window/iwindowapi.hpp"
#include <glm/ext/matrix_double4x3_precision.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <vcruntime_new.h>
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS 1
#include <array>
#include <cstddef>
#include "core/utilities.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
#include <utility>
#include "core/math/spatial.hpp"

namespace odin5{
namespace platform{
namespace graphics{
namespace vulkan{

    static constexpr int32_t MAX_FRAMES_IN_FLIGHT = 2;
    static constexpr int32_t FIF = MAX_FRAMES_IN_FLIGHT;
    static constexpr uint32_t DEFAULT_ALLOCATION_SIZE = odin5::math::bit::MiB * 64;

    static constexpr vk::VertexInputBindingDescription vertex_binding_description = {.binding = 0, .stride = sizeof(odin5::math::spatial::vertex), .inputRate = vk::VertexInputRate::eVertex};

    static constexpr std::array<vk::VertexInputAttributeDescription, 3> vertex_attribute_descriptions = {{
        {.location = 0, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, position)},
        {.location = 1, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, normal)},
        {.location = 2, .binding = 0, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(odin5::math::spatial::vertex, uv)}
        }};

    struct uniform_buffer_object {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct vulkan_state;

    struct gpu_buffer_create_info {
        vk::DeviceSize byte_size;
        vk::BufferUsageFlags usage;
        vk::MemoryPropertyFlags properties;
        int32_t element_count;
        const void* data;
    };

    struct gpu_buffer {
        gpu_buffer(std::nullptr_t) {};
        gpu_buffer(vulkan_state& vk_state, const gpu_buffer_create_info& gbci);
        vk::BufferCreateInfo buffer_info{};
        vk::raii::Buffer vk_buffer{std::nullptr_t{}};
        vk::MemoryRequirements mem_reqs{};
        vk::MemoryAllocateInfo alloc_info{};
        vk::DeviceSize memory_offset {};
        int32_t byte_size;
        int32_t element_count;
        const void* data;

        vk::raii::Buffer* operator->() { return &vk_buffer; };
        vk::Buffer operator*() { return *vk_buffer; };
    };

    struct buffer_key_t : odin5::util::type_safe_int32_wrapper<buffer_key_t> { using odin5::util::type_safe_int32_wrapper<buffer_key_t>::type_safe_int32_wrapper; };
    struct memory_key_t : odin5::util::type_safe_int32_wrapper<memory_key_t> { using odin5::util::type_safe_int32_wrapper<memory_key_t>::type_safe_int32_wrapper; };

    struct gpu_allocated_memory {
    private:
        vulkan_state* vk_state {nullptr};
        vk::DeviceSize offset = 0;
    public:
        gpu_allocated_memory(std::nullptr_t) {};
        gpu_allocated_memory(vulkan_state& vk_state, const vk::MemoryAllocateInfo& alloc_info);
        gpu_allocated_memory(const gpu_allocated_memory&) = delete;
        gpu_allocated_memory& operator=(const gpu_allocated_memory&) = delete;
        gpu_allocated_memory(gpu_allocated_memory&& other) noexcept {
            *this = std::move(other);
        };
        gpu_allocated_memory& operator=(gpu_allocated_memory&& other) noexcept {
            if (this == &other) return *this;
            device_memory = std::move(other.device_memory);
            alloc_info = other.alloc_info;
            buffers = std::move(other.buffers);
            vk_state = other.vk_state;
            offset = other.offset;
            map_data = other.map_data;
            other.map_data = nullptr;
            return *this;
        };
        ~gpu_allocated_memory();

        vk::raii::DeviceMemory device_memory{std::nullptr_t{}};
        vk::MemoryAllocateInfo alloc_info{};
        odin5::util::unordered_vector<gpu_buffer> buffers{};

        bool can_fit(const gpu_buffer& buffer);

        [[nodiscard]]
        buffer_key_t insert_buffer(gpu_buffer&& buffer);
        [[nodiscard]]
        buffer_key_t emplace_buffer(const gpu_buffer_create_info& create_info);
        gpu_buffer& query_buffer(buffer_key_t key);
        void* map_data = nullptr;
        void map();
        void unmap();

        void write_into_buffer(buffer_key_t key, const void* data);

        void wipe() { unmap(); offset = 0; buffers.clear(); };

        vk::raii::DeviceMemory* operator->() { return &device_memory; }
        vk::DeviceMemory operator*() { return *device_memory; }
    };

    struct gpu_buffer_accessor {
        memory_key_t memory_key;
        buffer_key_t buffer_key;
    };

    struct gpu_memory_manager {
    private:
        vulkan_state* vk_state {nullptr};
    public:
        gpu_memory_manager(std::nullptr_t) {};
        gpu_memory_manager(vulkan_state& vk_state);

        odin5::util::unordered_vector<gpu_allocated_memory> memory{};
        gpu_allocated_memory staging_memory{std::nullptr_t{}};

        [[nodiscard]] memory_key_t get_matching_memory(const gpu_buffer& buffer);
        gpu_allocated_memory& query_memory(memory_key_t key);
        gpu_buffer_accessor insert_into_memory(gpu_buffer&& buffer);
        gpu_buffer_accessor insert_into_memory_staged(gpu_buffer&& buffer);
        gpu_buffer& find_in_memory(gpu_buffer_accessor key);

    };

    struct mesh_buffer_info {
        gpu_buffer_accessor vertex_buffer;
        gpu_buffer_accessor index_buffer;
    };

    struct gpu_resource_manager {
    private:
        vulkan_state* vk_state {nullptr};
    public:
        gpu_resource_manager(std::nullptr_t) {};
        gpu_resource_manager(vulkan_state& vk_state);

        odin5::util::sequential_unordered_map<mesh_buffer_info, mesh_idx_t> meshes{};
        mesh_idx_t create_mesh_buffers(const odin5::math::spatial::vertex* vertex_p, size_t vertex_size, const uint32_t* index_p, size_t index_size);

    };

    struct gpu_uniform_buffer_memory {
        gpu_uniform_buffer_memory(std::nullptr_t) {};
        gpu_uniform_buffer_memory(vulkan_state& vk_state);
        void write_into_memory_at(int32_t i, const uniform_buffer_object& data) {
            memory.write_into_buffer(keys[i], &data);
        }
        gpu_buffer& query_buffer_at(int32_t i) {
            return memory.query_buffer(keys[i]);
        }
        gpu_allocated_memory memory{std::nullptr_t{}};
        std::array<vulkan::buffer_key_t, FIF> keys = odin5::util::construct_array_as<vulkan::buffer_key_t, int32_t>(std::make_index_sequence<FIF>());
    };

    struct surface {
        window::window_key_t wn_key{0};
        vk::raii::SurfaceKHR surface_khr{std::nullptr_t{}};
        vk::raii::SwapchainKHR swap_chain{std::nullptr_t{}};
        std::vector<vk::Image> swap_chain_images{};
        vk::Extent2D swap_chain_extent{};
        std::vector<vk::raii::ImageView> swap_chain_image_views{};

        std::array<vk::raii::Semaphore, FIF> present_complete_semaphores {odin5::util::construct_array_as<vk::raii::Semaphore, std::nullptr_t>(std::make_index_sequence<FIF>{})};
        std::vector<vk::raii::Semaphore> render_finished_semaphores{};
        std::array<vk::raii::Fence, FIF> flight_fences {odin5::util::construct_array_as<vk::raii::Fence, std::nullptr_t>(std::make_index_sequence<FIF>{})};

        std::array<vk::raii::DescriptorSet, FIF> descriptor_sets = odin5::util::construct_array_as<vk::raii::DescriptorSet, std::nullptr_t>(std::make_index_sequence<FIF>());

        gpu_uniform_buffer_memory uniform_buffer{std::nullptr_t{}};

        std::array<vk::raii::CommandBuffer, FIF> command_buffers {odin5::util::construct_array_as<vk::raii::CommandBuffer, std::nullptr_t>(std::make_index_sequence<FIF>{})};

        uint32_t image_index = 0;
        uint32_t frame_index = 0;


        odin5::platform::graphics::graphics_dynamic_info dynamic_info{};
    };

    struct vulkan_state {
    public:
        vk::raii::Context context{};
        vk::raii::Instance instance{std::nullptr_t{}};
        vk::raii::PhysicalDevice physical_device{std::nullptr_t{}};
        vk::raii::Device device{std::nullptr_t{}};
        vk::raii::Queue queue{std::nullptr_t{}};
        vk::SurfaceFormatKHR swap_chain_format;

        vk::raii::DescriptorSetLayout descriptor_set_layout{std::nullptr_t{}};
        vk::raii::PipelineLayout pipeline_layout{std::nullptr_t{}};
        vk::raii::Pipeline graphics_pipeline{std::nullptr_t{}};

        vulkan::gpu_memory_manager memory_manager{std::nullptr_t{}};
        vulkan::gpu_resource_manager resource_manager{std::nullptr_t{}};

        vk::raii::DescriptorPool descriptor_pool{std::nullptr_t{}};

        vk::raii::CommandPool command_pool{std::nullptr_t{}};

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

        odin5::util::sequential_unordered_map<surface, surface_key_t> surfaces{};
    };

    class graphics_api_spec {
    private:
        vulkan_state vulkan_{};
        odin5::platform::window::active_window_api* window_api_p = nullptr;
        bool first_window = true;

    public:
        graphics_api_spec(odin5::platform::graphics::graphics_create_info gci, odin5::platform::window::active_window_api& window_api);
        surface_key_t create_surface(window::window_key_t wn);
        void destroy_surface(surface_key_t sf);
        void set_viewport(surface_key_t sf, glm::vec4 vp);
        void set_scissor(surface_key_t sf, glm::vec4 sc);
        void set_viewport_and_scissor(surface_key_t sf, glm::vec4 vp);
        void set_view_3d(surface_key_t sf, glm::mat4 view);
        void set_proj_3d(surface_key_t sf, glm::mat4 proj);
        void set_clear_color(surface_key_t sf, glm::vec4 clear);
        void draw(surface_key_t sf, draw_3d_submit_info_constref_t draw_info);
        void framebuffer_resized(surface_key_t sf);
        mesh_idx_t upload_mesh(const std::vector<odin5::math::spatial::vertex>& vertices, const std::vector<uint32_t>& indices);
        ~graphics_api_spec();


    };

    using graphics_api = odin5::platform::graphics::graphics_api<graphics_api_spec>;
}
}
}
}
