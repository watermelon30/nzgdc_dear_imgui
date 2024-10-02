#pragma once
#include <memory>

#include "DebugWindowBase.h"

namespace nzgdc_demo
{
    class ParticleSystem;

    class ParticleSystemEditor : public DebugWindowBase
    {
    public:
        ParticleSystemEditor(const std::shared_ptr<ParticleSystem>& particleSystem);

        [[nodiscard]] std::string GetWindowId() const override;
        void RenderContent() override;

        static void DrawParticleSystemEditor(std::shared_ptr<ParticleSystem>& particleSystem);
    private:
        void DrawMenuBar(std::string& popupId);

        bool SaveToJson();

    private:
        std::shared_ptr<ParticleSystem> m_particleSystem;
    };
}
