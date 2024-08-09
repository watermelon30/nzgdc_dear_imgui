#include "App.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

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
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init();
	}

	App::~App()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void App::Run()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo{ 0 };
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		Shader basicShader("res/shaders/basic.vs", "res/shaders/basic.frag");
		basicShader.Use();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		while (!glfwWindowShouldClose(m_Window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();

			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(m_Window);
		}

		glBindVertexArray(0);
	}
}
