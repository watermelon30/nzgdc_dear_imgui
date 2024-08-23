#pragma once

#include <glm.hpp>
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"

namespace nzgdc_demo
{
    enum class ProjectionType : unsigned char
    {
        Orthographic,
        Perspective
    };

    struct CameraData
    {
        CameraData() noexcept = default;

        CameraData(ProjectionType proj, const float w, const float h, const glm::vec3& pos) noexcept :
            Projection(proj),
            Width(w),
            Height(h),
            Position(pos)
        {}

        ProjectionType Projection{ProjectionType::Perspective};

        float Pitch{0.0f};
        float Yaw{-90.0f};

        float NearPlane{0.1f};
        float FarPlane{1000.0f};
        // Perspective Projection
        float FieldOfView{45.0f};
        float AspectRatio{16.0f / 9.0f};
        // Orthographic Projection
        float Width{800.0f};
        float Height{600.0f};

        glm::vec3 Position{glm::vec3(0.0f)};
        glm::vec3 Front{glm::vec3(0.0f, 0.0f, -1.0f)};
        glm::vec3 Up{glm::vec3(0.0f, 1.0f, 0.0f)};
        glm::vec3 Right{glm::vec3(1.0f, 0.0f, 0.0f)};
        glm::vec3 WorldUp{glm::vec3(0.0f, 1.0f, 0.0f)};
    };

    class Camera
    {
    public:
        explicit Camera(const CameraData& data) noexcept :
            Data(data)
        {}

        glm::mat4 GetView() const
        {
            return glm::lookAt(Data.Position, Data.Position + Data.Front, Data.Up);
        }

        glm::mat4 GetProjection() const
        {
            if (Data.Projection == ProjectionType::Perspective)
            {
                return glm::perspective(glm::radians(Data.FieldOfView), Data.AspectRatio, Data.NearPlane,
                                        Data.FarPlane);
            }

            // center is 0,0
            const float halfWidth{Data.Width * 0.5f};
            const float halfHeight{Data.Height * 0.5f};
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, Data.NearPlane, Data.FarPlane);
        }

    public:
        CameraData Data;
    };
}
