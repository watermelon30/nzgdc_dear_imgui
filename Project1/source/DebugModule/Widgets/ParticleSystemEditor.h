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

        std::string GetWindowId() const override;
        void RenderContent() override;

    private:
        std::shared_ptr<ParticleSystem> m_particleSystem;
    };
}
