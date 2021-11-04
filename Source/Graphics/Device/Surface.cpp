#include "Surface.h"
#include "Graphics/Instance.h"

#include <GLFW/glfw3.h>

namespace Graphics {
	Surface::Surface(Instance& instance)
	    : m_Instance(instance), m_Window(nullptr) {
		m_Instance.addChild(*this);
	}

	Surface::~Surface() {
		if (isValid())
			destroy();
		m_Instance.removeChild(*this);
	}

	void Surface::setWindow(GLFWwindow* window) {
		m_Window = window;
	}

	void Surface::createImpl() {
		auto result = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Handle);
		if (result != VK_SUCCESS)
			return;
	}

	bool Surface::destroyImpl() {
		vkDestroySurfaceKHR(m_Instance, m_Handle, nullptr);
		return true;
	}
} // namespace Graphics