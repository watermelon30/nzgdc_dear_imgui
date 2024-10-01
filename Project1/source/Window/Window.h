#pragma once
#include <functional>
#include <string>
#include <glfw3.h>

namespace nzgdc_demo {
	class Window {
	public:
		Window(int windowWidth = 800, int windowHeight = 600, const std::string& windowTitle = "NZGDC_24");
		virtual ~Window();

		virtual void Init(GLFWwindow* share = NULL);
		void Destroy();

		void Use() const;
		void Present() const;

		void Show() const;
		void Hide() const;

		bool IsValid() const;
		GLFWwindow* GetGLFWwindow() const;

		std::function<void(GLFWwindow*)> OnWindowShouldClose;
	
		int m_windowWidth;
		int m_windowHeight;
		std::string m_windowTitle;
	
	private:
	    static void WindowCloseCallback(GLFWwindow* window);
		GLFWwindow* m_window = nullptr;
	};
}
