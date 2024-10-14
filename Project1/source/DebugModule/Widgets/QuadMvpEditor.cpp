#include "QuadMvpEditor.h"

#include "JsonHelper.h"

#include "misc/cpp/imgui_stdlib.h"
#include "QuadEditor.h"

nzgdc_demo::QuadMvpEditor::QuadMvpEditor(const std::shared_ptr<QuadMVP>& quadMvp)
{
}

void nzgdc_demo::QuadMvpEditor::RenderContent()
{

}

std::string nzgdc_demo::QuadMvpEditor::GetWindowId() const
{
	return "Quad Mvp Editor";
}

void nzgdc_demo::QuadMvpEditor::UpdateSettings()
{
	std::string popupId;
	DrawMenuBar(popupId);

	bool updated{false};

	updated |= ImGui::InputText("Texture Path", &m_quadMvpData.texturePath);
	updated |= QuadEditor::DrawQuadEditor(m_quadMvpData.quadData);
}

void nzgdc_demo::QuadMvpEditor::DrawMenuBar(std::string popupId)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Load"))
			{
				if (LoadFromJson())
				{
				}
			}
			if (ImGui::MenuItem("Save"))
			{
				SaveToJson();
			}
			if (ImGui::MenuItem("Reset coordinates"))
			{
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

bool nzgdc_demo::QuadMvpEditor::SaveToJson()
{
	m_loadedJson["quad_data"] = QuadEditor::Serialize(m_quadMvpData.quadData);
	m_loadedJson["texture_path"] = m_quadMvpData.texturePath;
	return JsonHelper::SaveToJson(QuadMVP::settingsPath, m_loadedJson);
}

bool nzgdc_demo::QuadMvpEditor::LoadFromJson()
{
	return false;
}
