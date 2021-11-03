#include "Instance.h"
#include "Debug/Debug.h"

#include <utility>

namespace Graphics {
	namespace Detail {
		InstanceLayer::InstanceLayer(std::string_view name, Version version, bool required)
		    : m_Name(name), m_Version(version), m_Required(required) { }
	} // namespace Detail

	Version Instance::s_CachedVersion;
	Instance::InstanceLayers Instance::s_CachedAvailableLayers;
	Instance::InstanceExtensions Instance::s_CachedAvailableExtensions;

	Version Instance::GetVulkanVersion() {
		if (!s_CachedVersion) {
			if (vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion")) {
				auto result = vkEnumerateInstanceVersion(&s_CachedVersion.m_Version);
				if (result != VK_SUCCESS)
					s_CachedVersion = {};
			} else {
				s_CachedVersion = VK_API_VERSION_1_0;
			}
		}
		return s_CachedVersion;
	}

	const Instance::InstanceLayers& Instance::GetAvailableLayers(bool requery) {
		if (requery || s_CachedAvailableLayers.empty()) {
			s_CachedAvailableLayers.clear();
			std::uint32_t propertyCount;
			vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
			std::vector<VkLayerProperties> properties(propertyCount);
			vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

			s_CachedAvailableLayers.reserve(propertyCount);
			for (std::size_t i = 0; i < propertyCount; ++i) {
				auto& property = properties[i];
				std::string layerName(property.layerName, property.layerName + std::strlen(property.layerName));
				s_CachedAvailableLayers.emplace_back(std::string_view { property.layerName, std::strlen(property.layerName) }, property.implementationVersion);
			}
		}
		return s_CachedAvailableLayers;
	}

	const Instance::InstanceExtensions& Instance::GetAvailableExtensions(bool requery) {
		if (requery || s_CachedAvailableExtensions.empty()) {
			s_CachedAvailableExtensions.clear();
			std::uint32_t propertyCount;
			vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
			std::vector<VkExtensionProperties> properties(propertyCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data());

			s_CachedAvailableExtensions.reserve(propertyCount);
			for (std::size_t i = 0; i < propertyCount; ++i) {
				auto& property = properties[i];
				s_CachedAvailableExtensions.emplace_back(std::string_view { property.extensionName, std::strlen(property.extensionName) }, property.specVersion);
			}
		}
		return s_CachedAvailableLayers;
	}

	bool Instance::HasLayer(std::string_view name, Version lowestVersion) {
		for (auto& layer : s_CachedAvailableLayers)
			if (layer.m_Name == name && layer.m_Version > lowestVersion)
				return true;
		return false;
	}

	bool Instance::HasExtension(std::string_view name, Version lowestVersion) {
		for (auto& extension : s_CachedAvailableExtensions)
			if (extension.m_Name == name && extension.m_Version > lowestVersion)
				return true;
		return false;
	}

	Instance::Instance(std::string_view appName, Version appVersion, std::string_view engineName, Version engineVersion, Version minAPIVersion, Version maxAPIVersion)
	    : m_AppName(appName), m_AppVersion(appVersion), m_EngineName(engineName), m_EngineVersion(engineVersion), m_MinAPIVersion(minAPIVersion), m_MaxAPIVersion(maxAPIVersion) { }

	Instance::~Instance() {
		if (isValid())
			destroy();
	}
	
	void Instance::setDebug(Debug& debug) {
		m_Debug = &debug;
	}

	void Instance::requestLayer(std::string_view name, Version requiredVersion, bool required) {
		auto itr = std::find_if(m_Layers.begin(), m_Layers.end(), [name](const Detail::InstanceLayer& layer) -> bool {
			return layer.m_Name == name;
		});

		if (itr != m_Layers.end()) {
			auto& layer = *itr;
			if (requiredVersion < layer.m_Version)
				layer.m_Version = requiredVersion;

			if (required)
				layer.m_Required = true;
		} else {
			m_Layers.emplace_back(name, requiredVersion, required);
		}
	}

	void Instance::requestExtension(std::string_view name, Version requiredVersion, bool required) {
		auto itr = std::find_if(m_Extensions.begin(), m_Extensions.end(), [name](const Detail::InstanceExtension& extension) -> bool {
			return extension.m_Name == name;
		});

		if (itr != m_Extensions.end()) {
			auto& extension = *itr;
			if (requiredVersion < extension.m_Version)
				extension.m_Version = requiredVersion;

			if (required)
				extension.m_Required = true;
		} else {
			m_Extensions.emplace_back(name, requiredVersion, required);
		}
	}

	Version Instance::getLayerVersion(std::string_view name) const {
		for (auto& layer : m_EnabledLayers)
			if (layer.m_Name == name)
				return layer.m_Version;
		return {};
	}

	Version Instance::getExtensionVersion(std::string_view name) const {
		for (auto& extension : m_EnabledExtensions)
			if (extension.m_Name == name)
				return extension.m_Version;
		return {};
	}

	void Instance::createImpl() {
		Version vulkanVersion = GetVulkanVersion();
		if (vulkanVersion < m_MinAPIVersion)
			return;

		Version instanceVersion;
		if (vulkanVersion >= m_MaxAPIVersion)
			instanceVersion = m_MaxAPIVersion;
		else if (vulkanVersion >= m_MinAPIVersion)
			instanceVersion = vulkanVersion;

		m_MissingLayers.clear();
		m_MissingExtensions.clear();
		GetAvailableLayers();
		for (auto& layer : m_Layers) {
			bool found = false;
			for (auto& availLayer : s_CachedAvailableLayers) {
				if (availLayer.m_Name == layer.m_Name && availLayer.m_Version < layer.m_Version) {
					m_EnabledLayers.push_back(availLayer);
					found = true;
					break;
				}
			}
			if (found) continue;
			if (layer.m_Required)
				m_MissingLayers.push_back(layer);
		}

		GetAvailableExtensions();
		for (auto& extension : m_Extensions) {
			bool found = false;
			for (auto& availExtension : s_CachedAvailableExtensions) {
				if (availExtension.m_Name == extension.m_Name && availExtension.m_Version < extension.m_Version) {
					m_EnabledExtensions.push_back(availExtension);
					found = true;
					break;
				}
			}
			if (found) continue;
			if (extension.m_Required)
				m_MissingExtensions.push_back(extension);
		}

		if (Debug::IsEnabled()) {
			bool found = false;
			for (auto& availLayer : s_CachedAvailableLayers) {
				if (availLayer.m_Name == "VK_LAYER_KHRONOS_validation") {
					m_EnabledLayers.push_back(availLayer);
					found = true;
					break;
				}
			}
			if (!found) {
				Debug::Disable();
			} else {
				found = false;
				for (auto& availExtension : s_CachedAvailableExtensions) {
					if (availExtension.m_Name == "VK_EXT_debug_utils") {
						m_EnabledExtensions.push_back(availExtension);
						found = true;
						break;
					}
				}
				if (!found)
					Debug::Disable();
			}
		}

		if (!m_MissingLayers.empty() || !m_MissingExtensions.empty())
			return;

		std::vector<const char*> useLayers(m_EnabledLayers.size());
		std::vector<const char*> useExtensions(m_EnabledExtensions.size());

		for (std::size_t i = 0; i < useLayers.size(); ++i) {
			auto& layer = m_EnabledLayers[i];
			char* buf   = new char[layer.m_Name.size() + 1] { 0 };
			std::memcpy(buf, layer.m_Name.data(), layer.m_Name.size());
			useLayers[i] = buf;
		}

		for (std::size_t i = 0; i < useExtensions.size(); ++i) {
			auto& extension = m_EnabledExtensions[i];
			char* buf       = new char[extension.m_Name.size() + 1] { 0 };
			std::memcpy(buf, extension.m_Name.data(), extension.m_Name.size());
			useExtensions[i] = buf;
		}

		VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, m_AppName.c_str(), m_AppVersion, m_EngineName.c_str(), m_EngineVersion, instanceVersion };

		VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &appInfo, static_cast<std::uint32_t>(useLayers.size()), useLayers.data(), static_cast<std::uint32_t>(useExtensions.size()), useExtensions.data() };

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

		if (Debug::IsEnabled()) {
			Debug::PopulateCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}

		auto result  = vkCreateInstance(&createInfo, nullptr, &m_Handle);
		if (result == VK_SUCCESS)
			m_ApiVersion = instanceVersion;
	}

	bool Instance::destroyImpl() {
		vkDestroyInstance(m_Handle, nullptr);
		m_EnabledLayers.clear();
		m_EnabledExtensions.clear();
		m_ApiVersion = {};
	}
} // namespace Graphics
