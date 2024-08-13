#include "DebugSystem.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void nzgdc_demo::DebugSystem::Initialize(GLFWwindow* mainWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init();
}

void nzgdc_demo::DebugSystem::Render()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	for (const auto& window : windows)
	{
		window->Render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Multi viewport
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* currentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// restore previous context
		glfwMakeContextCurrent(currentContext);
	}
}

void nzgdc_demo::DebugSystem::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void nzgdc_demo::DebugSystem::AddWindow(std::shared_ptr<DebugWindowBase> window)
{
	windows.push_back(window);
}
