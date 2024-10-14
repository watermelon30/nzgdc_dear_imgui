#include "DataAssetEditor.h"

#include <fstream>
#include <iostream>

#include "json/reader.h"
#include "json/writer.h"
#include "misc/cpp/imgui_stdlib.h" // Necessary for using std::string with ImGui::InputText


nzgdc_demo::DataAssetEditor::DataAssetEditor()
{
	 if (LoadJson(m_localJson))
	 {
	 	ParseJson(m_localJson);
	 }
	m_flags = ImGuiWindowFlags_MenuBar;

	std::unordered_map<std::string, SampleData> levelData;
	for (int i = 0; i < 3; ++i)
	{
		SampleData data;
		data.Name = "Jack";
		data.Texture_Path = "res/textures/jack.jpg";
		data.Size_X = 256;
		data.Size_Y = 512;
		data.Point = 8;
		data.Comment = "jack is a beast";
		data.Available_Levels = {1,2,3,4};
		levelData.emplace(std::to_string(i), data);
	}
	m_levelData.emplace("level_1", levelData);
}

void nzgdc_demo::DataAssetEditor::RenderContent()
{
	ImGui::Begin(GetWindowId().c_str());

	std::string popupId;
	DrawMenuBar(popupId);
	DrawPopups(popupId);

	if (m_currentEditingLevel.first.empty())
	{
		ImGui::TextUnformatted("No target level selected.\nSelect a level from open menu");
	}
	else
	{
		ImGui::Columns(2);
		DrawItemList();
		ImGui::NextColumn();
		DrawDataEditor();
	}

	ImGui::End();
}

std::string nzgdc_demo::DataAssetEditor::GetWindowId() const
{
	return "Data asset editor sample";
}

bool nzgdc_demo::DataAssetEditor::LoadJson(Json::Value& outData) const
{
	std::ifstream file(SettingsPath.data());
	if (!file.is_open())
	{
		// TODO: Log error (Failed to open the file)
		return false;
	}

	Json::CharReaderBuilder readerBuilder;
	std::string errors;

	if (!Json::parseFromStream(readerBuilder, file, &outData, &errors))
	{
		// TODO: Log error (Failed to parse JSON)
		return false;
	}
	return true;
}

void nzgdc_demo::DataAssetEditor::ParseJson(const Json::Value& inJson)
{

	for (const auto& levelId : inJson.getMemberNames())
	{
		const Json::Value levelJson = inJson[levelId];
		std::unordered_map<std::string, SampleData> levelData;

		for (const auto& dataId : levelJson.getMemberNames())
		{
			Json::Value dataJson = levelJson[dataId];
			SampleData data;
			std::string dataStr = dataJson.toStyledString();
			if (dataJson.isMember("name") && dataJson["name"].isString())
			{
				data.Name = dataJson["name"].asString();
			}
			if (dataJson.isMember("texture_path") && dataJson["texture_path"].isString())
			{
				data.Texture_Path = dataJson["texture_path"].asString();
			}
			if (dataJson.isMember("size_x") && dataJson["size_x"].isInt())
			{
				data.Size_X = dataJson["size_x"].asInt();
			}
			if (dataJson.isMember("size_y") && dataJson["size_y"].isInt())
			{
				data.Size_Y = dataJson["size_y"].asInt();
			}
			if (dataJson.isMember("point") && dataJson["point"].isInt())
			{
				data.Point = dataJson["point"].asInt();
			}
			if (dataJson.isMember("comment") && dataJson["comment"].isString())
			{
				data.Comment = dataJson["comment"].asString();
			}
			if (dataJson.isMember("available_levels") && dataJson["available_levels"].isArray()) {
				for (const auto& level : dataJson["available_levels"]) {
					if (level.isInt()) {
						data.Available_Levels.push_back(level.asInt());
					}
				}
			}
			levelData.emplace(dataId, data);
		}
		m_levelData.emplace(levelId, levelData);
	}
}

bool nzgdc_demo::DataAssetEditor::SaveToJson()
{
	Json::Value newVal;

	// Update edited data
	if (m_levelData.contains(m_currentEditingLevel.first))
	{
		m_levelData[m_currentEditingLevel.first] = m_currentEditingLevel.second;
	}

	for (const std::pair<std::string, std::unordered_map<std::string, SampleData>>& levelData : m_levelData)
	{
		Json::Value levelJson;
		for (const std::pair<std::string, SampleData>& objectData : levelData.second)
		{
			Json::Value objectJson;
			objectJson["name"] = objectData.second.Name;
			objectJson["texture_path"] = objectData.second.Texture_Path;
			objectJson["size_x"] = objectData.second.Size_X;
			objectJson["size_y"] = objectData.second.Size_Y;
			objectJson["point"] = objectData.second.Point;
			objectJson["comment"] = objectData.second.Comment;
			levelJson[objectData.first] = objectJson;
		}
		newVal[levelData.first] = levelJson;
	}

	std::ofstream file(SettingsPath.data());
	if (!file.is_open())
	{
		// TODO: Log error
		return false;
	}
    
	Json::StreamWriterBuilder writerBuilder;
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	writer->write(newVal, &file);
	m_localJson = newVal;
	return true;
}

void nzgdc_demo::DataAssetEditor::DrawMenuBar(std::string& popupId)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Open"))
		{
			for (const auto& level : m_levelData)
			{
				if (ImGui::MenuItem(level.first.c_str()))
				{
					m_currentEditingLevel = level;
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Load From Json"))
			{
				popupId = ConfirmLoadJsonPopupId;
			}
			if (ImGui::MenuItem("Save"))
			{
				popupId = ConfirmSaveJsonPopupId;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
void nzgdc_demo::DataAssetEditor::DrawPopups(const std::string& popupId)
{
	if (!popupId.empty())
	{
		ImGui::OpenPopup(popupId.c_str());
	}

	if (ImGui::BeginPopup(ConfirmLoadJsonPopupId.data()))
	{
		if (ImGui::Button("Reload data from local json file?"))
		{
			if (LoadJson(m_localJson))
			{
				ParseJson(m_localJson);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
		{
			ImGui::TextUnformatted("You can also click anywhere outside the popup to close");
			ImGui::EndTooltip();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal(ConfirmSaveJsonPopupId.data()))
	{
		if (ImGui::Button("Save data to local json file?"))
		{
			SaveToJson();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void nzgdc_demo::DataAssetEditor::DrawItemList()
{
	ImGui::SeparatorText(("Current Level: " + m_currentEditingLevel.first).c_str());

	if (ImGui::BeginTable("ItemListTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 125.0f);
		ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthFixed, 150.0f);
		ImGui::TableHeadersRow();
		for (auto& data : m_currentEditingLevel.second)
		{
			ImGui::PushID(("table_row_" + data.first).c_str());
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); // Same as ImGui::TableNextColumn()
				{
					ImGui::TextUnformatted(data.first.c_str());
				}
				ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(1)
				{
					ImGui::TextUnformatted(data.second.Name.c_str());
				}
				ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(2)
				{
					void* targetTextureId = getTargetTexture(data.first, data.second.Texture_Path);
					ImGui::Image(targetTextureId, ImVec2(TexturePreviewSize, TexturePreviewSize), ImVec2(0, 1), ImVec2(1, 0));
				}
				ImGui::SameLine();
				if (ImGui::Selectable("##selectable", m_currentEditingData.first == data.first, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, TexturePreviewSize))) {
					m_currentEditingData = data;
				}
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

void nzgdc_demo::DataAssetEditor::DrawDataEditor()
{
	if (m_currentEditingData.first.empty())
	{
		ImGui::TextUnformatted("Select an object from the left to edit");
		return;
	}
	ImGui::Text("Item Id: %s", m_currentEditingData.first.c_str());
	ImGui::InputText("Name", &m_currentEditingData.second.Name);
	ImGui::InputText("Texture Path", &m_currentEditingData.second.Texture_Path);

	ImGui::InputInt("Size X", &m_currentEditingData.second.Size_X);
	ImGui::DragInt("Size Y", &m_currentEditingData.second.Size_Y, 1, 0, INT_MAX);

	DrawPointComboBox(m_currentEditingData.second.Point);

	ImGui::InputTextMultiline("Comment", &m_currentEditingData.second.Comment, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 5));

	if (ImGui::TreeNode("Edit available levels"))
	{
		if (ImGui::Button("Add new level"))
		{
			m_currentEditingData.second.Available_Levels.emplace_back(0);
		}
		DrawAvailableLevelTable(m_currentEditingData.second.Available_Levels);
		ImGui::TreePop();
	}
}

void nzgdc_demo::DataAssetEditor::DrawPointComboBox(int& targetPoint)
{
	if (ImGui::BeginCombo("Point", std::to_string(targetPoint).c_str()))
	{
		for (int point = SampleData::POINT_MIN; point < SampleData::POINT_MAX; ++point)
		{
			if (ImGui::Selectable(std::to_string(point).c_str(), targetPoint == point))
			{
				targetPoint = point;
			}
		}
		ImGui::EndCombo();
	}
}
void nzgdc_demo::DataAssetEditor::DrawAvailableLevelTable(std::vector<int>& AvailableLevels)
{
	if (ImGui::BeginTable("AvailableLevelTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		ImGui::TableSetupColumn("LevelId", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Options", ImGuiTableColumnFlags_WidthStretch, 125.0f);
		ImGui::TableHeadersRow();

		int idToRemove{-1};
		for (int i = 0; i < AvailableLevels.size(); ++i)
		{
			int& data = AvailableLevels[i];
			ImGui::PushID(("table_row_" + std::to_string(i)).c_str());
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); // Same as ImGui::TableNextColumn()
				{
					ImGui::InputInt("##AvailableLevelId",&data);
				}
				ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(1)
				{
					if (ImGui::Button("Remove"))
					{
						idToRemove = i;
					}
				}
			}
			ImGui::PopID();
		}
		if (idToRemove >= 0)
		{
			AvailableLevels.erase(AvailableLevels.begin() + idToRemove);
		}
		ImGui::EndTable();
	}
}

void* nzgdc_demo::DataAssetEditor::getTargetTexture(const std::string& textureId, const std::string& texturePath)
{
	if (m_textureMap.contains(textureId))
	{
		return reinterpret_cast<void*>(m_textureMap[textureId]->GetTextureId());
	}
	else
	{
		auto texture = std::make_shared<nzgdc_demo::Texture>(texturePath);
		m_textureMap.emplace(textureId, texture);
		return reinterpret_cast<void*>(texture->GetTextureId());
	}
}
