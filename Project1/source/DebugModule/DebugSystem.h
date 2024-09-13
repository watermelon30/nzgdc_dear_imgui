#pragma once

#include <glfw3.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>
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
		void SubscribeToOpenQuadScene(const std::function<void()>& func) { m_OpenQuadFunction = func; }
		void SubscribeToOpenLighting(const std::function<void()>& func) { m_OpenLightingFunction = func; }

	private:
		void drawMainMenuBar(std::string& popupId);
	private:
		std::vector<std::shared_ptr<DebugWindowBase>> m_windows;
		std::function<void()> m_OpenLightingFunction;
		std::function<void()> m_OpenQuadFunction;
	};
}
