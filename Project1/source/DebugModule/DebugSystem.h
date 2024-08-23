#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DebugWindowBase.h"

namespace nzgdc_demo
{
	class App;

	class DebugSystem
	{
	public:
		void Initialize(App* app);
		void Render();
		void Shutdown();
		void AddWindow(std::shared_ptr<DebugWindowBase> window, bool open = false);

	private:
		void drawMainMenuBar(std::string& popupId);
	private:
		std::vector<std::shared_ptr<DebugWindowBase>> m_windows;
		App* m_app;
	};

}
