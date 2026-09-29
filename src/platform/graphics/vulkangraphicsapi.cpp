#include <cstring>
#define GLFW_INCLUDE_VULKAN 1
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include <GLFW/glfw3.h>
#include "platform/graphics/vulkangraphicsapi.hpp"
#include "platform/graphics/igraphicsapi.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>
#include <iostream>

static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
                                                      vk::DebugUtilsMessageTypeFlagsEXT              type,
                                                      const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
                                                      void *                                         pUserData)
{
  std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

  return vk::False;
}

odin5::platform::graphics::vulkan::graphics_api_spec::graphics_api_spec(odin5::platform::graphics::graphics_create_info gci, odin5::platform::window::active_window_api& window_api) {
    vk::ApplicationInfo app_info{
        .pApplicationName = gci.application_name.c_str(),
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = gci.engine_name.c_str(),
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    std::vector<const char*> required_layers;
    if constexpr (odin5::platform::graphics::vulkan::vulkan_state::using_validation_layers) {
        required_layers.assign(odin5::platform::graphics::vulkan::vulkan_state::validation_layers.begin(), odin5::platform::graphics::vulkan::vulkan_state::validation_layers.end());
    }
    auto layer_props = vulkan_.context.enumerateInstanceLayerProperties();
	for (const auto& layer : required_layers) {
	    bool found = false;
	    for (const auto& prop : layer_props) {
			int32_t result = strcmp(prop.layerName, layer);
			if (result == 0) {
			    found = true;
				break;
			}
		}
		odin5::util::error_if(!found, "unsupported layer");
	}

    if constexpr (odin5::enm::active_window_api == odin5::enm::window_api_identifiers::glfw) {
        uint32_t extension_count = 0;
        auto extensions_p = glfwGetRequiredInstanceExtensions(&extension_count);
        std::vector<const char*> extensions(extensions_p, extensions_p + extension_count);

        auto extension_props = vulkan_.context.enumerateInstanceExtensionProperties();
        for (const auto& extension : extensions) {
            bool found = false;
            for (const auto& prop : extension_props) {
                int32_t result = strcmp(prop.extensionName, extension);
                if (result == 0) {
                    found = true;
                    break;
                }
            }
            odin5::util::error_if(!found, "unsupported extension");
        }
        vk::InstanceCreateInfo create_info{
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
            .ppEnabledLayerNames = required_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };
        vulkan_.instance = vulkan_.context.createInstance(create_info);
    }
}

odin5::platform::graphics::vulkan::graphics_api_spec::~graphics_api_spec() {

}
