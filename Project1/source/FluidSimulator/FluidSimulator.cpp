#include "FluidSimulator.h"

#include <glfw3.h>
#include "glad/glad.h"
#include "Window/Window.h"

namespace nzgdc_demo
{
	void FluidSimulator::Show(bool bNewShow)
	{
		if (m_bShow == bNewShow)
		{
			return;
		}
		m_bShow = bNewShow;
		if (!m_window->IsValid())
		{
			// Lazy init
			m_window->Init(m_share);
			Init();
		}
		if (m_bShow)
		{
			m_window->Show();
		}
		else
		{
			m_window->Hide();
		}
	}

	FluidSimulator::FluidSimulator()
	{
		m_window = std::make_shared<Window>(800, 600, "FluidSimulator");
		m_window->OnWindowShouldClose = [](GLFWwindow* window)
		{
			Get().Show(false);
			glfwSetWindowShouldClose(window, false);
		};
	}

	void FluidSimulator::Update(float deltaTime)
	{
		if (m_bShow)
		{
			Render(deltaTime);
		}
	}

	void FluidSimulator::Render(float deltaTime)
	{
		m_window->Use();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(200.0f);
		
		m_shader->Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
		
		m_window->Present();
	}

	void FluidSimulator::Init()
	{
		m_window->Use();
		m_shader = std::make_shared<Shader>("res/shaders/fluid.vs", "res/shaders/fluid.frag");
		
		float vertices[] = {
			 0.0f,  0.0f
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
