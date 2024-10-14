#pragma once
#include <string>
#include <unordered_map>

#include "DebugWindowBase.h"

#include "json/value.h"

namespace Json
{
	class Value;
}

struct SampleData
{
	std::string Name;
	std::string Texture_Path;
	int Size_X{128};
	int Size_Y{128};
	int Point{0};
	std::string Comment;
};

namespace nzgdc_demo
{
	class DataAssetEditor: public nzgdc_demo::DebugWindowBase
	{
	public:
		DataAssetEditor();
		void RenderContent() override;
		[[nodiscard]] std::string GetWindowId() const override;

	private:
		bool LoadJson(Json::Value& outData) const;
		void ParseJson(const Json::Value& inJson);
		bool SaveToJson();

	private:
		inline static std::string settingsPath {"res/assets/Data.json"};

		struct EnabledViewData {
			bool Texture{false};
			bool Size{false};
			bool Table{false};
			bool TreeNode{false};
			bool ComboBox{false};
			bool InputText{false};
		} m_viewSettings;

		std::unordered_map<std::string, std::unordered_map<std::string, SampleData>> m_levelData;
		Json::Value m_localJson;
	};
}
