#pragma once
#include <memory>

#include "Window.h"

namespace nzgdc_demo
{
    class FluidSimulator;

    class FluidSimulatorWindow : public Window
    {
    public:
        FluidSimulatorWindow();
        
    protected:
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender(float deltaTime) override;
        
    private:
        std::shared_ptr<FluidSimulator> m_fluidSimulator;
    };
}
