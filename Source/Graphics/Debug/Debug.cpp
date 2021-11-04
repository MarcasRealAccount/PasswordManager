#include "Debug.h"
#include "Graphics/Instance.h"

#include <iostream>
#include <sstream>

namespace Graphics {
	bool Debug::s_Enabled = Core::s_IsConfigDebug;

	void Debug::Disable() {
		s_Enabled = false;
	}

	void Debug::PopulateCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo.messageSeverity = /*vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | */ vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		createInfo.messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
		createInfo.pfnUserCallback = &Debug::DebugCallback;
	}

	std::string Debug::GetSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
		switch (messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "Verbose";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "Info";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "Warning";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "Error";
		default: return "Unknown";
		}
	}

	std::string Debug::GetTypes(VkDebugUtilsMessageTypeFlagsEXT messageTypes) {
		std::string str;
		if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
			str += "General";
			messageTypes &= ~VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
		}
		if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
			if (!str.empty())
				str += " | ";
			str += "Validation";
			messageTypes &= ~VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		}
		if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
			if (!str.empty())
				str += " | ";
			str += "Performance";
			messageTypes &= ~VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		}
		if (messageTypes != 0) {
			if (str.empty())
				str = std::to_string(messageTypes);
			else
				str += "(" + str + ") + " + std::to_string(messageTypes);
		}
		return str;
	}

	VkBool32 Debug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::ostringstream str;

		str << "Vulkan " << GetSeverity(messageSeverity) << " (" << GetTypes(messageTypes) << "): " << pCallbackData->pMessage << "\n";

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			std::cerr << str.str();
		else
			std::cout << str.str();

		return VK_FALSE;
	}

	Debug::Debug(Instance& instance)
	    : m_DebugUtilsEXT({}), m_Instance(instance) {
		m_Instance.addChild(*this);
	}

	Debug::~Debug() {
		if (isValid())
			destroy();
		m_Instance.removeChild(*this);
	}

	void Debug::createImpl() {
		vk::DebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateCreateInfo(createInfo);
		m_DebugUtilsEXT.init(m_Instance);

		VkDebugUtilsMessengerCreateInfoEXT vkCreateInfo = createInfo;

		VkDebugUtilsMessengerEXT debugUtilsMesenger;
		auto result = m_DebugUtilsEXT.vkCreateDebugUtilsMessengerEXT(*m_Instance, &vkCreateInfo, nullptr, &debugUtilsMesenger);
		if (result == VK_SUCCESS)
			m_Handle = debugUtilsMesenger;
	}

	bool Debug::destroyImpl() {
		m_DebugUtilsEXT.vkDestroyDebugUtilsMessengerEXT(*m_Instance, m_Handle, nullptr);
		return true;
	}
} // namespace Graphics
