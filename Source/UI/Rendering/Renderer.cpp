#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace UI {
	Renderer::Renderer()
	    : m_Instance("PasswordManager", { 0, 0, 1, 0 }, "PasswordManager", { 0, 0, 1, 0 }, VK_API_VERSION_1_0, VK_API_VERSION_1_2), m_Debug(m_Instance), m_Surface(m_Instance), m_Device(m_Surface) {
		m_Instance.setDebug(m_Debug);
	}

	Renderer::~Renderer() {
		deinit();
	}

	void Renderer::init(GLFWwindow* window) {
		if (m_Initialized)
			return;

		std::uint32_t glfwExtensionCount;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (std::size_t i = 0; i < glfwExtensionCount; ++i)
			m_Instance.requestExtension(glfwExtensions[i]);

		if (!m_Instance.create()) {
			auto& missingLayers     = m_Instance.getMissingLayers();
			auto& missingExtensions = m_Instance.getMissingExtensions();
			if (!missingLayers.empty() || !missingExtensions.empty()) {
				std::ostringstream str;
				if (!missingLayers.empty()) {
					str << "Missing " << missingLayers.size() << (missingExtensions.size() > 1 ? " instance layers:" : " instance layer:");
					for (auto& layer : missingLayers) {
						auto& subversion = layer.m_Version.m_SubVersions;
						str << "\n  " << layer.m_Name << " with version: " << subversion.variant << "." << subversion.major << "." << subversion.minor << "." << subversion.patch;
					}
					str << "\n";
				}
				if (!missingExtensions.empty()) {
					str << "Missing " << missingExtensions.size() << (missingExtensions.size() > 1 ? " instance extensions:" : " instance extension:");
					for (auto& extension : missingExtensions) {
						auto& subversion = extension.m_Version.m_SubVersions;
						str << "\n  " << extension.m_Name << " with version: " << subversion.variant << "." << subversion.major << "." << subversion.minor << "." << subversion.patch;
					}
					str << "\n";
				}
				std::cerr << str.str();
			} else {
				std::cerr << "Failed to create vulkan instance!\n";
			}
			return;
		}

		if (Graphics::Debug::IsEnabled())
			m_Debug.create();

		m_Surface.setWindow(window);

		if (!m_Surface.create()) {
			std::cerr << "Failed to create vulkan surface!\n";
			return;
		}

		if (!m_Device.create()) {
			std::cerr << "Failed to create vulkan device!\n";
			return;
		}
	}

	void Renderer::deinit() {
		if (!m_Initialized)
			return;

		m_Instance.destroy();
	}

	void Renderer::render() {
	}
} // namespace UI