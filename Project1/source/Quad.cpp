#include "Quad.h"

#include <fstream>

#include "Shader/Shader.h"
#include <glad/glad.h>

#include "json/reader.h"

namespace nzgdc_demo
{
    Quad::Quad(const Shader& shader) :
        m_shader(shader)
    {
        constexpr float vertices[] =
        {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        constexpr unsigned int indices[] =
        {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Reset buffer to 0
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        LoadLocalData();
    }

    void Quad::Render()
    {
        m_shader.Use();

        glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "transform"), 1, GL_FALSE,
                           glm::value_ptr(m_transform.Get()));

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    void Quad::LoadLocalData()
    {
        QuadData quadData;
        Json::Value jsonData;
        if (!LoadJson(jsonData))
        {
            return;
        }
        
        ParseJson(jsonData, quadData);
        SetTransformData(quadData);
    }

    QuadData Quad::GetTransformData() noexcept
    {
        QuadData data;
        data.Position[0] = m_transform.m_position.x;
        data.Position[1] = m_transform.m_position.y;
        data.Rotation = m_transform.m_rotation.z;
        data.Scale[0] = m_transform.m_scale.x;
        data.Scale[1] = m_transform.m_scale.y;
        return data;
    }


    void Quad::SetTransformData(const QuadData& data)
    {
        m_transform.m_position = glm::vec3(data.Position[0], data.Position[1], 0.0f);
        m_transform.m_rotation = glm::vec3(0.0f, 0.0f, data.Rotation);
        m_transform.m_scale = glm::vec3(data.Scale[0], data.Scale[1], 0.0f);
    }
    
    bool Quad::LoadJson(Json::Value& outData) const
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
    
    void Quad::ParseJson(const Json::Value& inJson, QuadData& outQuadData)
    {
        if (inJson.isMember("scale_x") && inJson["scale_x"].isNumeric())
        {
            outQuadData.Scale[0] = inJson["scale_x"].asFloat();
        }
        if (inJson.isMember("scale_y") && inJson["scale_y"].isNumeric())
        {
            outQuadData.Scale[1] = inJson["scale_y"].asFloat();
        }
        if (inJson.isMember("rotation") && inJson["rotation"].isNumeric())
        {
            outQuadData.Rotation = inJson["rotation"].asFloat();
        }
        if (inJson.isMember("position_x") && inJson["position_x"].isNumeric())
        {
            outQuadData.Position[0] = inJson["position_x"].asFloat();
        }
        if (inJson.isMember("position_y") && inJson["position_y"].isNumeric())
        {
            outQuadData.Position[1] = inJson["position_y"].asFloat();
        }
    }
}
