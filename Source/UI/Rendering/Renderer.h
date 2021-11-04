#pragma once

#include "Core.h"

#include "Graphics/Debug/Debug.h"
#include "Graphics/Device/Device.h"
#include "Graphics/Device/Surface.h"
#include "Graphics/Instance.h"

struct GLFWwindow;

namespace UI {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void init(GLFWwindow* m_Window);
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
	};
} // namespace UI