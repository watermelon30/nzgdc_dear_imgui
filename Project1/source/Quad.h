#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace nzgdc_demo
{
    class Shader;

    struct Transform
    {
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);

        glm::mat4 Get() const
        {
            // Translation
            const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
            // Rotation
            const glm::mat4 rotationXMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1, 0, 0));
            const glm::mat4 rotationYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0, 1, 0));
            const glm::mat4 rotationZMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0, 0, 1));
            const glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
            // Scale
            const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

            // Combine all the matrices: Translation * Rotation * Scale
            const glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

            return transformMatrix;
        }
    };

    class Quad
    {
    public:
        Quad(const Shader& shader);
        virtual ~Quad() = default;

        virtual void Render() const;
        Transform& GetTransform() { return m_transform; }


    protected:
        const Shader& m_shader;
        unsigned int m_vao, m_vbo, m_ebo;
        Transform m_transform;
    };
}
