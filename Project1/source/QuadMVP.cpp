#include "QuadMVP.h"

#include "Shader/Shader.h"
#include "glad/glad.h"

#include "json/reader.h"

QuadMvpData nzgdc_demo::QuadMVP::GetQuadMvpData() noexcept
{
    QuadMvpData data;
    data.quadData = GetTransformData();
    data.texturePath = m_texturePath;
    return data;
}

void nzgdc_demo::QuadMVP::SetQuadMvpData(const QuadMvpData& data)
{
    SetTransformData(data.quadData);
    m_texture.SetTexturePath(data.texturePath);
}

void nzgdc_demo::QuadMVP::Render()
{
    m_shader.Use();
    m_texture.Bind();

    glUniform1i(glGetUniformLocation(m_shader.GetId(), "texture_sampler"), 0);
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "model"), 1, GL_FALSE, glm::value_ptr(m_transform.Get()));

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void nzgdc_demo::QuadMVP::SetView(const glm::mat4& view) const
{
    m_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void nzgdc_demo::QuadMVP::SetProjection(const glm::mat4& projection) const
{
    m_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void nzgdc_demo::QuadMVP::ParseJson(const Json::Value& inJson, QuadMvpData& outQuadMvpData)
{
    Quad::ParseJson(inJson, outQuadMvpData.quadData);
    if (inJson.isMember("texture_path") && inJson["texture_path"].isString())
    {
        outQuadMvpData.texturePath = inJson["texture_path"].asString();
    }
}
