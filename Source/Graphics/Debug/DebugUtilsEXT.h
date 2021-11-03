#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {
	struct Instance;

	namespace DebugUtilsEXT {
		template <class R, class... Ts>
		using FunctionPtrT = R (*)(Ts...);

		struct FunctionPtrs {
		public:
			VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, VkAllocationCallbacks* allocationCallbacks, VkDebugUtilsMessengerEXT* messenger) { return m_CreateDebugUtilsMessengerEXT(instance, createInfo, allocationCallbacks, messenger); }
			void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks* allocationCallbacks) { m_DestroyDebugUtilsMessengerEXT(instance, messenger, allocationCallbacks); }
			VkResult vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* nameInfo) { return m_SetDebugUtilsObjectNameEXT(device, nameInfo); }

			void init(Instance& instance);

		private:
			FunctionPtrT<VkResult, VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*> m_CreateDebugUtilsMessengerEXT;
			FunctionPtrT<void, VkInstance, VkDebugUtilsMessengerEXT, VkAllocationCallbacks*> m_DestroyDebugUtilsMessengerEXT;
			FunctionPtrT<VkResult, VkDevice, const VkDebugUtilsObjectNameInfoEXT*> m_SetDebugUtilsObjectNameEXT;
		};
	} // namespace DebugUtilsEXT
} // namespace Graphics
