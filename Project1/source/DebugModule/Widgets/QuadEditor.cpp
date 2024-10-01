#include "QuadEditor.h"

#include <fstream>

#include "imgui.h"

#include <Quad.h>

#include <json/reader.h>
#include <json/writer.h>

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
	bool edited{false};
	if (ImGui::DragFloat2("Position", m_quadData.Position))
	{
		edited = true;
	}
	if (ImGui::DragFloat("Rotation", &m_quadData.Rotation))
	{
		edited = true;
	}
	if (ImGui::DragFloat2("Scale", m_quadData.Scale))
	{
		edited = true;
	}
	if (edited)
	{
		UpdateQuadSettings();
	}
}

std::string nzgdc_demo::QuadEditor::GetWindowId() const
{
	return "Quad Editor";
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
				SaveToJson();
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
	m_loadedJson.clear();
	m_loadedJson["scale_x"] = m_quadData.Scale[0];
	m_loadedJson["scale_y"] = m_quadData.Scale[1];

	m_loadedJson["rotation"] = m_quadData.Rotation;

	m_loadedJson["position_x"] = m_quadData.Position[0];
	m_loadedJson["position_y"] = m_quadData.Position[1];

	std::ofstream file(Quad::settingsPath);

	if (!file.is_open())
	{
		// TODO: Log error
		return false;
	}
	Json::StreamWriterBuilder writerBuilder;
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	writer->write(m_loadedJson, &file);
	return true;
}

bool nzgdc_demo::QuadEditor::LoadFromJson()
{
	if (m_quad->LoadJson(m_loadedJson))
	{
		m_quad->ParseJson(m_loadedJson, m_quadData);
		m_quad->SetTransformData(m_quadData);
		return true;
	}
	return false;
}
