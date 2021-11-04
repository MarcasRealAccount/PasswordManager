#pragma once

#include "Graphics/Common.h"

namespace Graphics {
	struct QueueFamily;

	namespace Detail {
		struct DeviceLayer {
		public:
			DeviceLayer(std::string_view name, Version version, bool required = true);

		public:
			std::string m_Name;
			Version m_Version;
			bool m_Required;
		};

		using DeviceExtension = DeviceLayer;

		struct DeviceQueueFamilyRequest {
		public:
			DeviceQueueFamilyRequest(std::uint32_t count, VkQueueFlags queueFlags, bool supportsPresent = false, bool required = true);

		public:
			std::uint32_t m_Count;
			VkQueueFlags m_QueueFlags;

			bool m_SupportsPresent;
			bool m_Required;
		};
	} // namespace Detail

	struct Surface;

	struct Device : public Handle<VkDevice, true, false> {
	public:
		using Layers              = std::vector<Detail::DeviceLayer>;
		using Extensions          = std::vector<Detail::DeviceExtension>;
		using QueueFamilyRequests = std::vector<Detail::DeviceQueueFamilyRequest>;

	public:
		Device(Surface& surface);
		~Device();

		void requestLayer(std::string_view name, Version requiredVersion = {}, bool required = true);
		void requestExtension(std::string_view name, Version requiredVersion = {}, bool required = true);

		void requestQueueFamily(std::uint32_t count, VkQueueFlags queueFlags, bool supportsPresent = false, bool required = true);

		Version getLayerVersion(std::string_view name) const;
		Version getExtensionVersion(std::string_view name) const;

		QueueFamily* getQueueFamily(VkQueueFlags queueFlags, bool supportsPresent = false) const;
		auto& getQueueFamilies() const { return m_QueueFamilies; }

		bool isLayerEnabled(std::string_view name) const { return getLayerVersion(name); }
		bool isExtensionEnabled(std::string_view name) const { return getExtensionVersion(name); }

		auto& getEnabledLayers() const { return m_EnabledLayers; }
		auto& getEnabledExtensions() const { return m_EnabledExtensions; }

		auto& getSurface() { return m_Surface; }
		auto& getSurface() const { return m_Surface; }
		auto& getPhysicalDevice() const { return m_PhysicalDevice; }

	private:
		virtual void createImpl() override;
		virtual bool destroyImpl() override;

	public:
		Layers m_Layers;
		Extensions m_Extensions;

		QueueFamilyRequests m_QueueRequests;

	protected:
		Layers m_EnabledLayers;
		Extensions m_EnabledExtensions;

		std::vector<QueueFamily> m_QueueFamilies;

	private:
		Surface& m_Surface;

		VkPhysicalDevice m_PhysicalDevice = nullptr;
	};
} // namespace Graphics