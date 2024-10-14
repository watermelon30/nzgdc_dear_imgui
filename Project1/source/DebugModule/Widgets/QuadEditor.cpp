#include "QuadEditor.h"

#include <fstream>

#include "imgui.h"

#include <Quad.h>

#include <json/reader.h>
#include <json/writer.h>

#include "JsonHelper.h"
#include "QuadMVP.h"

nzgdc_demo::QuadEditor::QuadEditor(const std::shared_ptr<Quad>& quad)
	: m_quad(quad), m_loadedJson(Json::nullValue)
{
	m_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
	m_quadData = m_quad->GetTransformData();
}

void nzgdc_demo::QuadEditor::UpdateQuadSettings() {
	m_quad->SetTransformData(m_quadData);
}

void nzgdc_demo::QuadEditor::RenderContent()
{
	std::string popupId;
	DrawMenuBar(popupId);
	if (DrawQuadEditor(m_quadData))
	{
		UpdateQuadSettings();
	}
}

std::string nzgdc_demo::QuadEditor::GetWindowId() const
{
	return "Quad Editor";
}

bool nzgdc_demo::QuadEditor::DrawQuadEditor(QuadData& quadData)
{
	bool edited{false};
	if (ImGui::DragFloat2("Position", quadData.Position))
	{
		edited =  true;
	}
	if (ImGui::DragFloat("Rotation", &quadData.Rotation))
	{
		edited = true;
	}
	if (ImGui::DragFloat2("Scale", quadData.Scale))
	{
		edited = true;
	}
	return edited;
}

Json::Value nzgdc_demo::QuadEditor::Serialize(const QuadData& data)
{
	Json::Value outJson;

	outJson["scale_x"] = data.Scale[0];
	outJson["scale_y"] = data.Scale[1];

	outJson["rotation"] = data.Rotation;

	outJson["position_x"] = data.Position[0];
	outJson["position_y"] = data.Position[1];
	return outJson;
}

void nzgdc_demo::QuadEditor::DrawMenuBar(std::string popupId)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Load"))
			{
				if (LoadFromJson())
				{
					UpdateQuadSettings();
				}
			}
			if (ImGui::MenuItem("Save"))
			{
				JsonHelper::SaveToJson(Quad::settingsPath, m_loadedJson);
			}
			if (ImGui::MenuItem("Reset coordinates"))
			{
				ResetCoordinates();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void nzgdc_demo::QuadEditor::ResetCoordinates()
{
	m_quadData.Position[0] = 0.0f;
	m_quadData.Position[1] = 0.0f;
	m_quadData.Rotation = 0.0f;
	UpdateQuadSettings();
}

bool nzgdc_demo::QuadEditor::SaveToJson()
{
	m_loadedJson = Serialize(m_quadData);
	return JsonHelper::SaveToJson(Quad::settingsPath, m_loadedJson);
}

bool nzgdc_demo::QuadEditor::LoadFromJson()
{
	if (JsonHelper::LoadJson(Quad::settingsPath, m_loadedJson))
	{
		Quad::ParseJson(m_loadedJson, m_quadData);
		m_quad->SetTransformData(m_quadData);
		return true;
	}
	return false;
}
