#pragma once
#include <memory>

namespace nzgdc_demo
{
    class Quad;

    class QuadEditor
    {
    public:
        QuadEditor(const std::shared_ptr<Quad>& quad);
        void UpdateQuadSettings();
        void Render();

    private:
        std::shared_ptr<Quad> m_quad;
        // ImGui requires a reference& to the data it manipulates.
        // store the reference in this class and upate quad every frame
        float m_position[2] { 0.0f, 0.0f };
        float m_rotation { 0.0f };
        float m_scale[2] { 1.0f, 1.0f };
    };
}
