#include "PaintWindow.h"

nzgdc_demo::PaintWindow::PaintWindow()
{
}

void nzgdc_demo::PaintWindow::RenderContent()
{
	if (ImGui::TreeNode("Select Color:"))
	{
		ImGui::SetNextItemWidth(200.0f);
		ImGui::ColorPicker3("Color picker 3", (float*)&m_currentColor, ImGuiColorEditFlags_NoAlpha);
		ImGui::TreePop();
	}

	if (ImGui::Button("Draw Line"))
	{
		m_currentShape = ShapeType::Line;
	}
	ImGui::SameLine();
	if (ImGui::Button("Draw Circle"))
	{
		m_currentShape = ShapeType::Circle;
	}
	ImGui::SameLine();
	ImGui::Text("Current shape: %s", m_currentShape == ShapeType::Line ? "Line" : "Circle");

	ImGui::Text("Canvas:");
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		m_drawData.clear();
	}

	const ImVec2 drawingCursorScreenPos = ImGui::GetCursorScreenPos();
	const ImVec2 canvasAvailableRegion = ImGui::GetContentRegionAvail();
	const ImVec2 canvasBottomLeft = ImVec2(drawingCursorScreenPos.x + canvasAvailableRegion.x, drawingCursorScreenPos.y + canvasAvailableRegion.y);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(drawingCursorScreenPos, canvasBottomLeft, IM_COL32(50, 50, 50, 255));  // Fill background with gray

	ImGui::InvisibleButton("canvas", canvasAvailableRegion);

	const ImVec2 mouseScreenPos = ImGui::GetMousePos();
	const ImVec2 mouseWorldPosInCanvas = ImVec2(mouseScreenPos.x - drawingCursorScreenPos.x, mouseScreenPos.y - drawingCursorScreenPos.y);

	if (ImGui::IsItemHovered())
	{
		if (!m_drawing && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_dragStart = mouseWorldPosInCanvas;
			m_drawing = true;
		}

		if (m_drawing) {
			if (m_currentShape == ShapeType::Line)
			{
				draw_list->AddLine(ImVec2(drawingCursorScreenPos.x + m_dragStart.x, drawingCursorScreenPos.y + m_dragStart.y),
					ImVec2(drawingCursorScreenPos.x + mouseWorldPosInCanvas.x, drawingCursorScreenPos.y + mouseWorldPosInCanvas.y),
					m_currentColor);
			}
			else if (m_currentShape == ShapeType::Circle)
			{
				const ImVec2 center = ImVec2((m_dragStart.x + mouseWorldPosInCanvas.x) * 0.5f, (m_dragStart.y + mouseWorldPosInCanvas.y) * 0.5f);
				const float radius = CalculateDistance(m_dragStart, mouseWorldPosInCanvas) * 0.5f;  // Calculate the radius based on distance
				draw_list->AddCircle(ImVec2(drawingCursorScreenPos.x + center.x, drawingCursorScreenPos.y + center.y), radius, m_currentColor);
			}
		}

		if (m_drawing&& ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			ShapeData shapeData;
			shapeData.Type = m_currentShape;
			shapeData.Start = m_dragStart;
			shapeData.End = mouseWorldPosInCanvas;
			shapeData.Color = m_currentColor;

			m_drawData.push_back(shapeData);
			m_drawing = false;
		}
	}

	for (const auto& drawData : m_drawData)
	{
		if (drawData.Type == ShapeType::Line)
		{
			draw_list->AddLine(ImVec2(drawingCursorScreenPos.x + drawData.Start.x, drawingCursorScreenPos.y + drawData.Start.y),
				ImVec2(drawingCursorScreenPos.x + drawData.End.x, drawingCursorScreenPos.y + drawData.End.y),
				drawData.Color);
		}
		else if (drawData.Type == ShapeType::Circle)
		{
			const ImVec2 center = ImVec2((drawData.Start.x + drawData.End.x) * 0.5f, (drawData.Start.y + drawData.End.y) * 0.5f);
			const float radius = CalculateDistance(drawData.Start, drawData.End) * 0.5f;  // Calculate the radius based on distance
			draw_list->AddCircle(ImVec2(drawingCursorScreenPos.x + center.x, drawingCursorScreenPos.y + center.y), radius, drawData.Color);
		}
	}
}

std::string nzgdc_demo::PaintWindow::GetWindowId() const
{
	return "Paint Window";
}

float nzgdc_demo::PaintWindow::CalculateDistance(const ImVec2& a, const ImVec2& b)
{
	return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
