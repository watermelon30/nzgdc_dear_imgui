#pragma once
#include <glfw3.h>
#include <memory>
#include <string>
#include <vector>
#include "DebugWindowBase.h"
namespace nzgdc_demo
{
	class DebugSystem
	{
	public:
		static void Initialize(GLFWwindow* mainWindow);
		static void Shutdown();

		void InitWindows();
		void Render();
		void AddWindow(std::shared_ptr<DebugWindowBase> window, bool open = false);

	private:
		void DrawMainMenuBar(std::string& openPopupId);
		void DrawPopups(std::string& openPopupId);

		void DrawPopup1();

	private:
		inline static const std::string Popup1Id{ "Popup1" };

		std::vector<std::shared_ptr<DebugWindowBase>> m_windows;
	};
}
