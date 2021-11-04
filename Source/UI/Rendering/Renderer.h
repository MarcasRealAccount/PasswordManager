#pragma once

#include "Core.h"

#include "Graphics/Command/CommandPool.h"
#include "Graphics/Debug/Debug.h"
#include "Graphics/Device/Device.h"
#include "Graphics/Device/Queue.h"
#include "Graphics/Device/Surface.h"
#include "Graphics/Instance.h"
#include "Graphics/Memory/Vma.h"
#include "Graphics/Sync/Fence.h"
#include "Graphics/Sync/Semaphore.h"

#include <vector>

struct GLFWwindow;

namespace UI {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void init(GLFWwindow* m_Window, std::size_t maxFramesInFlight);
		void deinit();

		void render();

		auto& getInstance() { return m_Instance; }
		auto& getInstance() const { return m_Instance; }
		auto& getDebug() { return m_Debug; }
		auto& getDebug() const { return m_Debug; }

	private:
		bool m_Initialized = false;
		Graphics::Instance m_Instance;
		Graphics::Debug m_Debug;
		Graphics::Surface m_Surface;
		Graphics::Device m_Device;
		Graphics::Queue* m_GraphicsPresentQueue;
		Graphics::Memory::Vma m_Vma;

		std::vector<Graphics::CommandPool> m_CommandPools;
		std::vector<Graphics::Sync::Semaphore> m_ImageAvailableSemaphores;
		std::vector<Graphics::Sync::Semaphore> m_RenderFinishedSemaphores;
		std::vector<Graphics::Sync::Fence> m_InFlightFences;
	};
} // namespace UI
