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
		void Render();
		void Shutdown();
		void AddWindow(std::shared_ptr<DebugWindowBase> window, bool open = false);

	private:
		void drawMainMenuBar(std::string& popupId);
	private:
		inline static const std::string Popup1Id{"Popup1"};
		
		std::vector<std::shared_ptr<DebugWindowBase>> m_windows;
	};

}
