#pragma once
#include <memory>

#include "DebugWindowBase.h"
#include "QuadMVP.h"

namespace nzgdc_demo
{
	class QuadMvpEditor : public nzgdc_demo::DebugWindowBase
	{
	public:
		QuadMvpEditor(const std::shared_ptr<QuadMVP>& quadMvp);
		void RenderContent() override;
		[[nodiscard]] std::string GetWindowId() const override;

		void UpdateSettings();

	private:
		void DrawMenuBar(std::string popupId);
		bool SaveToJson();
		bool LoadFromJson();

	private:
		std::shared_ptr<QuadMVP> m_quadMvp;
		QuadMvpData m_quadMvpData;
		Json::Value m_loadedJson;
	};
}
