#pragma once
#include <memory>

#include "DebugWindowBase.h"
#include <json/value.h>

#include "QuadMVP.h"

namespace nzgdc_demo {
	class Quad;

	class QuadEditor : public DebugWindowBase
	{
	public:
		QuadEditor(const std::shared_ptr<Quad>& quad);
		void UpdateQuadSettings();
		void RenderContent() override;
		[[nodiscard]] std::string GetWindowId() const override;

		static bool DrawQuadEditor(QuadData& quadData);
		static Json::Value Serialize(const QuadData& data);

	private:
		void DrawMenuBar(std::string popupId);
		void ResetCoordinates();
		bool SaveToJson();
		bool LoadFromJson();

	private:
		std::shared_ptr<Quad> m_quad;
		QuadData m_quadData;
		Json::Value m_loadedJson;
	};
}
