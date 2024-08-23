#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace nzgdc_demo
{
    class Transform
    {
    public:
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_rotation = glm::vec3(0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);

        glm::mat4& Get()
        {
            // Translation
            glm::mat4 translationMatrix{glm::translate(glm::mat4(1.0f), m_position)};
            
            // Rotation
            glm::mat4 rotationXMatrix{glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0))};
            glm::mat4 rotationYMatrix{glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, 1, 0))};
            glm::mat4 rotationZMatrix{glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, 1))};
            glm::mat4 rotationMatrix{rotationZMatrix * rotationYMatrix * rotationXMatrix};
            
            // Scale
            glm::mat4 scaleMatrix{ glm::scale(glm::mat4(1.0f), m_scale)};

            // Combine all the matrices: Translation * Rotation * Scale
            m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
            
            return m_transformMatrix;
        }

    private:
        glm::mat4 m_transformMatrix{};
    };
}
