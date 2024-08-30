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

        virtual std::string GetWindowId() const override;
        virtual void RenderContent() override;

    private:
        std::shared_ptr<ParticleSystem> m_particleSystem;
    };
}
