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
        void Init(GLFWwindow* share = NULL) override;

    protected:
        void OnUpdate(float deltaTime) override;
        void OnRender(float deltaTime) override;
    private:
        std::shared_ptr<FluidSimulator> m_fluidSimulator;
    };
}
