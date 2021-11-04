#pragma once

#include "Graphics/Common.h"
#include "Graphics/Debug/Debug.h"

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
			DeviceQueueFamilyRequest(std::uint32_t count, vk::QueueFlags queueFlags, bool supportsPresent = false, bool required = true);

		public:
			std::uint32_t m_Count;
			vk::QueueFlags m_QueueFlags;

			bool m_SupportsPresent;
			bool m_Required;
		};
	} // namespace Detail

	struct Surface;
	struct Instance;

	struct Device : public Handle<vk::Device, true, false> {
	public:
		using Layers              = std::vector<Detail::DeviceLayer>;
		using Extensions          = std::vector<Detail::DeviceExtension>;
		using QueueFamilyRequests = std::vector<Detail::DeviceQueueFamilyRequest>;

	public:
		Device(Surface& surface);
		~Device();

		void requestLayer(std::string_view name, Version requiredVersion = {}, bool required = true);
		void requestExtension(std::string_view name, Version requiredVersion = {}, bool required = true);

		void requestQueueFamily(std::uint32_t count, vk::QueueFlags queueFlags, bool supportsPresent = false, bool required = true);

		Version getLayerVersion(std::string_view name) const;
		Version getExtensionVersion(std::string_view name) const;

		QueueFamily* getQueueFamily(vk::QueueFlags queueFlags, bool supportsPresent = false) const;
		auto& getQueueFamilies() const { return m_QueueFamilies; }

		bool isLayerEnabled(std::string_view name) const { return getLayerVersion(name); }
		bool isExtensionEnabled(std::string_view name) const { return getExtensionVersion(name); }

		auto& getEnabledLayers() const { return m_EnabledLayers; }
		auto& getEnabledExtensions() const { return m_EnabledExtensions; }

		Instance& getInstance();
		Instance& getInstance() const;
		Debug& getDebug();
		Debug& getDebug() const;
		auto& getSurface() { return m_Surface; }
		auto& getSurface() const { return m_Surface; }
		auto& getPhysicalDevice() const { return m_PhysicalDevice; }

		template <class Handle, std::enable_if_t<std::is_base_of_v<HandleBase, Handle>, bool> = true>
		void setDebugName(Handle& handle, std::string_view name) {
			if constexpr (Handle::Debuggable) {
				if (Debug::IsEnabled()) {
					using HandleType               = typename Handle::HandleT;
					using BaseHandleT              = typename HandleType::CType;
					HandleType& baseHandle         = handle;
					BaseHandleT evenMoreBaseHandle = baseHandle;

					auto& debug         = getDebug();
					auto& debugUtilsEXT = debug.getDebugUtilsEXT();
					if (name.empty()) {
						vk::DebugUtilsObjectNameInfoEXT nameInfo = { baseHandle.objectType, reinterpret_cast<std::uint64_t>(evenMoreBaseHandle), nullptr };

						VkDebugUtilsObjectNameInfoEXT vkNameInfo = nameInfo;
						debugUtilsEXT.vkSetDebugUtilsObjectNameEXT(m_Handle, &vkNameInfo);
						handle.setDebugName(nullptr);
					} else {
						std::string tName(name);

						vk::DebugUtilsObjectNameInfoEXT nameInfo = { baseHandle.objectType, reinterpret_cast<std::uint64_t>(evenMoreBaseHandle), tName.c_str() };

						VkDebugUtilsObjectNameInfoEXT vkNameInfo = nameInfo;
						getDebug().getDebugUtilsEXT().vkSetDebugUtilsObjectNameEXT(m_Handle, &vkNameInfo);
						handle.setDebugName(name);
					}
				}
			}
		}

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

		vk::PhysicalDevice m_PhysicalDevice = nullptr;
	};
} // namespace Graphics
