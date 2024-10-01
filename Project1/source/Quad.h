#pragma once

#include <string>

#include "Transform.h"

#include "json/value.h"

namespace nzgdc_demo
{
    class Shader;

    struct QuadData
    {
        float Position[2]{0.0f, 0.0f};
        float Rotation{0.0f};
        float Scale[2]{1.0f, 1.0f};
    };

    class Quad
    {
    public:
        explicit Quad(const Shader& shader);
        virtual ~Quad() = default;

        virtual void Render();
        [[nodiscard]] virtual std::string GetSettingsPath() const;

        QuadData GetTransformData() noexcept;
        void SetTransformData(const QuadData& data);

        bool LoadJson(Json::Value& outData) const;
        void ParseJson(const Json::Value& inJson, QuadData& outQuadData);

    protected:
        void LoadLocalData();

    protected:
        const Shader& m_shader;
        unsigned int m_vao, m_vbo, m_ebo;
        Transform m_transform;
    };
}
