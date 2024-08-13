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
        void RenderContent() override;
        std::string GetWindowId() const override;
        void UpdateCameraSettings();

    private:
        const char* m_projectionOptions[2] = { "ORTHOGRAPHIC", "PERSPECTIVE" };
        
        std::shared_ptr<Camera> m_camera;

        CameraData m_data;
        int m_selectedProjectionIndex { 0 };
        const char* m_currentSelectedProjection = m_projectionOptions[0];
    };
}
