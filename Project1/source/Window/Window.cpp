#include "Window.h"

#include <cassert>
#include <glfw3.h>
#include <iostream>
#include <glad/glad.h>

namespace nzgdc_demo
{
	Window::Window(int windowWidth, int windowHeight, const std::string& windowTitle) : m_windowWidth(windowWidth), m_windowHeight(windowHeight),
		m_windowTitle(windowTitle)
	{
	}

	Window::~Window()
	{
		Destroy();
	}

	void Window::Init(GLFWwindow* share)
	{
		if (m_window) {
			std::cout << "Already Window::Init\n";
			return;
		}

		glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), NULL, share);
		if (!m_window)
		{
			std::cerr << "Failed to create GLFW window\n";
			return;
		}
        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowCloseCallback(m_window, Window::WindowCloseCallback);

		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			glViewport(0.0f, 0.0f, width, height);
		});
	}

	bool Window::IsValid() const {
		return m_window != nullptr;
	}

	void Window::Use() const {
		assert(m_window != nullptr);
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		glfwMakeContextCurrent(m_window);
		glViewport(0, 0, width, height);
	}

	void Window::Present() const {
		assert(m_window != nullptr);
		glfwSwapBuffers(m_window);
	}

	void Window::Show() const {
		if (!m_window) {
			return;
		}
		glfwShowWindow(m_window);
	}

	void Window::Hide() const {
		if (!m_window) {
			return;
		}
		glfwHideWindow(m_window);
	}

	GLFWwindow* Window::GetGLFWwindow() const {
		return m_window;
	}

	void Window::WindowCloseCallback(GLFWwindow* window) {
		Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (windowPtr && windowPtr->OnWindowShouldClose) {
			windowPtr->OnWindowShouldClose(window);
        }
	}

	void Window::Destroy()
	{
		if (!m_window) {
			return;
		}
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}
}