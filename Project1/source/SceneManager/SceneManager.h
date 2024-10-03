#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Shader/Shader.h"


namespace Json
{
    class Value;
}
namespace nzgdc_demo
{
    class Camera;
    class ParticleSystem;
    class QuadMVP;
    
    class SceneManager
    {
    public:
        SceneManager();
        void Init(float width, float height);
        void Update(float deltaTime);
        void Render();

        [[nodiscard]] std::shared_ptr<Camera> GetCamera() const;
        [[nodiscard]] std::vector<std::shared_ptr<QuadMVP>> GetQuadsVector() const;
        [[nodiscard]] std::vector<std::shared_ptr<ParticleSystem>> GetParticleSystemsVector() const;

        bool LoadJson(Json::Value& outData) const;
        static void ParseJson(const Json::Value& inJson, SceneManager* outSceneManager);
        
    private:
        void LoadLocalData();

    public:
        inline static std::string settingsPath {"res/assets/SceneManager.json"};
        
    private:
        std::shared_ptr<Camera> m_camera;
        std::vector<std::shared_ptr<QuadMVP>> m_quadMVPs;
        std::vector<std::shared_ptr<ParticleSystem>> m_particleSystems;
    };
}
