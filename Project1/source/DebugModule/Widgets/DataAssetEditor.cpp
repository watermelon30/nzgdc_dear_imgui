#include "DataAssetEditor.h"

#include <fstream>
#include <iostream>

#include "json/reader.h"
#include "json/writer.h"
nzgdc_demo::DataAssetEditor::DataAssetEditor()
{
	if (LoadJson(m_localJson))
	{
		ParseJson(m_localJson);
	}
}

void nzgdc_demo::DataAssetEditor::RenderContent()
{
	ImGui::Begin(GetWindowId().c_str());
	ImGui::End();
}

std::string nzgdc_demo::DataAssetEditor::GetWindowId() const
{
	return "Data asset editor sample";
}

bool nzgdc_demo::DataAssetEditor::LoadJson(Json::Value& outData) const
{
	std::ifstream file(settingsPath);
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
	auto data = outData.toStyledString();
	std::cout << data.c_str() << std::endl;
	return true;
}

void nzgdc_demo::DataAssetEditor::ParseJson(const Json::Value& inJson)
{
	std::vector<std::string> levelNames = inJson.getMemberNames();

	for (Json::Value::ArrayIndex i = 0; i < inJson.size(); i++)
	{
		const Json::Value levelJson = inJson[i];
		std::string levelId = levelNames[i];

		std::unordered_map<std::string, SampleData> levelData;

		std::vector<std::string> dataNames = levelJson.getMemberNames();
		for (Json::Value::ArrayIndex j = 0; j != inJson.size(); j++)
		{
			Json::Value dataJson = levelJson[j];
			std::string dataId = dataNames[j];
			SampleData data;
			if (dataJson.isMember("name") && inJson["name"].isString())
			{
				data.Name = dataJson["name"].asString();
			}
			if (dataJson.isMember("texture_path") && inJson["texture_path"].isString())
			{
				data.Texture_Path = dataJson["texture_path"].asString();
			}
			if (dataJson.isMember("size_x") && inJson["size_x"].isInt())
			{
				data.Size_X = dataJson["size_x"].asInt();
			}
			if (dataJson.isMember("size_y") && inJson["size_y"].isInt())
			{
				data.Size_Y = dataJson["size_y"].asInt();
			}
			if (dataJson.isMember("point") && inJson["point"].isInt())
			{
				data.Point = dataJson["point"].asInt();
			}
			if (dataJson.isMember("comment") && inJson["comment"].isString())
			{
				data.Comment = dataJson["comment"].asString();
			}
			levelData.emplace(dataId, data);
		}
		m_levelData.emplace(levelId, levelData);
	}
}

bool nzgdc_demo::DataAssetEditor::SaveToJson()
{
	Json::Value newVal;
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

	std::ofstream file(settingsPath);
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
