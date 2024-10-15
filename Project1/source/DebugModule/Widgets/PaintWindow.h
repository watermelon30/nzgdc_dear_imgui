#pragma once
#include <vector>

#include "DebugWindowBase.h"

namespace nzgdc_demo
{
	enum class ShapeType : unsigned int
	{
		Line,
		Circle
	};

	struct ShapeData {
		ShapeType Type{ShapeType::Line};
		ImVec2 Start;
		ImVec2 End;
		ImColor Color{ImColor(255, 255, 255)};
	};

	class PaintWindow : public nzgdc_demo::DebugWindowBase
	{
	public:
		PaintWindow();
		void RenderContent() override;
		[[nodiscard]] std::string GetWindowId() const override;

	private:
		static float CalculateDistance(const ImVec2& a, const ImVec2& b);

	private:
		std::vector<ShapeData> m_drawData;
		ShapeType m_currentShape {ShapeType::Line};
		ImColor m_currentColor{ImColor(255, 255, 255)};
		bool m_drawing{false};
		ImVec2 m_dragStart;
	};
}
