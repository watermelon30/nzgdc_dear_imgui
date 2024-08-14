#include "FluidSimulator.h"

#include <glfw3.h>
#include "glad/glad.h"
#include "Window/Window.h"

namespace nzgdc_demo
{
	void FluidSimulator::Show(bool bNewShow) {
		if (m_bShow == bNewShow) {
			return;
		}
		m_bShow = bNewShow;
		if (!m_window->IsValid()) {
			// Lazy init
			m_window->Init(m_share);
		}
		if (m_bShow) {
			m_window->Show();
		} else {
			m_window->Hide();
		}
	}

	FluidSimulator::FluidSimulator() {
		m_window = std::make_shared<Window>(800, 600, "FluidSimulator");
		m_window->OnWindowShouldClose = [](GLFWwindow* window) {
			FluidSimulator::Get().Show(false);
			glfwSetWindowShouldClose(window, false);
		};
	}

	void FluidSimulator::Update(float deltaTime)
	{
		if (m_bShow) {
			Render(deltaTime);
		}
	}

	void FluidSimulator::Render(float deltaTime)
	{
		m_window->Use();

		glClearColor(0.52f, 0.8f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_window->Present();
	}
}
