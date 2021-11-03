#include "UI.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace UI {
	App::App() : m_Window(nullptr) { }

	App::~App() { }

	void App::run() {
		createWindow();
		m_Renderer.init(m_Window);

		while (!shouldClose()) {
			updateWindow();
			m_Renderer.render();
		}

		m_Renderer.deinit();
		destroyWindow();
	}

	void App::createWindow() {
		if (m_Window)
			return;

		if (!glfwInit()) {
			std::cerr << "GLFW failed initialize\n";
			return;
		}

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO(MarcasRealAccount): Enable once Swapchain is recreatable.

		m_Window = glfwCreateWindow(1280, 720, "PasswordManager", nullptr, nullptr);
	}

	void App::destroyWindow() {
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
		glfwTerminate();
	}

	bool App::shouldClose() const {
		return glfwWindowShouldClose(m_Window);
	}

	void App::updateWindow() {
		glfwPollEvents();
	}
} // namespace UI