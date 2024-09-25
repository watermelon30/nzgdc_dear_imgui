#include "App.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glfw3.h>
#include <iostream>

#include "Camera.h"
#include "Quad.h"
#include "QuadMVP.h"
#include "FluidSimulator/FluidSimulator.h"

namespace nzgdc_demo
{
	constexpr static int windowWidth{ 800 }; 
	constexpr static int windowHeight{ 600 };

	App::App() :
		m_Window{ nullptr }
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW\n";
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(windowWidth, windowHeight, "NZGDC_24", nullptr, nullptr);
		if (!m_Window)
		{
			std::cerr << "Failed to create GLFW window\n";
			glfwTerminate();
		}
		
		glfwMakeContextCurrent(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			});

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD\n";
			return;
		}

		FluidSimulator::Get().m_share = m_Window;
	}

	App::~App()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void App::Run()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		const CameraData cameraData(ProjectionType::Orthographic, windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 3.0f));
		m_camera = std::make_shared<Camera>(cameraData);

		const Shader defaultShader("res/shaders/basic.vs", "res/shaders/basic.frag");

		m_quad = std::make_shared<Quad>(defaultShader);
		m_quadMVP = std::make_shared<QuadMVP>(defaultShader, "res/textures/jack.jpg");
		m_quadMVP->GetTransform().m_scale = glm::vec3(400.0f);

		glfwSwapInterval(1);
		while (!glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();

			m_currentFrame = static_cast<float>(glfwGetTime());
			const float deltaTime{ m_currentFrame - m_lastFrame };
			m_lastFrame = m_currentFrame;

			Update(deltaTime);
			Render(deltaTime);

			HandleFluidSimulator(deltaTime);
		}

		glBindVertexArray(0);
	}

	void App::Update(float deltaTime)
	{		
		m_quadMVP->SetView(m_camera->GetView());
		m_quadMVP->SetProjection(m_camera->GetProjection());
	}

	void App::Render(float deltaTime)
	{
		glfwMakeContextCurrent(m_Window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// m_quad->Render();
		m_quadMVP->Render();
		glfwSwapBuffers(m_Window);
	}

	void App::HandleFluidSimulator(float deltaTime) {
		FluidSimulator::Get().Update(deltaTime);
	}
}
