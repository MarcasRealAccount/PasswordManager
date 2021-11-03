#pragma once

#include "Core.h"

#include "Rendering/Renderer.h"

struct GLFWwindow;

namespace UI {
	class App {
	public:
		App();
		~App();

		void run();

	private:
		void createWindow();
		void destroyWindow();
		bool shouldClose() const;
		void updateWindow();

	private:
		GLFWwindow* m_Window;
		Renderer m_Renderer;
	};
} // namespace UI