#include "DebugSystem.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "App.h"
#include "glfw3.h"

void nzgdc_demo::DebugSystem::Initialize(App* app)
{
	m_app = app;
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(app->GetMainWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void nzgdc_demo::DebugSystem::Render()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	std::string popupId;
	drawMainMenuBar(popupId);
	if (!popupId.empty())
	{
		ImGui::OpenPopup(popupId.c_str()); // Open pop up outside of the menu bar
	}
	drawPopups();
	for (const auto& window : m_windows)
	{
		if (window->isWindowOpen())
		{
			window->Render();
		}
	}
	if (m_imguiDemoOpen)
	{
		ImGui::ShowDemoWindow(&m_imguiDemoOpen);
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

void nzgdc_demo::DebugSystem::AddWindow(std::shared_ptr<DebugWindowBase> window, bool open)
{
	window->SetWindowEnable(open);
	m_windows.push_back(window);
}

void nzgdc_demo::DebugSystem::drawMainMenuBar(std::string& popupId)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Open Window"))
		{
			for (const auto& window : m_windows)
			{
				if (ImGui::MenuItem(window->GetWindowId().c_str()))
				{
					window->SetWindowEnable(true);
				}

				// Demo: just to show you can do it this way too
				bool isWindowOpen = window->isWindowOpen();
				auto selectableId = window->GetWindowId() + " (selectable)";
				// Note: ImGuiSelectableFlags_DontClosePopups works only if ImGuiWindowFlags_NoFocusOnAppearing is set on the opening window.
				if (ImGui::Selectable(selectableId.c_str(), &isWindowOpen, ImGuiSelectableFlags_DontClosePopups))
				{
					window->SetWindowEnable(!window->isWindowOpen());
				}
			}

			ImGui::Selectable("ImGui Demo Window (ocornut)", &m_imguiDemoOpen, ImGuiSelectableFlags_DontClosePopups);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Open pop up"))
		{
			if (ImGui::MenuItem("Popup1 (wrong)"))
			{
				ImGui::OpenPopup(PopupExample.data()); // does not work
			}
			if (ImGui::MenuItem("Popup1 (correct)"))
			{
				popupId = PopupExample;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Open Editor"))
		{
			if (ImGui::MenuItem("Fluid Simulator (WIP)"))
			{
				m_app->CreateFluidSimulatorWindow();
			}
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void nzgdc_demo::DebugSystem::drawPopups()
{
	if (ImGui::BeginPopup(PopupExample.data()))
	{
		ImGui::TextUnformatted("This is a pop up");
		ImGui::EndPopup();
	}
}
