#include "CommandBuffer.h"
#include "CommandPool.h"

namespace Graphics {
	CommandBuffer::CommandBuffer(CommandPool& pool, vk::CommandBuffer handle, vk::CommandBufferLevel level)
	    : Handle(handle), m_Pool(pool), m_Level(level) {
		m_Pool.addChild(*this);
	}

	CommandBuffer::~CommandBuffer() {
		if (isValid())
			destroy();
		m_Pool.removeChild(*this);
	}

	bool CommandBuffer::begin() {
		vk::CommandBufferBeginInfo beginInfo = { {}, nullptr };
		return m_Handle.begin(&beginInfo) == vk::Result::eSuccess;
	}

	void CommandBuffer::end() {
		m_Handle.end();
	}
} // namespace Graphics
