#include "ParticleSystemWindow.h"

#include "Camera.h"
#include "ParticleSystem.h"
#include "Shader.h"
#include "glad/glad.h"
#include "Widgets/ParticleSystemEditor.h"

#ifdef _DEBUG
#include "DebugSystem.h"
#endif

nzgdc_demo::ParticleSystemWindow::ParticleSystemWindow()
{
    CameraData cameraData;
    cameraData.Projection = ProjectionType::Orthographic;
    cameraData.Width = m_windowWidth;
    cameraData.Height = m_windowHeight;
    cameraData.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_camera = std::make_shared<Camera>(cameraData);

    Shader particleShader("res/shaders/particle.vs", "res/shaders/particle.frag");
    m_quadMVP = std::make_shared<QuadMVP>(particleShader);
    m_quadMVP->GetTransform().Scale = glm::vec3(100.0f);
		
    ParticleSystemData particleSystemData;
    particleSystemData.AngularVelocity = 100.0f;

    m_particleSystem = std::make_shared<ParticleSystem>(particleSystemData, particleShader, m_camera);
    m_particleSystem->Play();
}

#ifdef _DEBUG
void nzgdc_demo::ParticleSystemWindow::InitializeDebugWindow(const std::weak_ptr<DebugSystem>& debugSystem)
{
    if (const auto& debugSystemPtr = debugSystem.lock())
    {
        debugSystemPtr->AddWindow(std::make_shared<ParticleSystemEditor>(m_particleSystem), true);
    }
}
#endif

void nzgdc_demo::ParticleSystemWindow::OnUpdate(float deltaTime)
{
    m_particleSystem->Update(deltaTime);
}

void nzgdc_demo::ParticleSystemWindow::OnRender(float deltaTime)
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_particleSystem->Render();
}
