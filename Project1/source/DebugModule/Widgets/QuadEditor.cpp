#include "QuadEditor.h"
#include <fstream>
#include <vec3.hpp>
#include "imgui.h"
#include <Quad.h>
#include <json/reader.h>
#include <json/writer.h>
nzgdc_demo::QuadEditor::QuadEditor(const std::shared_ptr<Quad>& quad)
	: m_quad(quad), m_loadedJson(Json::nullValue)
{
	m_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
	m_position[0] = quad->GetTransform().m_position[0];
	m_position[1] = quad->GetTransform().m_position[1];
	m_rotation = quad->GetTransform().m_rotation[2];
	m_scale[0] = quad->GetTransform().m_scale[0];
	m_scale[1] = quad->GetTransform().m_scale[1];
}
void nzgdc_demo::QuadEditor::UpdateQuadSettings() {
	m_quad->GetTransform().m_position = glm::vec3(m_position[0], m_position[1], 0.0f);
	m_quad->GetTransform().m_rotation = glm::vec3(0.0f, 0.0f, m_rotation);
	m_quad->GetTransform().m_scale = glm::vec3(m_scale[0], m_scale[1], 0.0f);
}
void nzgdc_demo::QuadEditor::RenderContent()
{
	std::string popupId;
	drawMenuBar(popupId);
	bool edited{ false };
	if (ImGui::DragFloat2("Position", m_position))
	{
		edited = true;
	}
	if (ImGui::DragFloat("Rotation", &m_rotation))
	{
		edited = true;
	}
	if (ImGui::DragFloat2("Scale", m_scale))
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
void nzgdc_demo::QuadEditor::drawMenuBar(std::string popupId)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Load"))
			{
				if (loadFromJson())
				{
					UpdateQuadSettings();
				}
			}
			if (ImGui::MenuItem("Save"))
			{
				saveToJson();
			}
			if (ImGui::MenuItem("Reset coordinates"))
			{
				resetCoordinates();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
void nzgdc_demo::QuadEditor::resetCoordinates()
{
	m_position[0] = 0.0f;
	m_position[1] = 0.0f;
	m_rotation = 0.0f;
	UpdateQuadSettings();
}
bool nzgdc_demo::QuadEditor::saveToJson()
{
	m_loadedJson.clear();
	m_loadedJson["scale_x"] = m_scale[0];
	m_loadedJson["scale_y"] = m_scale[1];
	m_loadedJson["rotation"] = m_rotation;
	m_loadedJson["position_x"] = m_position[0];
	m_loadedJson["position_y"] = m_position[1];
	std::ofstream file(saveSettingsPath);
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
bool nzgdc_demo::QuadEditor::loadFromJson()
{
	std::ifstream file(saveSettingsPath);
	if (!file.is_open())
	{
		// TODO: Log error (Failed to open the file)
		return false;
	}
	Json::Value root;
	Json::CharReaderBuilder readerBuilder;
	std::string errors;
	if (!Json::parseFromStream(readerBuilder, file, &root, &errors))
	{
		// TODO: Log error (Failed to parse JSON)
		return false;
	}
	if (root.isMember("scale_x") && root["scale_x"].isNumeric())
	{
		m_scale[0] = root["scale_x"].asFloat();
	}
	if (root.isMember("scale_y") && root["scale_y"].isNumeric())
	{
		m_scale[1] = root["scale_y"].asFloat();
	}
	if (root.isMember("rotation") && root["rotation"].isNumeric())
	{
		m_rotation = root["rotation"].asFloat();
	}
	if (root.isMember("position_x") && root["position_x"].isNumeric())
	{
		m_position[0] = root["position_x"].asFloat();
	}
	if (root.isMember("position_y") && root["position_y"].isNumeric())
	{
		m_position[1] = root["position_y"].asFloat();
	}
	return true;
}