#include "FluidSimulatorWindow.h"

#include "FluidSimulator/FluidSimulator.h"

nzgdc_demo::FluidSimulatorWindow::FluidSimulatorWindow()
    : m_fluidSimulator(std::make_shared<FluidSimulator>())
{
}

void nzgdc_demo::FluidSimulatorWindow::OnUpdate(float deltaTime)
{
    m_fluidSimulator->Update(deltaTime);
}

void nzgdc_demo::FluidSimulatorWindow::OnRender(float deltaTime)
{
    m_fluidSimulator->Render(deltaTime);
}
