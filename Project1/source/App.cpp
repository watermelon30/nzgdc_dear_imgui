#include "App.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glfw3.h>
#include <iostream>

#include "Camera.h"
#include "ParticleSystem.h"
#include "Quad.h"
#include "QuadMVP.h"
#include "DebugModule/Widgets/QuadEditor.h"
#include "Widgets/CameraEditor.h"
#include "Widgets/ParticleSystemEditor.h"
#include "Widgets/ImguiDemoWindow.h"

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

        const CameraData cameraData(ProjectionType::Orthographic, windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 3.0f));
        m_camera = std::make_shared<Camera>(cameraData);

        Shader particleShader("res/shaders/particle.vs", "res/shaders/particle.frag");
        m_quadMVP = std::make_shared<QuadMVP>(particleShader);
        m_quadMVP->GetTransform().m_scale = glm::vec3(100.0f);

        Shader transformShader("res/shaders/transform.vs", "res/shaders/basic.frag");
        m_quad = std::make_shared<Quad>(transformShader);

        ParticleSystemData particleSystemData;
        particleSystemData.AngularVelocity = 100.0f;
        m_particleSystem = std::make_shared<ParticleSystem>(particleSystemData, particleShader, m_camera);
        m_particleSystem->Play();

        //const Shader defaultShader("res/shaders/basic.vs", "res/shaders/basic.frag");
        // m_quad = std::make_shared<Quad>(defaultShader);
        // m_quadMVP = std::make_shared<QuadMVP>(defaultShader, "res/textures/jack.jpg");
        // m_quadMVP->GetTransform().m_scale = glm::vec3(400.0f);

#ifdef _DEBUG
        // m_debugSystem->AddWindow(std::make_shared<QuadEditor>(m_quad));
        m_debugSystem->AddWindow(std::make_shared<QuadEditor>(m_quadMVP), true);
        m_debugSystem->AddWindow(std::make_shared<CameraEditor>(m_camera), true);
        m_debugSystem->AddWindow(std::make_shared<ParticleSystemEditor>(m_particleSystem), true);
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
		m_quadMVP->SetView(m_camera->GetView());
		m_quadMVP->SetProjection(m_camera->GetProjection());
    	m_particleSystem->Update(deltaTime);
	}

    void App::Render(float deltaTime)
    {
        glfwMakeContextCurrent(m_Window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // m_quad->Render();
        // m_quadMVP->Render();
        m_particleSystem->Render();

#ifdef _DEBUG
        m_debugSystem->Render();
#endif

		glfwSwapBuffers(m_Window);
	}

}
