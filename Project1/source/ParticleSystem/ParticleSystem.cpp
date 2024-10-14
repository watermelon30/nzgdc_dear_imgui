#include "ParticleSystem.h"

#include <fstream>
#include <iostream>

#include "Camera.h"
#include "Particle.h"

#include "json/reader.h"

nzgdc_demo::ParticleSystem::ParticleSystem(const ParticleSystemData& data, const Shader& shader, const std::shared_ptr<Camera>& camera)
    : m_data(data)
    , m_camera(camera)
    , m_shader(shader)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    LoadLocalData();
}

void nzgdc_demo::ParticleSystem::Update(float deltaTime)
{
    if (!m_isPlaying) return;

    std::erase_if(m_particles, [this](const std::shared_ptr<Particle>& particle)
    {
        return particle->GetRemainingLifeTime() >= m_data.LifeTime;
    });

    for (const auto& particle : m_particles)
    {
        particle->SetView(m_camera->GetView());
        particle->SetProjection(m_camera->GetProjection());
        particle->Update(deltaTime);
    }
    
    if (m_timer >= m_data.Duration)
    {
        if (!m_data.Looping)
        {
            m_isPlaying = false;
            return;
        }
        Restart();
    }

    m_timer += deltaTime;
    m_emissionTimer += deltaTime;

    if (m_emissionTimer >= 1.0f / m_data.EmissionRate)
    {
        if (m_data.Looping || m_timer < m_data.Duration - m_data.LifeTime)
        {
            EmitParticle();
            m_emissionTimer = 0.0f;
        }
    }
}

void nzgdc_demo::ParticleSystem::Render()
{
    for (const auto& particle : m_particles)
    {
        particle->Render();
    }
}

void nzgdc_demo::ParticleSystem::Play()
{
    if (m_timer >= m_data.Duration)
    {
        m_timer = 0.0f;
    }
    m_isPlaying = true;
    EmitParticle();
}

void nzgdc_demo::ParticleSystem::Pause()
{
    m_isPlaying = false;
}

void nzgdc_demo::ParticleSystem::Restart()
{
    m_isPlaying = true;
    m_timer = 0.0f;
}

void nzgdc_demo::ParticleSystem::Stop()
{
    m_isPlaying = false;
    m_timer = 0.0f;
    m_particles.clear();
}

void nzgdc_demo::ParticleSystem::SetData(const ParticleSystemData& newData)
{
    m_data = newData;
}
bool nzgdc_demo::ParticleSystem::LoadJson(Json::Value& outData) const
{
    std::ifstream file(settingsPath);
    if (!file.is_open())
    {
        // TODO: Log error (Failed to open the file)
        return false;
    }

    Json::CharReaderBuilder readerBuilder;
    std::string errors;

    if (!Json::parseFromStream(readerBuilder, file, &outData, &errors))
    {
        // TODO: Log error (Failed to parse JSON)
        return false;
    }
    return true;
}

void nzgdc_demo::ParticleSystem::ParseJson(const Json::Value& inJson, ParticleSystemData& outParticleSystemData)
{
    if (inJson.isMember("looping") && inJson["looping"].isBool())
    {
        outParticleSystemData.Looping = inJson["looping"].asBool();
    }
    if (inJson.isMember("duration") && inJson["duration"].isNumeric())
    {
        outParticleSystemData.Duration = inJson["duration"].asFloat();
    }
    if (inJson.isMember("max_particles") && inJson["max_particles"].isInt())
    {
        outParticleSystemData.MaxParticles = inJson["max_particles"].asInt();
    }
    if (inJson.isMember("emission_rate") && inJson["emission_rate"].isNumeric())
    {
        outParticleSystemData.EmissionRate = inJson["emission_rate"].asFloat();
    }
    if (inJson.isMember("radius") && inJson["radius"].isNumeric())
    {
        outParticleSystemData.Radius = inJson["radius"].asFloat();
    }
    if (inJson.isMember("arc") && inJson["arc"].isInt())
    {
        outParticleSystemData.Arc = inJson["arc"].asInt();
    }
    if (inJson.isMember("angular_velocity") && inJson["angular_velocity"].isNumeric())
    {
        outParticleSystemData.AngularVelocity = inJson["angular_velocity"].asFloat();
    }
    if (inJson.isMember("particle_speed") && inJson["particle_speed"].isNumeric())
    {
        outParticleSystemData.ParticleSpeed = inJson["particle_speed"].asFloat();
    }
    if (inJson.isMember("position_x") && inJson["position_x"].isNumeric())
    {
        outParticleSystemData.Position.x = inJson["position_x"].asFloat();
    }
    if (inJson.isMember("position_y") && inJson["position_y"].isNumeric())
    {
        outParticleSystemData.Position.y = inJson["position_y"].asFloat();
    }
    if (inJson.isMember("position_z") && inJson["position_z"].isNumeric())
    {
        outParticleSystemData.Position.z = inJson["position_z"].asFloat();
    }
    if (inJson.isMember("rotation") && inJson["rotation"].isNumeric())
    {
        outParticleSystemData.Rotation = inJson["rotation"].asFloat();
    }
    if (inJson.isMember("start_size_x") && inJson["start_size_x"].isNumeric())
    {
        outParticleSystemData.StartSize.x = inJson["start_size_x"].asFloat();
    }
    if (inJson.isMember("start_size_y") && inJson["start_size_y"].isNumeric())
    {
        outParticleSystemData.StartSize.y = inJson["start_size_y"].asFloat();
    }
    if (inJson.isMember("start_size_z") && inJson["start_size_z"].isNumeric())
    {
        outParticleSystemData.StartSize.z = inJson["start_size_z"].asFloat();
    }
    if (inJson.isMember("lifetime") && inJson["lifetime"].isNumeric())
    {
        outParticleSystemData.LifeTime = inJson["lifetime"].asFloat();
    }
    if (inJson.isMember("use_size_over_lifetime") && inJson["use_size_over_lifetime"].isBool())
    {
        outParticleSystemData.UseSizeOverLifeTime = inJson["use_size_over_lifetime"].asBool();
    }
    if (inJson.isMember("end_size_x") && inJson["end_size_x"].isNumeric())
    {
        outParticleSystemData.EndSize.x = inJson["end_size_x"].asFloat();
    }
    if (inJson.isMember("end_size_y") && inJson["end_size_y"].isNumeric())
    {
        outParticleSystemData.EndSize.y = inJson["end_size_y"].asFloat();
    }
    if (inJson.isMember("end_size_z") && inJson["end_size_z"].isNumeric())
    {
        outParticleSystemData.EndSize.z = inJson["end_size_z"].asFloat();
    }
    if (inJson.isMember("acceleration_x") && inJson["acceleration_x"].isNumeric())
    {
        outParticleSystemData.Acceleration.x = inJson["acceleration_x"].asFloat();
    }
    if (inJson.isMember("acceleration_y") && inJson["acceleration_y"].isNumeric())
    {
        outParticleSystemData.Acceleration.y = inJson["acceleration_y"].asFloat();
    }
    if (inJson.isMember("acceleration_z") && inJson["acceleration_z"].isNumeric())
    {
        outParticleSystemData.Acceleration.z = inJson["acceleration_z"].asFloat();
    }
    if (inJson.isMember("start_color_r") && inJson["start_color_r"].isNumeric())
    {
        outParticleSystemData.StartColor.r = inJson["start_color_r"].asFloat();
    }
    if (inJson.isMember("start_color_g") && inJson["start_color_g"].isNumeric())
    {
        outParticleSystemData.StartColor.g = inJson["start_color_g"].asFloat();
    }
    if (inJson.isMember("start_color_b") && inJson["start_color_b"].isNumeric())
    {
        outParticleSystemData.StartColor.b = inJson["start_color_b"].asFloat();
    }
    if (inJson.isMember("start_color_a") && inJson["start_color_a"].isNumeric())
    {
        outParticleSystemData.StartColor.a = inJson["start_color_a"].asFloat();
    }
    if (inJson.isMember("use_color_over_lifetime") && inJson["use_color_over_lifetime"].isBool())
    {
        outParticleSystemData.UseColorOverLifeTime = inJson["use_color_over_lifetime"].asBool();
    }
    if (inJson.isMember("end_color_r") && inJson["end_color_r"].isNumeric())
    {
        outParticleSystemData.EndColor.r = inJson["end_color_r"].asFloat();
    }
    if (inJson.isMember("end_color_g") && inJson["end_color_g"].isNumeric())
    {
        outParticleSystemData.EndColor.g = inJson["end_color_g"].asFloat();
    }
    if (inJson.isMember("end_color_b") && inJson["end_color_b"].isNumeric())
    {
        outParticleSystemData.EndColor.b = inJson["end_color_b"].asFloat();
    }
    if (inJson.isMember("end_color_a") && inJson["end_color_a"].isNumeric())
    {
        outParticleSystemData.EndColor.a = inJson["end_color_a"].asFloat();
    }
}
void nzgdc_demo::ParticleSystem::LoadLocalData()
{
    Json::Value jsonVal;
    if (LoadJson(jsonVal))
    {
        ParseJson(jsonVal, m_data);
    }
}

void nzgdc_demo::ParticleSystem::EmitParticle()
{
    if (m_particles.size() >= m_data.MaxParticles) return;
    auto particle = GenerateParticle();
    m_particles.emplace_back(particle);
}

std::shared_ptr<nzgdc_demo::Particle> nzgdc_demo::ParticleSystem::GenerateParticle() const
{
    const float randomRadius = static_cast <float>(std::rand()) / (RAND_MAX / m_data.Radius);
    
    ParticleData data;
    data.LifeTime = m_data.LifeTime;
    data.AngularVelocity = m_data.AngularVelocity;
    data.Velocity = glm::vec3(CalculateParticleVelocity(), 0.0f) * m_data.ParticleSpeed;
    data.StartPosition = m_data.Position + glm::vec3(CalculateParticleVelocity(), 0.0f) * randomRadius;
    data.StartSize = m_data.StartSize;
    data.UseSizeOverLifeTime = m_data.UseSizeOverLifeTime;
    data.EndSize = m_data.EndSize;
    data.Acceleration = m_data.Acceleration;
    data.UseColorOverLifeTime = m_data.UseColorOverLifeTime;
    data.StartColor = m_data.StartColor;
    data.EndColor = m_data.EndColor;

    // TODO: use a pool?
    return std::make_shared<Particle>(data, m_shader);
}

glm::vec2 nzgdc_demo::ParticleSystem::CalculateParticleVelocity() const
{
    const auto randomDirection = m_data.Rotation + static_cast<float>(std::rand() % (m_data.Arc + 1));
    return {glm::cos(glm::radians(randomDirection)), glm::sin(glm::radians(randomDirection))};
}
