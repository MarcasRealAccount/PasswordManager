#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {
	struct Instance;
	
	namespace DebugUtilsEXT {
		template <class R, class... Ts>
		using FunctionPtrT = R(*)(Ts...);
		
		struct FunctionPtrs {
		public:
			FunctionPtrT<VkResult, VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*> vkCreateDebugUtilsMessengerEXT;
			FunctionPtrT<void, VkInstance, VkDebugUtilsMessengerEXT, VkAllocationCallbacks*> vkDestroyDebugUtilsMessengerEXT;
			FunctionPtrT<VkResult, VkDevice, const VkDebugUtilsObjectNameInfoEXT*> vkSetDebugUtilsObjectNameEXT;
			
			void init(Instance& instance);
		};
	}
}
