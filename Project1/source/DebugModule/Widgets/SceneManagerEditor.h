#pragma once
#include <memory>

#include "DebugWindowBase.h"

#include "QuadMVP.h"

#include "ParticleSystem/ParticleSystem.h"

namespace nzgdc_demo {
    class SceneManager;
    class Quad;

    class SceneManagerEditor : public DebugWindowBase
    {
    public:
        SceneManagerEditor(const std::shared_ptr<SceneManager>& sceneManager);
        void RenderContent() override;
        [[nodiscard]] std::string GetWindowId() const override;

    private:
        void DrawMenuBar(std::string& popupId);
        bool SaveToJson();
        bool LoadFromJson();

    private:
        std::shared_ptr<SceneManager> m_sceneManager;
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<QuadMVP> m_targetQuad;
        QuadData m_targetQuadData;
        std::shared_ptr<ParticleSystem> m_targetParticleSystem;
    };
}
