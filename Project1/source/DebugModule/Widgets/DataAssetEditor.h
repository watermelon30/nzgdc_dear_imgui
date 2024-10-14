#pragma once
#include <string>
#include <unordered_map>

#include "DebugWindowBase.h"
#include "Texture.h"

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
	std::vector<int> Available_Levels;
	inline static constexpr int POINT_MAX{10};
	inline static constexpr int POINT_MIN{0};
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

		void DrawMenuBar(std::string& popupId);
		void DrawPopups(const std::string& popupId);

		void DrawItemList();
		void DrawDataEditor();

		void DrawPointComboBox(int& targetPoint);
		void DrawAvailableLevelTable(std::vector<int>& AvailableLevels);


		void* getTargetTexture(const std::string& textureId, const std::string& texturePath);

	private:
		static constexpr std::string_view SettingsPath {"res/assets/Data.json"};
		static constexpr std::string_view ConfirmLoadJsonPopupId{"LoadFromJson"};
		static constexpr std::string_view ConfirmSaveJsonPopupId{"SaveToJson"};

		static constexpr int TexturePreviewSize {128};

		std::unordered_map<std::string, std::unordered_map<std::string, SampleData>> m_levelData;
		std::unordered_map<std::string, Texture> m_textureMap;

		Json::Value m_localJson;

		std::pair<std::string, std::unordered_map<std::string, SampleData>> m_currentEditingLevel;
		std::pair<std::string, SampleData> m_currentEditingData;
	};
}
