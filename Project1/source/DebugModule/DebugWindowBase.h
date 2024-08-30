#pragma once
#include <string>

#include "imgui.h"

namespace nzgdc_demo
{
	class DebugWindowBase
	{
	public:
		DebugWindowBase() = default;
		virtual ~DebugWindowBase() = default;
		void Render();

		[[nodiscard]] virtual std::string GetWindowId() const = 0;
		virtual void RenderContent() = 0;

		void SetWindowEnable(bool enabled);
		[[nodiscard]] bool isWindowOpen() const;
	protected:
		bool m_isOpen{true};
		ImGuiWindowFlags m_flags{ImGuiWindowFlags_NoFocusOnAppearing};
	};
}
