#include "App.h"
#include "Shader/Shader.h"
#include "glad/glad.h"
#include <glfw3.h>
#include <iostream>

#include "Camera.h"
#include "Quad.h"
#include "QuadMVP.h"
#include "DebugModule/Widgets/QuadEditor.h"
#include "ParticleSystem/ParticleSystem.h"

#include "SceneManager/SceneManager.h"

#include "Widgets/CameraEditor.h"
#include "Widgets/ParticleSystemEditor.h"
#include "Widgets/ImguiDemoWindow.h"
#include "Widgets/SceneManagerEditor.h"

#include "Window/FluidSimulatorWindow.h"

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
#ifdef _DEBUG
		m_debugSystem = std::make_shared<DebugSystem>();
		m_debugSystem->Initialize(this);
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    	m_sceneManager = std::make_shared<SceneManager>();
    	m_sceneManager->Init(windowWidth, windowHeight);

#ifdef _DEBUG
       // m_debugSystem->AddWindow(std::make_shared<QuadEditor>(m_quadMVP), true);
        //m_debugSystem->AddWindow(std::make_shared<CameraEditor>(m_camera), true);
       // m_debugSystem->AddWindow(std::make_shared<ParticleSystemEditor>(m_particleSystem), true);
    	m_debugSystem->AddWindow(std::make_shared<SceneManagerEditor>(m_sceneManager), true);
        m_debugSystem->AddWindow(std::make_shared<ImguiDemoWindow>(), true);
#endif

        glfwSwapInterval(1);
        while (!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();

            m_currentFrame = static_cast<float>(glfwGetTime());
            const float deltaTime{ m_currentFrame - m_lastFrame };
            m_lastFrame = m_currentFrame;

            Update(deltaTime);
            Render(deltaTime);

			for (const auto& window : m_windows)
			{
				window->Update(deltaTime);
				window->Render(deltaTime);
			}
		}

        glBindVertexArray(0);
    }

	std::shared_ptr<Window> App::CreateFluidSimulatorWindow()
	{
		auto window = std::make_shared<FluidSimulatorWindow>();
		window->Init(m_Window);
		window->OnWindowShouldClose = [this, win = std::weak_ptr(window)](GLFWwindow* windowPtr)
		{
			if (auto sharedWindow = win.lock())
			{
				m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), sharedWindow), m_windows.end());
			}
		};
		m_windows.push_back(window);
		return window;
	}

	void App::Update(float deltaTime)
	{
    	m_sceneManager->Update(deltaTime);
	}

    void App::Render(float deltaTime)
    {
        glfwMakeContextCurrent(m_Window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	m_sceneManager->Render();

#ifdef _DEBUG
        m_debugSystem->Render();
#endif

		glfwSwapBuffers(m_Window);
	}

}
