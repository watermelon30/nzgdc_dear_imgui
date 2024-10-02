#include "SceneManager.h"

#include "Camera.h"

#include "ParticleSystem/ParticleSystem.h"

#include "Shader/Shader.h"
nzgdc_demo::SceneManager::SceneManager()
{
}
void nzgdc_demo::SceneManager::Init(float width, float height)
{
    const CameraData cameraData(ProjectionType::Orthographic, width, height, glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera = std::make_shared<Camera>(cameraData);

    static Shader particleShader("res/shaders/particle.vs", "res/shaders/particle.frag");
    ParticleSystemData particleSystemData;
    particleSystemData.AngularVelocity = 100.0f;
    auto particleSystem = std::make_shared<ParticleSystem>(particleSystemData, particleShader, m_camera);
    m_particleSystems.emplace_back(particleSystem);
    particleSystem->Play();

    static const Shader defaultShader("res/shaders/basic.vs", "res/shaders/basic.frag");
    auto quadMVP = std::make_shared<QuadMVP>(defaultShader, "res/textures/jack.jpg");
    m_quadMVPs.emplace_back(quadMVP);
}

void nzgdc_demo::SceneManager::Update(float deltaTime)
{
    for (auto& quad : m_quadMVPs)
    {
        quad->SetView(m_camera->GetView());
        quad->SetProjection(m_camera->GetProjection());
    }
    
    for (auto& particleSystem : m_particleSystems)
    {
        particleSystem->Update(deltaTime);
    }
}

void nzgdc_demo::SceneManager::Render()
{
    for (auto& quad : m_quadMVPs)
    {
        quad->Render();
    }
    for (auto& particleSystem : m_particleSystems)
    {
        particleSystem->Render();
    }
}
std::shared_ptr<nzgdc_demo::Camera> nzgdc_demo::SceneManager::GetCamera() const
{
    return m_camera;
}
std::vector<std::shared_ptr<nzgdc_demo::QuadMVP>> nzgdc_demo::SceneManager::GetQuadsVector() const
{
    return m_quadMVPs;
}
std::vector<std::shared_ptr<nzgdc_demo::ParticleSystem>> nzgdc_demo::SceneManager::GetParticleSystemsVector() const
{
    return m_particleSystems;
}
