#pragma once
#include <memory>
#include <vector>


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

    private:
        std::shared_ptr<Camera> m_camera;
        std::vector<std::shared_ptr<QuadMVP>> m_quadMVPs;
        std::vector<std::shared_ptr<ParticleSystem>> m_particleSystems;
    };
}
