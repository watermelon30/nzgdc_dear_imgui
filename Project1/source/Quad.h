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

        QuadData GetTransformData() noexcept;
        void SetTransformData(const QuadData& data);

        bool LoadJson(Json::Value& outData) const;
        static void ParseJson(const Json::Value& inJson, QuadData& outQuadData);

    protected:
        void LoadLocalData();

    public:
        inline static std::string settingsPath {"res/assets/Quad.json"}; 

    protected:
        const Shader& m_shader;
        unsigned int m_vao, m_vbo, m_ebo;
        Transform m_transform;
    };
}
