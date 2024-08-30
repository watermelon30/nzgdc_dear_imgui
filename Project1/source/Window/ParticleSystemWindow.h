#pragma once
#include <memory>

#include "Window.h"

namespace nzgdc_demo
{
#ifdef _DEBUG
    class DebugSystem;
#endif
    class QuadMVP;
    class Camera;
    class ParticleSystem;

    class ParticleSystemWindow : public Window
    {
    public:
        ParticleSystemWindow();

        void InitializeDebugWindow(const std::weak_ptr<DebugSystem>& debugSystem);
        
    protected:
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender(float deltaTime) override;
        
    private:
        std::shared_ptr<ParticleSystem> m_particleSystem;
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<QuadMVP> m_quadMVP;
    };
}
