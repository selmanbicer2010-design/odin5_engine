#include "core/file.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#define GLFW_INCLUDE_VULKAN 1
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS 1
#include "vulkan/vulkan.hpp"
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include "platform/graphics/vulkangraphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#undef max

namespace {
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
                                                          vk::DebugUtilsMessageTypeFlagsEXT              type,
                                                          const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
                                                          void *                                         pUserData)
    {
      std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;
      odin5::util::silence_compiler_unused(severity, pUserData);
      return vk::False;
    }

    using vulkan_state_ref_t = odin5::platform::graphics::vulkan::vulkan_state&;
    using graphics_dynamic_info_ref_t = odin5::platform::graphics::graphics_dynamic_info&;

    template <typename func>
    bool check_vk_validity_of(const std::vector<const char*>& arr0, const auto& arr1, func&& check) {
        bool any_not_found = false;
        for (const auto& a : arr0) {
            bool found = false;
            for (const auto& b : arr1) {
                int32_t result = check(b, a);
                if (result == 0) {
                    found = true;
                    break;
                }
            }
            odin5::util::error_if(!found, "unsupported extension");
            if (!found) {
                any_not_found = true;
            }
        }
        return !any_not_found;
    }

    void add_platform_specific_extensions(std::vector<const char*>& extensions) {
        if constexpr (odin5::enm::active_window_api == odin5::enm::window_api_identifiers::glfw) {
            uint32_t glfw_extension_count = 0;
            auto glfw_extensions_p = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
            std::vector<const char*> glfw_extensions(glfw_extensions_p, glfw_extensions_p + glfw_extension_count);
            extensions.insert(extensions.end(), glfw_extensions.begin(), glfw_extensions.end());
        }
    }

    std::vector<const char*> create_vk_layers(vulkan_state_ref_t vk_state) {
        std::vector<const char*> layers;
        if constexpr (odin5::platform::graphics::vulkan::vulkan_state::using_validation_layers) {
            layers.insert(layers.end(), odin5::platform::graphics::vulkan::vulkan_state::validation_layers.begin(), odin5::platform::graphics::vulkan::vulkan_state::validation_layers.end());
        }
        auto layer_props = vk_state.context.enumerateInstanceLayerProperties();
        check_vk_validity_of(layers, layer_props, [](const auto& a, const char* b){ return strcmp(a.layerName, b) == 0; });
	    return layers;
    }

    std::vector<const char*> create_vk_extensions(vulkan_state_ref_t vk_state) {
        std::vector<const char*> extensions;
        if constexpr (odin5::platform::graphics::vulkan::vulkan_state::using_validation_layers) {
            extensions.push_back(vk::EXTDebugUtilsExtensionName);
        }
	    add_platform_specific_extensions(extensions);
        auto extension_props = vk_state.context.enumerateInstanceExtensionProperties();
        check_vk_validity_of(extensions, extension_props, [](const auto& a, const char* b){ return strcmp(a.extensionName, b) == 0; });
        return extensions;
    }

    void create_instance(vulkan_state_ref_t vk_state, odin5::platform::graphics::graphics_create_info gci) {
        vk::ApplicationInfo app_info{
            .pApplicationName = gci.application_name.c_str(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = gci.engine_name.c_str(),
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };

        std::vector<const char*> layers = create_vk_layers(vk_state);
        std::vector<const char*> extensions = create_vk_extensions(vk_state);

        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        vk_state.instance = vk_state.context.createInstance(create_info);
    }

    void create_debug_msgr(vulkan_state_ref_t vk_state) {
        if constexpr (odin5::platform::graphics::vulkan::vulkan_state::using_validation_layers) {
            vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
            vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
            vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
                .messageSeverity = severityFlags,
                .messageType = messageTypeFlags,
                .pfnUserCallback = &debugCallback
            };
            vk_state.debug_msgr = vk_state.instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
        }
    }

    bool physical_device_supports_queues(vk::PhysicalDevice physical_device) {
        auto queue_families = physical_device.getQueueFamilyProperties();
        bool supports_graphics = false;
        for (const auto& family : queue_families) {
            if ((family.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0))
            {
                supports_graphics = true;
                break;
            }
        }
        return (supports_graphics);
    }

    bool physical_device_supports_extensions(vk::PhysicalDevice physical_device) {
        std::vector<const char*> extensions = {
            vk::KHRSwapchainExtensionName
        };
        auto device_props = physical_device.enumerateDeviceExtensionProperties();
        bool supported = check_vk_validity_of(extensions, device_props, [](const auto& a, const char* b){ return strcmp(a.extensionName, b) == 0; });
        return supported;
    }

    bool physical_device_supports_features(vk::PhysicalDevice physical_device) {
        auto features = physical_device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        bool supports_required_features = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
                                          features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                                          features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
        return supports_required_features;
    }

    bool physical_device_supports_vk13(vk::PhysicalDevice physical_device) {
        return physical_device.getProperties().apiVersion >= vk::ApiVersion13;
    }

    bool physical_device_is_suitable(vk::PhysicalDevice physical_device) {
        return physical_device_supports_queues(physical_device) and physical_device_supports_extensions(physical_device) and physical_device_supports_features(physical_device) and physical_device_supports_vk13(physical_device);
    }

    int32_t rate_physical_device(vk::PhysicalDevice physical_device) {
        odin5::util::silence_compiler_unused(physical_device, []{ rate_physical_device({}); });
        return 9001; //ITS OVER 9000!!!
    }

    void pick_physical_device(vulkan_state_ref_t vk_state) {
        std::vector<vk::raii::PhysicalDevice> physical_devices = vk_state.instance.enumeratePhysicalDevices();
        if (physical_devices.empty()) {
            odin5::util::throw_except<std::runtime_error>("no gpus detected by vulkan");
        }
        uint32_t selection_idx = 0;
        for (auto& physical_device : physical_devices)
        {
            bool suitable = physical_device_is_suitable(physical_device);
            if (suitable) {
                break;
            }
            selection_idx++;
        }

        if (selection_idx == physical_devices.size()) {
            odin5::util::throw_except<std::runtime_error>("no suitable gpu detected by vulkan");
        }

        vk_state.physical_device = std::move(physical_devices[selection_idx]);
    }

    void create_surface(vulkan_state_ref_t vk_state, odin5::platform::window::active_window_api& window_api) {
        if constexpr (odin5::enm::active_window_api == odin5::enm::window_api_identifiers::glfw) {
            VkSurfaceKHR c_surface; // Youre gonna make me touch the C api for this?!
            if (glfwCreateWindowSurface(*vk_state.instance, window_api.get_glfw_native_window(), nullptr, &c_surface) != VK_SUCCESS) {
                odin5::util::throw_except<std::runtime_error>("failed to create window surface for vulkan");
            }
            vk_state.surface = vk::raii::SurfaceKHR(vk_state.instance, c_surface); // Why does instance not have a method for this!!!
        }
    }

    uint32_t logical_device_graphics_and_present_idx(vulkan_state_ref_t vk_state, const std::vector<vk::QueueFamilyProperties>& queue_family_properties) {
        uint32_t queue_idx = UINT32_MAX;
        for (uint32_t qfp_idx = 0; qfp_idx < queue_family_properties.size(); qfp_idx++)
        {
            if ((queue_family_properties[qfp_idx].queueFlags & vk::QueueFlagBits::eGraphics) && vk_state.physical_device.getSurfaceSupportKHR(qfp_idx, *vk_state.surface)) {
              // found a queue family that supports both graphics and present
                queue_idx = qfp_idx;
                break;
            }
        }
        return queue_idx;
    }

    void create_logical_device(vulkan_state_ref_t vk_state) {
        std::vector<vk::QueueFamilyProperties> queue_family_properties = vk_state.physical_device.getQueueFamilyProperties();

        vk_state.queue_idx = logical_device_graphics_and_present_idx(vk_state, queue_family_properties);
        if (vk_state.queue_idx == UINT32_MAX) {
            odin5::util::throw_except<std::runtime_error>("could not find a queue for both graphics and present");
        }

        vk::PhysicalDeviceFeatures device_features;
        using vk_struct_chain = vk::StructureChain<vk::PhysicalDeviceFeatures2,
                           vk::PhysicalDeviceVulkan11Features,
                           vk::PhysicalDeviceVulkan13Features,
                           vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>;
        vk_struct_chain featureChain = {
            {},                                    // vk::PhysicalDeviceFeatures2 (empty for now)
            {.shaderDrawParameters = true},        // Enable shader draw parameters from Vulkan 1.1
            {.synchronization2 = true, .dynamicRendering = true},            // Enable dynamic rendering from Vulkan 1.3
            {.extendedDynamicState = true}         // Enable extended dynamic state from the extension
        };

        std::vector<const char*> required_device_extensions = {
            vk::KHRSwapchainExtensionName
        };

        float queue_priority = 0.5f;
        vk::DeviceQueueCreateInfo device_queue_create_info {
            .queueFamilyIndex = vk_state.queue_idx,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        };

        vk::DeviceCreateInfo device_create_info {
            .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &device_queue_create_info,
            .enabledExtensionCount = static_cast<uint32_t>(required_device_extensions.size()),
            .ppEnabledExtensionNames = required_device_extensions.data(),
        };

        vk_state.device = vk_state.physical_device.createDevice(device_create_info);
        vk_state.queue = vk::raii::Queue(vk_state.device, vk_state.queue_idx, 0);
    }

    vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats) {
        if (available_formats.empty()) {
            odin5::util::throw_except<std::out_of_range>("empty available_formats for swapchain");
        }
        const auto format_it = std::ranges::find_if(available_formats,
            [](const auto& format) {
                return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
            }
        );
        return format_it != available_formats.end() ? *format_it : available_formats[0];
    }

    vk::PresentModeKHR choose_swap_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes) {
        const auto ret = std::ranges::any_of(available_present_modes,
            [](const auto& present_mode) {
                return vk::PresentModeKHR::eMailbox == present_mode;
            }
        ) ? vk::PresentModeKHR::eMailbox : vk::PresentModeKHR::eFifo;
        return ret;
    }

    vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities, odin5::platform::window::active_window_api& window_api) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        int width, height;
        if constexpr (odin5::enm::active_window_api == odin5::enm::window_api_identifiers::glfw) {
            glfwGetFramebufferSize(window_api.get_glfw_native_window(), &width, &height);
        }

        return {
            std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    uint32_t choose_swap_min_image_count(const vk::SurfaceCapabilitiesKHR& surface_capabilities) {
        auto minImageCount = std::max(3u, surface_capabilities.minImageCount);
        if ((0 < surface_capabilities.maxImageCount) && (surface_capabilities.maxImageCount < minImageCount)) {
            minImageCount = surface_capabilities.maxImageCount;
        }
        return minImageCount;
    }

    template <bool recreate_swapchain = false>
    void create_swapchain(vulkan_state_ref_t vk_state, odin5::platform::window::active_window_api& window_api) {
        vk::SurfaceCapabilitiesKHR surface_capabilities = vk_state.physical_device.getSurfaceCapabilitiesKHR(*vk_state.surface);
        std::vector<vk::SurfaceFormatKHR> available_formats = vk_state.physical_device.getSurfaceFormatsKHR(*vk_state.surface);
        std::vector<vk::PresentModeKHR> available_present_modes = vk_state.physical_device.getSurfacePresentModesKHR(*vk_state.surface);

        vk_state.swap_chain_extent = choose_swap_extent(surface_capabilities, window_api);
        uint32_t min_image_count = choose_swap_min_image_count(surface_capabilities);
        vk_state.swap_chain_format = choose_swap_surface_format(available_formats);
        vk::PresentModeKHR swap_chain_present_mode = choose_swap_present_mode(available_present_modes);

        vk::SwapchainCreateInfoKHR swap_chain_create_info{
            .surface          = *vk_state.surface,
            .minImageCount    = min_image_count,
            .imageFormat      = vk_state.swap_chain_format.format,
            .imageColorSpace  = vk_state.swap_chain_format.colorSpace,
            .imageExtent      = vk_state.swap_chain_extent,
            .imageArrayLayers = 1,
            .imageUsage       = vk::ImageUsageFlagBits::eColorAttachment,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform     = surface_capabilities.currentTransform,
            .compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode      = swap_chain_present_mode,
            .clipped          = true
        };

        if constexpr (recreate_swapchain) {
            vk::raii::SwapchainKHR old_swapchain = std::move(vk_state.swap_chain);
            swap_chain_create_info.oldSwapchain = *old_swapchain;
            vk_state.swap_chain = vk_state.device.createSwapchainKHR(swap_chain_create_info);
        }
        else {
            swap_chain_create_info.oldSwapchain = nullptr;
            vk_state.swap_chain = vk_state.device.createSwapchainKHR(swap_chain_create_info);
        }

        vk_state.swap_chain_images = vk_state.swap_chain.getImages();

    }

    template <bool recreate_swapchain = false>
    void create_image_views(vulkan_state_ref_t vk_state) {
        vk::ImageViewCreateInfo image_view_create_info{
            .viewType = vk::ImageViewType::e2D,
            .format = vk_state.swap_chain_format.format,
            .subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 },
        };

        if constexpr (recreate_swapchain) {
            vk_state.swap_chain_image_views.clear();
        }
        for (auto& image : vk_state.swap_chain_images) {
            image_view_create_info.image = image;
             vk_state.swap_chain_image_views.emplace_back(vk_state.device, image_view_create_info);
        }

    }

    void recreate_swapchain(vulkan_state_ref_t vk_state, odin5::platform::window::active_window_api& window_api) {
        vk_state.device.waitIdle();
        create_swapchain<true>(vk_state, window_api);
        create_image_views<true>(vk_state);
    }

    vk::raii::ShaderModule create_shader_module(vulkan_state_ref_t vk_state, const std::vector<uint8_t>& bytecode) {
        vk::ShaderModuleCreateInfo create_info{
            .codeSize = bytecode.size() * sizeof(uint8_t),
            .pCode = reinterpret_cast<const uint32_t*>(bytecode.data())
        };
        return vk_state.device.createShaderModule(create_info);
    }

    void create_graphics_pipeline(vulkan_state_ref_t vk_state) {
        vk::raii::ShaderModule shader = create_shader_module(vk_state, odin5::file::read("./shaders/main.spv"));
        vk::PipelineShaderStageCreateInfo vert_shader_stage_info {
            .stage = vk::ShaderStageFlagBits::eVertex,
            .module = shader,
            .pName = "vert_main"
        };
        vk::PipelineShaderStageCreateInfo frag_shader_stage_info {
            .stage = vk::ShaderStageFlagBits::eFragment,
            .module = shader,
            .pName = "frag_main"
        };
        vk::PipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

        vk::PipelineVertexInputStateCreateInfo vertex_input_info;

        vk::PipelineInputAssemblyStateCreateInfo input_assembly{
            .topology = vk::PrimitiveTopology::eTriangleList
        };

        vk::Viewport viewport{
            0.0f, 0.0f,
            static_cast<float>(vk_state.swap_chain_extent.width), static_cast<float>(vk_state.swap_chain_extent.height),
            0.0f, 1.0f
        };

        vk::Rect2D scissor{
            vk::Offset2D{ 0, 0 },
            vk_state.swap_chain_extent
        };

        std::vector<vk::DynamicState> dynamic_states {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamic_state {
            .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
            .pDynamicStates = dynamic_states.data()
        };

        vk::PipelineViewportStateCreateInfo viewport_state {
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
        };

        vk::PipelineRasterizationStateCreateInfo rasterizer {
            .depthClampEnable = vk::False,
            .rasterizerDiscardEnable = vk::False,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = vk::CullModeFlagBits::eBack,
            .frontFace = vk::FrontFace::eClockwise,
            .depthBiasEnable = vk::False,
            .lineWidth = 1.0f
        };

        vk::PipelineMultisampleStateCreateInfo multisampling {
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = vk::False
        };

        vk::PipelineColorBlendAttachmentState color_blend_attachment{
            .blendEnable         = vk::True,
            .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp        = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor = vk::BlendFactor::eZero,
            .alphaBlendOp        = vk::BlendOp::eAdd,
            .colorWriteMask      = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo color_blending{
            .logicOpEnable = vk::False,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = 1,
            .pAttachments = &color_blend_attachment
        };

        vk::PipelineLayoutCreateInfo pipeline_layout_info{
            .setLayoutCount = 0,
            .pushConstantRangeCount = 0
        };

        vk_state.pipeline_layout = vk_state.device.createPipelineLayout(pipeline_layout_info);

        vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipeline_create_info_chain {
            {
                .stageCount          = 2,
                .pStages             = shader_stages,
                .pVertexInputState   = &vertex_input_info,
                .pInputAssemblyState = &input_assembly,
                .pViewportState      = &viewport_state,
                .pRasterizationState = &rasterizer,
                .pMultisampleState   = &multisampling,
                .pColorBlendState    = &color_blending,
                .pDynamicState       = &dynamic_state,
                .layout              = vk_state.pipeline_layout,
                .renderPass          = nullptr,
            },
            {
                .colorAttachmentCount = 1,
                .pColorAttachmentFormats = &vk_state.swap_chain_format.format,
            },
        };

        vk_state.graphics_pipeline = vk_state.device.createGraphicsPipeline(std::nullptr_t{}, pipeline_create_info_chain.get<vk::GraphicsPipelineCreateInfo>());
    }

    void create_command_pool(vulkan_state_ref_t vk_state) {
        vk::CommandPoolCreateInfo pool_info {
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = vk_state.queue_idx,
        };
        vk_state.command_pool = vk_state.device.createCommandPool(pool_info);
    }

    void create_command_buffer(vulkan_state_ref_t vk_state) {
        vk::CommandBufferAllocateInfo alloc_info{
            .commandPool = vk_state.command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = 1
        };
        vk_state.command_buffer = std::move(vk::raii::CommandBuffers(vk_state.device, alloc_info).front());
    }

    void create_synchronization_objects(vulkan_state_ref_t vk_state) {
        vk_state.present_complete_semaphore = vk::raii::Semaphore(vk_state.device, vk::SemaphoreCreateInfo());
        for (size_t i = 0; i < vk_state.swap_chain_images.size(); i++) {
            vk_state.render_finished_semaphores.emplace_back(vk::raii::Semaphore(vk_state.device, vk::SemaphoreCreateInfo()));
        }
        vk_state.draw_fence = vk::raii::Fence(vk_state.device, {.flags = vk::FenceCreateFlagBits::eSignaled});
    }

    void transition_image_layout(vulkan_state_ref_t vk_state,
        uint32_t imageIndex,
	    vk::ImageLayout old_layout, vk::ImageLayout new_layout,
	    vk::AccessFlags2 src_access_mask, vk::AccessFlags2 dst_access_mask,
	    vk::PipelineStageFlags2 src_stage_mask, vk::PipelineStageFlags2 dst_stage_mask) {
		vk::ImageMemoryBarrier2 barrier {
		    .srcStageMask = src_stage_mask,
            .srcAccessMask = src_access_mask,
            .dstStageMask = dst_stage_mask,
            .dstAccessMask = dst_access_mask,
            .oldLayout = old_layout,
            .newLayout = new_layout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = vk_state.swap_chain_images[imageIndex],
            .subresourceRange {
                .aspectMask     = vk::ImageAspectFlagBits::eColor,
                .baseMipLevel   = 0,
		        .levelCount     = 1,
		        .baseArrayLayer = 0,
		        .layerCount     = 1
            }
		};
        vk::DependencyInfo dependency_info {
            .dependencyFlags = {},
		    .imageMemoryBarrierCount = 1,
		    .pImageMemoryBarriers = &barrier
        };
        vk_state.command_buffer.pipelineBarrier2(dependency_info);
    }

    void record_command_buffer(vulkan_state_ref_t vk_state, graphics_dynamic_info_ref_t dynamic, uint32_t image_index) {
        vk_state.command_buffer.begin({});

        transition_image_layout(
            vk_state, image_index,
            vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal,
            {}, vk::AccessFlagBits2::eColorAttachmentWrite,
            vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eColorAttachmentOutput
        );

        vk::ClearValue clear_color = vk::ClearColorValue(dynamic.clear_color.x, dynamic.clear_color.y, dynamic.clear_color.z, dynamic.clear_color.w);
        vk::RenderingAttachmentInfo attachment_info {
            .imageView   = vk_state.swap_chain_image_views[image_index],
            .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .loadOp      = vk::AttachmentLoadOp::eClear,
            .storeOp     = vk::AttachmentStoreOp::eStore,
            .clearValue  = clear_color
        };

        vk::RenderingInfo rendering_info {
            .renderArea = {.offset = {0, 0}, .extent = vk_state.swap_chain_extent},
            .layerCount = 1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &attachment_info
        };

        vk_state.command_buffer.beginRendering(rendering_info);

        vk_state.command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *vk_state.graphics_pipeline);

        vk_state.command_buffer.setViewport(0, vk::Viewport(dynamic.viewport.x, dynamic.viewport.y, dynamic.viewport.z, dynamic.viewport.w, 0.0f, 1.0f));
        vk_state.command_buffer.setScissor(0, vk::Rect2D(vk::Offset2D(dynamic.scissor.x, dynamic.scissor.y), vk::Extent2D(dynamic.scissor.z, dynamic.scissor.w)));

        vk_state.command_buffer.draw(3, 1, 0, 0);

        vk_state.command_buffer.endRendering();

        transition_image_layout(
            vk_state, image_index,
            vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR,
            vk::AccessFlagBits2::eColorAttachmentWrite, {},
            vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eBottomOfPipe
        );

        vk_state.command_buffer.end();
    }

    void draw_frame(vulkan_state_ref_t vk_state, graphics_dynamic_info_ref_t dynamic) {
        auto fence_result = vk_state.device.waitForFences(*vk_state.draw_fence, vk::True, UINT64_MAX);
        if (fence_result != vk::Result::eSuccess) {
            odin5::util::throw_except<std::runtime_error>("failed to wait for fence");
        }
        vk_state.device.resetFences(*vk_state.draw_fence);

        auto [result, image_index] = vk_state.swap_chain.acquireNextImage(UINT64_MAX, *vk_state.present_complete_semaphore, nullptr);
        if (result != vk::Result::eSuccess) {
            odin5::util::throw_except<std::runtime_error>("failed to get next image");
        }

        record_command_buffer(vk_state, dynamic, image_index);

        vk::PipelineStageFlags wait_destination_stage_mask{vk::PipelineStageFlagBits::eColorAttachmentOutput};
        const vk::SubmitInfo submit_info {
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = &*vk_state.present_complete_semaphore,
            .pWaitDstStageMask    = &wait_destination_stage_mask,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &*vk_state.command_buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = &*vk_state.render_finished_semaphores[image_index]
        };

        vk_state.queue.submit(submit_info, *vk_state.draw_fence);

        const vk::PresentInfoKHR present_info_KHR {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = &*vk_state.render_finished_semaphores[image_index],
            .swapchainCount     = 1,
            .pSwapchains        = &*vk_state.swap_chain,
            .pImageIndices      = &image_index
        };

        result = vk_state.queue.presentKHR(present_info_KHR);

    }

}

odin5::platform::graphics::vulkan::graphics_api_spec::graphics_api_spec(odin5::platform::graphics::graphics_create_info gci, odin5::platform::window::active_window_api& window_api) {
    window_api_p = &window_api;
    create_instance(vulkan_, gci);
    create_debug_msgr(vulkan_); // runs fully only when in debug
    pick_physical_device(vulkan_);
    create_surface(vulkan_, window_api);
    create_logical_device(vulkan_);
    create_swapchain(vulkan_, window_api);
    create_image_views(vulkan_);
    create_graphics_pipeline(vulkan_);
    create_command_pool(vulkan_);
    create_command_buffer(vulkan_);
    create_synchronization_objects(vulkan_);

}

void odin5::platform::graphics::vulkan::graphics_api_spec::set_viewport(glm::vec4 vp) {
    dynamic_info.viewport = vp;
}

void odin5::platform::graphics::vulkan::graphics_api_spec::set_scissor(glm::vec4 sc) {
    dynamic_info.scissor = sc;
}

void odin5::platform::graphics::vulkan::graphics_api_spec::set_viewport_and_scissor(glm::vec4 vp) {
    dynamic_info.viewport = vp;
    dynamic_info.scissor = vp;
}

void odin5::platform::graphics::vulkan::graphics_api_spec::set_clear_color(glm::vec4 clear) {
    dynamic_info.clear_color = clear;
}

void odin5::platform::graphics::vulkan::graphics_api_spec::draw() {
    draw_frame(vulkan_, dynamic_info);
}

void odin5::platform::graphics::vulkan::graphics_api_spec::framebuffer_resized() {
    recreate_swapchain(vulkan_, *window_api_p);
}

odin5::platform::graphics::vulkan::graphics_api_spec::~graphics_api_spec() {
    vulkan_.device.waitIdle();
}
