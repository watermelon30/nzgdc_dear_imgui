#include "DebugWindowBase.h"

void nzgdc_demo::DebugWindowBase::Render() {
	ImGui::Begin(GetWindowId().c_str(), &m_isOpen, m_flags);
	if (!m_isOpen) {
		ImGui::End();
		return;
	}
	RenderContent();
	ImGui::End();
}

void nzgdc_demo::DebugWindowBase::SetWindowEnable(bool enabled) {
	m_isOpen = enabled;
}

bool nzgdc_demo::DebugWindowBase::isWindowOpen() const
{
	return m_isOpen;
}
