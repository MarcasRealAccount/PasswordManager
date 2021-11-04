#pragma once

#include "Graphics/Common.h"

namespace Graphics {
	struct Device;
	struct Queue;

	struct QueueFamily : public Handle<void*, false, false> {
	public:
		QueueFamily(Device& device, std::uint32_t familyIndex, VkQueueFlags queueFlags, std::uint32_t timestampValidBits, VkExtent3D minImageTransferGranularity, bool supportsPresent, std::uint32_t queueCount);
		~QueueFamily();

		auto getQueueFlags() const { return m_QueueFlags; }
		auto getTimestampValidBits() const { return m_TimestampValidBits; }
		auto& getImageTransferGranularity() const { return m_MinImageTransferGranularity; }
		auto isPresentSupported() const { return m_SupportsPresent; }
		auto getFamilyIndex() const { return m_FamilyIndex; }

		Queue* getQueue(std::uint32_t index) const;
		auto& getQueues() const { return m_Queues; }

		auto& getDevice() { return m_Device; }
		auto& getDevice() const { return m_Device; }

	private:
		Device& m_Device;

		VkQueueFlags m_QueueFlags;
		std::uint32_t m_TimestampValidBits;
		VkExtent3D m_MinImageTransferGranularity;
		bool m_SupportsPresent;

		std::uint32_t m_FamilyIndex;
		std::vector<Queue> m_Queues;
	};

	struct Queue : public Handle<VkQueue, false, false> {
	public:
		Queue(QueueFamily& queueFamily, std::uint32_t index, VkQueue handle);
		~Queue();

		auto getIndex() const { return m_Index; }

		auto& getQueueFamily() { return m_QueueFamily; }
		auto& getQueueFamily() const { return m_QueueFamily; }

	private:
		QueueFamily& m_QueueFamily;

		std::uint32_t m_Index;
	};
} // namespace Graphics