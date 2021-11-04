#include "Device.h"
#include "Graphics/Debug/Debug.h"
#include "Graphics/Instance.h"
#include "Queue.h"
#include "Surface.h"

#include <map>

namespace Graphics {
	namespace Detail {
		DeviceLayer::DeviceLayer(std::string_view name, Version version, bool required)
		    : m_Name(name), m_Version(version), m_Required(required) { }

		DeviceQueueFamilyRequest::DeviceQueueFamilyRequest(std::uint32_t count, VkQueueFlags queueFlags, bool supportsPresent, bool required)
		    : m_Count(count), m_QueueFlags(queueFlags), m_SupportsPresent(supportsPresent), m_Required(required) { }
	} // namespace Detail

	Device::Device(Surface& surface)
	    : m_Surface(surface) {
		m_Surface.addChild(*this);
	}

	Device::~Device() {
		if (isValid())
			destroy();
		m_Surface.removeChild(*this);
	}

	void Device::requestLayer(std::string_view name, Version requiredVersion, bool required) {
		auto itr = std::find_if(m_Layers.begin(), m_Layers.end(), [name](const Detail::DeviceLayer& layer) -> bool {
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

	void Device::requestExtension(std::string_view name, Version requiredVersion, bool required) {
		auto itr = std::find_if(m_Extensions.begin(), m_Extensions.end(), [name](const Detail::DeviceExtension& extension) -> bool {
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

	void Device::requestQueueFamily(std::uint32_t count, VkQueueFlags queueFlags, bool supportsPresent, bool required) {
		m_QueueRequests.emplace_back(count, queueFlags, supportsPresent, required);
	}

	Version Device::getLayerVersion(std::string_view name) const {
		for (auto& layer : m_EnabledLayers)
			if (layer.m_Name == name)
				return layer.m_Version;
		return {};
	}

	Version Device::getExtensionVersion(std::string_view name) const {
		for (auto& extension : m_EnabledExtensions)
			if (extension.m_Name == name)
				return extension.m_Version;
		return {};
	}

	QueueFamily* Device::getQueueFamily(VkQueueFlags queueFlags, bool supportsPresent) const {
		for (auto& queueFamily : m_QueueFamilies)
			if ((queueFamily.getQueueFlags() & queueFlags) && (queueFamily.isPresentSupported() >= supportsPresent))
				return const_cast<QueueFamily*>(&queueFamily);
		return nullptr;
	}

	void Device::createImpl() {
		auto& instance = m_Surface.getInstance();
		std::uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
		physicalDevices.resize(physicalDeviceCount);

		VkPhysicalDevice bestPhysicalDevice = nullptr;
		std::size_t bestScore               = 0;

		for (auto& physicalDevice : physicalDevices) {
			std::size_t score = 1;

			bool missingLayers     = false;
			bool missingExtensions = false;
			bool missingQueue      = false;

			std::uint32_t layerCount;
			vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, availableLayers.data());
			availableLayers.resize(layerCount);
			for (auto& layer : m_Layers) {
				bool found = false;
				for (std::size_t i = 0; i < layerCount; ++i) {
					auto& availLayer = availableLayers[i];

					std::string_view layerName = { availLayer.layerName, std::strlen(availLayer.layerName) };

					if (layerName == layer.m_Name && availLayer.implementationVersion >= layer.m_Version) {
						if (!layer.m_Required)
							++score;
						found = true;
						break;
					}
				}
				if (found)
					continue;
				if (layer.m_Required) {
					missingLayers = true;
					break;
				}
			}
			if (missingLayers)
				continue;

			std::uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
			for (auto& extension : m_Extensions) {
				bool found = false;
				for (std::size_t i = 0; i < extensionCount; ++i) {
					auto& availExtension = availableExtensions[i];

					std::string_view extensionName = { availExtension.extensionName, std::strlen(availExtension.extensionName) };

					if (extensionName == extension.m_Name && availExtension.specVersion >= extension.m_Version) {
						if (!extension.m_Required)
							++score;
						found = true;
						break;
					}
				}
				if (found)
					continue;
				if (extension.m_Required) {
					missingExtensions = true;
					break;
				}
			}
			if (missingExtensions)
				continue;

			std::uint32_t queueFamilyCount;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
			for (auto& queueRequest : m_QueueRequests) {
				bool found = false;
				for (std::uint32_t i = 0; i < queueFamilyCount; ++i) {
					auto& queueFamilyProperty = queueFamilyProperties[i];

					bool eval = (queueFamilyProperty.queueFlags & queueRequest.m_QueueFlags) && queueFamilyProperty.queueCount >= queueRequest.m_Count;
					if (queueRequest.m_SupportsPresent) {
						VkBool32 supported;
						vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supported);
						eval = eval && supported;
					}

					if (eval) {
						if (!queueRequest.m_Required)
							++score;
						found = true;
						break;
					}
				}
				if (found)
					continue;
				if (queueRequest.m_Required) {
					missingQueue = true;
					break;
				}
			}
			if (missingQueue)
				continue;

			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(physicalDevice, &features);

			switch (properties.deviceType) {
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				break;
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				score *= 2;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				score *= 3;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				score *= 4;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				score *= 5;
				break;
			default:
				break;
			}

			if (score > bestScore) {
				bestScore          = score;
				bestPhysicalDevice = physicalDevice;
			}
		}

		if (!bestPhysicalDevice)
			return;

		m_PhysicalDevice = bestPhysicalDevice;

		std::uint32_t layerCount;
		vkEnumerateDeviceLayerProperties(m_PhysicalDevice, &layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateDeviceLayerProperties(m_PhysicalDevice, &layerCount, availableLayers.data());
		availableLayers.resize(layerCount);
		for (auto& layer : m_Layers) {
			for (std::size_t i = 0; i < layerCount; ++i) {
				auto& availLayer = availableLayers[i];

				std::string_view layerName = { availLayer.layerName, std::strlen(availLayer.layerName) };

				if (layerName == layer.m_Name && availLayer.implementationVersion >= layer.m_Version) {
					m_EnabledLayers.emplace_back(layerName, availLayer.implementationVersion);
					break;
				}
			}
		}

		std::uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, availableExtensions.data());
		for (auto& extension : m_Extensions) {
			for (std::size_t i = 0; i < extensionCount; ++i) {
				auto& availExtension = availableExtensions[i];

				std::string_view extensionName = { availExtension.extensionName, std::strlen(availExtension.extensionName) };

				if (extensionName == extension.m_Name && availExtension.specVersion >= extension.m_Version) {
					m_EnabledExtensions.emplace_back(extensionName, availExtension.specVersion);
					break;
				}
			}
		}

		if (Debug::IsEnabled()) {
			for (auto& availLayer : availableLayers) {
				std::string_view layerName = { availLayer.layerName, std::strlen(availLayer.layerName) };

				if (layerName == "VK_LAYER_KHRONOS_validation") {
					m_EnabledLayers.emplace_back(layerName, availLayer.implementationVersion);
				}
			}
		}

		std::map<std::uint32_t, std::uint32_t> uniqueQueueFamilyIndices;

		std::uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());
		for (auto& queueRequest : m_QueueRequests) {
			for (std::uint32_t i = 0; i < queueFamilyCount; ++i) {
				auto& queueFamilyProperty = queueFamilyProperties[i];

				bool eval = (queueFamilyProperty.queueFlags & queueRequest.m_QueueFlags) && queueFamilyProperty.queueCount >= queueRequest.m_Count;
				if (queueRequest.m_SupportsPresent) {
					VkBool32 supported;
					vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface, &supported);
					eval = eval && supported;
				}

				if (eval) {
					auto itr = uniqueQueueFamilyIndices.find(i);
					if (itr == uniqueQueueFamilyIndices.end())
						uniqueQueueFamilyIndices.insert({ i, queueRequest.m_Count });
					else if (queueRequest.m_Count > itr->second)
						itr->second = queueRequest.m_Count;
					break;
				}
			}
		}

		std::vector<std::vector<float>> queueProperties(uniqueQueueFamilyIndices.size());
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilyIndices.size());

		{
			std::size_t i = 0;
			for (auto itr = uniqueQueueFamilyIndices.begin(); itr != uniqueQueueFamilyIndices.end(); ++itr, ++i) {
				auto [familyIndex, queueCount] = *itr;

				auto& queueProperty = queueProperties[i];
				queueProperty.resize(queueCount);
				for (std::size_t j = 0; j < queueCount; ++j)
					queueProperty[j] = 1.0f;

				auto& queueCreateInfo = queueCreateInfos[i];
				queueCreateInfo       = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, familyIndex, queueCount, queueProperty.data() };
			}
		}

		VkPhysicalDeviceFeatures enabledFeatures;

		std::vector<const char*> useLayers(m_EnabledLayers.size());
		std::vector<const char*> useExtensions(m_EnabledExtensions.size());

		for (std::size_t i = 0; i < useLayers.size(); ++i) {
			auto& layer  = m_EnabledLayers[i];
			useLayers[i] = layer.m_Name.c_str();
		}

		for (std::size_t i = 0; i < useExtensions.size(); ++i) {
			auto& extension  = m_EnabledExtensions[i];
			useExtensions[i] = extension.m_Name.c_str();
		}

		VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0, static_cast<std::uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(), static_cast<std::uint32_t>(useLayers.size()), useLayers.data(), static_cast<std::uint32_t>(useExtensions.size()), useExtensions.data(), &enabledFeatures };

		auto result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Handle);
		if (result == VK_SUCCESS) {
			m_QueueFamilies.reserve(uniqueQueueFamilyIndices.size());
			for (auto itr = uniqueQueueFamilyIndices.begin(); itr != uniqueQueueFamilyIndices.end(); ++itr) {
				auto [familyIndex, queueCount] = *itr;

				auto& queueFamilyProperty = queueFamilyProperties[familyIndex];

				VkBool32 presentSupport;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, familyIndex, m_Surface, &presentSupport);

				m_QueueFamilies.emplace_back(*this, familyIndex, queueFamilyProperty.queueFlags, queueFamilyProperty.timestampValidBits, queueFamilyProperty.minImageTransferGranularity, presentSupport, queueCount);
			}
		}
	}

	bool Device::destroyImpl() {
		vkDestroyDevice(m_Handle, nullptr);
		m_EnabledLayers.clear();
		m_EnabledExtensions.clear();
		m_QueueFamilies.clear();
		return true;
	}
} // namespace Graphics