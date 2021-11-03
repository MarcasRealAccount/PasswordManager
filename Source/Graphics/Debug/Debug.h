#pragma once

#include "DebugUtilsEXT.h"
#include "Graphics/Common.h"

#include <string>

namespace Graphics {
	struct Instance;

	struct Debug : public Handle<VkDebugUtilsMessengerEXT, true, false> {
	public:
		static void Disable();
		static auto IsEnabled() { return s_Enabled; }
		static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	private:
		static std::string GetSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
		static std::string GetTypes(VkDebugUtilsMessageTypeFlagsEXT messageTypes);
		static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	private:
		static bool s_Enabled;

	public:
		Debug(Instance& instance);
		~Debug();

		auto& getInstance() { return m_Instance; }
		auto& getInstance() const { return m_Instance; }

		auto& getDebugUtilsEXT() { return m_DebugUtilsEXT; }
		auto& getDebugUtilsEXT() const { return m_DebugUtilsEXT; }

	private:
		virtual void createImpl() override;
		virtual bool destroyImpl() override;

	protected:
		DebugUtilsEXT::FunctionPtrs m_DebugUtilsEXT;

	private:
		Instance& m_Instance;
	};
} // namespace Graphics
