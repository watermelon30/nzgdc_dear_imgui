#include "App.h"
#include "imgui.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "Quad.h"
namespace nzgdc_demo
{
	const static int windowWidth = 800; 
	const static int windowHeight = 600;

	App::App() :
		m_Window{ nullptr }
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW\n";
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(windowWidth, windowHeight, "NZGDC_24", NULL, NULL);
		if (!m_Window)
		{
			std::cerr << "Failed to create GLFW window\n";
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD\n";
		}
#ifdef _DEBUG
		m_debugSystem = std::make_shared<DebugSystem>();
		m_debugSystem->Initialize(m_Window);
#endif
	}

	App::~App()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		

#ifdef _DEBUG
		m_debugSystem->Shutdown();
#endif
	}

	void App::Run()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		Shader transformShader("res/shaders/transform.vs", "res/shaders/basic.frag");
		const std::shared_ptr<Quad> quad = std::make_shared<Quad>(transformShader);
		m_debugSystem->AddWindow(std::make_shared<QuadEditor>(quad));
		
		while (!glfwWindowShouldClose(m_Window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();

			quad->Render();

#ifdef _DEBUG
			m_debugSystem->Render();
#endif


			glfwSwapBuffers(m_Window);
		}

		glBindVertexArray(0);
	}
}
