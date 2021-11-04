#pragma once

#include "Graphics/Common.h"

namespace Graphics {
	struct CommandPool;

	struct CommandBuffer : public Handle<vk::CommandBuffer, false, true> {
	public:
		CommandBuffer(CommandPool& pool, vk::CommandBuffer handle, vk::CommandBufferLevel level);
		~CommandBuffer();

		bool begin();
		void end();

		auto& getPool() { return m_Pool; }
		auto& getPool() const { return m_Pool; }

		auto getLevel() const { return m_Level; }

	private:
		CommandPool& m_Pool;
		vk::CommandBufferLevel m_Level;
	};
} // namespace Graphics
