#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace UI {
	Renderer::Renderer()
	    : m_Instance("PasswordManager", { 0, 0, 1, 0 }, "PasswordManager", { 0, 0, 1, 0 }, VK_API_VERSION_1_0, VK_API_VERSION_1_2), m_Debug(m_Instance), m_Surface(m_Instance), m_Device(m_Surface), m_GraphicsPresentQueue(nullptr), m_Vma(m_Device) {
		m_Instance.setDebug(m_Debug);
	}

	Renderer::~Renderer() {
		deinit();
	}

	void Renderer::init(GLFWwindow* window, std::size_t maxFramesInFlight) {
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

		m_Device.requestExtension("VK_KHR_swapchain");
		m_Device.requestExtension("VK_KHR_portability_subset", {}, false);

		m_Device.requestQueueFamily(1, vk::QueueFlagBits::eGraphics, true);

		if (!m_Device.create()) {
			std::cerr << "Failed to create vulkan device!\n";
			return;
		}

		auto graphicsPresentQueueFamily = m_Device.getQueueFamily(vk::QueueFlagBits::eGraphics, true);
		m_GraphicsPresentQueue          = graphicsPresentQueueFamily->getQueue(0);

		if (!m_Vma.create()) {
			std::cerr << "Failed to create vulkan memory allocator!\n";
			return;
		}

		m_CommandPools.reserve(maxFramesInFlight);
		m_InFlightFences.reserve(maxFramesInFlight);
		m_ImageAvailableSemaphores.reserve(maxFramesInFlight);
		m_RenderFinishedSemaphores.reserve(maxFramesInFlight);
		for (std::size_t i = 0; i < maxFramesInFlight; ++i) {
			auto& commandPool = m_CommandPools.emplace_back(m_Device);

			commandPool.setQueueFamily(*graphicsPresentQueueFamily);

			if (!commandPool.create()) {
				std::cerr << "Failed to create vulkan command pool!\n";
				return;
			}
			m_Device.setDebugName(commandPool, "m_CommandPools[" + std::to_string(i) + "]");

			auto& imageAvailableSemaphore = m_ImageAvailableSemaphores.emplace_back(m_Device);
			if (!imageAvailableSemaphore.create()) {
				std::cerr << "Failed to create vulkan semaphore!\n";
				return;
			}
			m_Device.setDebugName(imageAvailableSemaphore, "m_ImageAvailableSemaphores[" + std::to_string(i) + "]");

			auto& renderFinishedSemaphore = m_RenderFinishedSemaphores.emplace_back(m_Device);
			if (!renderFinishedSemaphore.create()) {
				std::cerr << "Failed to create vulkan semaphore!\n";
				return;
			}
			m_Device.setDebugName(renderFinishedSemaphore, "m_RenderFinishedSemaphores[" + std::to_string(i) + "]");

			auto& inFlightFence = m_InFlightFences.emplace_back(m_Device);
			inFlightFence.setSignaled();
			if (!inFlightFence.create()) {
				std::cerr << "Failed to create vulkan fence!\n";
				return;
			}
			m_Device.setDebugName(inFlightFence, "m_CommandPools[" + std::to_string(i) + "]");
		}

		m_Initialized = true;
	}

	void Renderer::deinit() {
		if (!m_Initialized)
			return;

		m_Instance.destroy();
		m_Initialized = false;
	}

	void Renderer::render() {
	}
} // namespace UI
