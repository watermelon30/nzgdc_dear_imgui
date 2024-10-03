#pragma once
#include <memory>

#include "DebugWindowBase.h"

#include "json/value.h"

namespace nzgdc_demo
{
    struct ParticleSystemData;
    class ParticleSystem;

    class ParticleSystemEditor : public DebugWindowBase
    {
    public:
        ParticleSystemEditor(const std::shared_ptr<ParticleSystem>& particleSystem);

        [[nodiscard]] std::string GetWindowId() const override;
        void RenderContent() override;

        static void DrawParticleSystemEditor(std::shared_ptr<ParticleSystem>& particleSystem);
        static Json::Value Serialize(const ParticleSystemData& data);
    private:
        void DrawMenuBar(std::string& popupId);

        bool SaveToJson();

    private:
        std::shared_ptr<ParticleSystem> m_particleSystem;
    };
}
