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
		void drawMainMenuBar(std::string& popupId); // Pass the popup ID to open out of here if you need to open one

		void drawPopups();

	private:
		static constexpr std::string_view PopupExample{"PopupExample"};

		std::vector<std::shared_ptr<DebugWindowBase>> m_windows;

		bool m_imguiDemoOpen{true};
		App* m_app{nullptr};
	};
}
