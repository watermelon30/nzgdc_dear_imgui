#include "LightShow.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glfw3.h>

namespace nzgdc_demo
{
	LightShow::LightShow(const LightShowData& _data)
	{
		
	}

	void LightShow::Render()
	{
        RenderImGui();
	}

	void LightShow::Activate()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}

    void LightShow::RenderImGui()
    {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        for (size_t i = 0; i < 5; ++i)
        {
            ImGui::Text("Point Light %d", static_cast<int>(i));
        }

        if (ImGui::Button("Add Point Light"))
        {
        }

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			// restore previous context
			glfwMakeContextCurrent(currentContext);
		}
    }
}