#pragma once
#include <memory>

#include "DebugWindowBase.h"
#include <json/value.h>

namespace nzgdc_demo {
	class Quad;

	class QuadEditor : public DebugWindowBase
	{
	public:
		QuadEditor(const std::shared_ptr<Quad>& quad);
		void UpdateQuadSettings();
		void RenderContent() override;
		[[nodiscard]] std::string GetWindowId() const override;

	private:
		void drawMenuBar(std::string popupId);
		void resetCoordinates();
		bool saveToJson();
		bool loadFromJson();

	private:
		inline static std::string saveSettingsPath{"res/assets/QuadEditor.json"};
		std::shared_ptr<Quad> m_quad;
		Json::Value m_loadedJson;
		// ImGui requires a reference& to the data it manipulates.
		// store the reference in this class and upate quad every frame
		float m_position[2]{0.0f, 0.0f};
		float m_rotation{0.0f};
		float m_scale[2]{1.0f, 1.0f};
	};
}
