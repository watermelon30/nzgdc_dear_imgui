#include "SceneManager.h"

#include <fstream>

#include "Camera.h"
#include "JsonHelper.h"

#include "json/reader.h"

#include "ParticleSystem/ParticleSystem.h"

#include "Shader/Shader.h"
nzgdc_demo::SceneManager::SceneManager()
{
}
void nzgdc_demo::SceneManager::Init(float width, float height)
{
    const CameraData cameraData(ProjectionType::Orthographic, width, height, glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera = std::make_shared<Camera>(cameraData);
    LoadLocalData();
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

void nzgdc_demo::SceneManager::ParseJson(const Json::Value& inJson, SceneManager* outSceneManager)
{
    static Shader particleShader("res/shaders/particle.vs", "res/shaders/particle.frag");
    static const Shader defaultShader("res/shaders/basic.vs", "res/shaders/basic.frag");

    if (inJson.isMember("ParticleSystems") && inJson["ParticleSystems"].isArray())
    {
        for (Json::Value::ArrayIndex i = 0; i != inJson["ParticleSystems"].size(); i++) // for (const auto& particleSystem : inJson["ParticleSystems"]) doesnt work for some reason
        {
            ParticleSystemData particleSystemData;
            particleSystemData.AngularVelocity = 100.0f;
            ParticleSystem::ParseJson(inJson["ParticleSystems"][i], particleSystemData);
            std::shared_ptr<ParticleSystem> particleSystem = std::make_shared<ParticleSystem>(particleSystemData, particleShader, outSceneManager->GetCamera());
            particleSystem->Play();
            outSceneManager->m_particleSystems.emplace_back(particleSystem);
        }
    }
    
    if (inJson.isMember("Quads") && inJson["Quads"].isArray())
    {
        for (Json::Value::ArrayIndex i = 0; i != inJson["Quads"].size(); i++)
        {
            std::shared_ptr<QuadMVP> quadMvp = std::make_shared<QuadMVP>(defaultShader, "res/textures/jack.jpg"); // TODO: load path from json too
            QuadData data;
            Quad::ParseJson(inJson["Quads"][i], data);
            quadMvp->SetTransformData(data);
            outSceneManager->m_quadMVPs.emplace_back(quadMvp);
        }
    }
}

void nzgdc_demo::SceneManager::LoadLocalData()
{
    Json::Value jsonData;
    if (!JsonHelper::LoadJson(settingsPath, jsonData))
    {
        return;
    }
        
    ParseJson(jsonData, this);
}
