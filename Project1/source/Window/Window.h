#pragma once
#include <functional>
#include <string>
#include <glfw3.h>

namespace nzgdc_demo
{
    class Window
    {
    public:
        Window(int windowWidth = 800, int windowHeight = 600, const std::string& windowTitle = "NZGDC_24");
        virtual ~Window();

        virtual void Init(GLFWwindow* share = NULL);
        void Destroy();

        void Update(float deltaTime);
        void Render(float deltaTime);

        bool IsValid() const;
        GLFWwindow* GetGLFWwindow() const;

        std::function<void(GLFWwindow*)> OnWindowShouldClose;

        int m_windowWidth;
        int m_windowHeight;
        std::string m_windowTitle;

    protected:
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender(float deltaTime) = 0;

    private:
        static void WindowCloseCallback(GLFWwindow* window);
        GLFWwindow* m_window = nullptr;
    };
}
