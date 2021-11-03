#include "Debug.h"
#include "Graphics/Instance.h"

#include <iostream>

namespace Graphics {
	bool Debug::s_Enabled = Core::s_IsConfigDebug;
	
	void Disable();
	auto IsEnabled() { return s_Enabled; }
	void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	
	static std::string GetSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
	static std::string GetTypes(VkDebugUtilsMessageTypeFlagsEXT messageTypes);
	static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
}
