#pragma once

#include "Common.h"

#include <string>
#include <string_view>

namespace Graphics {
	namespace Detail {
		struct InstanceLayer {
		public:
			InstanceLayer(std::string_view name, Version version, bool required = true);

		public:
			std::string m_Name;
			Version m_Version;
			bool m_Required;
		};

		using InstanceExtension = InstanceLayer;
	} // namespace Detail

	struct Debug;

	struct Instance : public Handle<VkInstance, true, false> {
	public:
		using Layers     = std::vector<Detail::InstanceLayer>;
		using Extensions = std::vector<Detail::InstanceExtension>;

	public:
		static Version GetVulkanVersion();
		static const Layers& GetAvailableLayers(bool requery = false);
		static const Extensions& GetAvailableExtensions(bool requery = false);
		static bool HasLayer(std::string_view name, Version lowestVersion = {});
		static bool HasExtension(std::string_view name, Version lowestVersion = {});

	private:
		static Version s_CachedVersion;
		static Layers s_CachedAvailableLayers;
		static Extensions s_CachedAvailableExtensions;

	public:
		Instance(std::string_view appName, Version appVersion, std::string_view engineName, Version engineVersion, Version minAPIVersion = {}, Version maxAPIVersion = { ~0U });
		~Instance();

		void setDebug(Debug& debug);
		auto& getDebug() { return *m_Debug; }
		auto& getDebug() const { return *m_Debug; }

		void requestLayer(std::string_view name, Version requiredVersion = {}, bool required = true);
		void requestExtension(std::string_view name, Version requiredVersion = {}, bool required = true);

		Version getLayerVersion(std::string_view name) const;
		Version getExtensionVersion(std::string_view name) const;

		bool isLayerEnabled(std::string_view name) const { return getLayerVersion(name); }
		bool isExtensionEnabled(std::string_view name) const { return getExtensionVersion(name); }

		auto getApiVersin() const { return m_ApiVersion; }

		auto& getEnabledLayers() const { return m_EnabledLayers; }
		auto& getEnabledExtensions() const { return m_EnabledExtensions; }

		auto& getMissingLayers() const { return m_MissingLayers; }
		auto& getMissingExtensions() const { return m_MissingExtensions; }

	private:
		virtual void createImpl() override;
		virtual bool destroyImpl() override;

	public:
		std::string m_AppName;
		Version m_AppVersion;
		std::string m_EngineName;
		Version m_EngineVersion;
		Version m_MinAPIVersion;
		Version m_MaxAPIVersion;

		Layers m_Layers;
		Extensions m_Extensions;

	protected:
		Version m_ApiVersion;

		Layers m_EnabledLayers;
		Extensions m_EnabledExtensions;

	private:
		Debug* m_Debug = nullptr;

		Layers m_MissingLayers;
		Extensions m_MissingExtensions;
	};
} // namespace Graphics
