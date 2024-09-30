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
		virtual void RenderContent() = 0;
		[[nodiscard]] virtual std::string GetWindowId() const = 0;

		void Render();
		void SetWindowEnable(bool enabled);
		[[nodiscard]] bool isWindowOpen() const;
	protected:
		bool m_isOpen{ true };
		ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoFocusOnAppearing };
	};
}