#pragma once
#include <memory>

#include "DebugModule/DebugSystem.h"

namespace nzgdc_demo
{
	class ParticleSystemWindow;
}

struct GLFWwindow;

namespace nzgdc_demo
{
	class Camera;
	class QuadMVP;
	class Window;

	class App
	{
	public:
		App();
		~App();

		void Run();
		std::shared_ptr<Window> CreateFluidSimulatorWindow();
		std::weak_ptr<ParticleSystemWindow> CreateParticleSystemWindow();
		
		GLFWwindow* GetMainWindow() const { return m_Window; }

		App(const App&) = delete;
		App& operator=(const App&) = delete; 
		App(App&&) = delete; 
		App& operator=(App&&) = delete;

	private:
		void Update(float deltaTime);
		void Render(float deltaTime);

	private:
		GLFWwindow* m_Window;
		std::shared_ptr<DebugSystem> m_debugSystem;

		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<QuadMVP> m_quadMVP;
		
		float m_currentFrame { 0.0f };
		float m_lastFrame { 0.0f };

		std::vector<std::shared_ptr<Window>> m_windows;
	};
}