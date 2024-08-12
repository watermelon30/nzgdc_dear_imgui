#pragma once
#include <glfw3.h>
#include <memory>
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
		void AddWindow(std::shared_ptr<DebugWindowBase> window);
	private:
		std::vector<std::shared_ptr<DebugWindowBase>> windows;
	};

}
