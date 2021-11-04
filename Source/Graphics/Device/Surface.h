#pragma once

#include "Graphics/Common.h"

namespace Graphics {
	struct Instance;

	struct Surface : public Handle<vk::SurfaceKHR, true, false> {
	public:
		Surface(Instance& instance);
		~Surface();

		void setWindow(GLFWwindow* window);
		auto getWindow() const { return m_Window; }

		auto& getInstance() { return m_Instance; }
		auto& getInstance() const { return m_Instance; }

	private:
		virtual void createImpl() override;
		virtual bool destroyImpl() override;

	private:
		Instance& m_Instance;
		GLFWwindow* m_Window;
	};
} // namespace Graphics
