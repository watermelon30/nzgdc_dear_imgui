#pragma once
#include <memory>

#include "Camera.h"
#include "DebugWindowBase.h"

namespace nzgdc_demo
{
    class CameraEditor : public DebugWindowBase
    {
    public:
        CameraEditor(const std::shared_ptr<Camera>& camera);
        virtual void Render() override;

        void UpdateCameraSettings();

    private:
        std::shared_ptr<Camera> m_camera;

        CameraData m_data;
    };
}
