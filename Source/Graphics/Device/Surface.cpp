#include "Surface.h"
#include "Graphics/Instance.h"

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
		VkSurfaceKHR surface;
		auto result = glfwCreateWindowSurface(*m_Instance, m_Window, nullptr, &surface);
		if (result == VK_SUCCESS)
			m_Handle = surface;
	}

	bool Surface::destroyImpl() {
		m_Instance->destroySurfaceKHR(m_Handle, nullptr);
		return true;
	}
} // namespace Graphics
