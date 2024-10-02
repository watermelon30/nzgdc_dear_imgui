#pragma once
#include <memory>

#include "DebugModule/DebugSystem.h"

struct GLFWwindow;

namespace nzgdc_demo
{
	class SceneManager;
	class ParticleSystem;
	class Camera;
	class QuadMVP;
	class Quad;
	class Window;

	class App
	{
	public:
		App();
		~App();

		void Run();
		std::shared_ptr<Window> CreateFluidSimulatorWindow();
		[[nodiscard]] GLFWwindow* GetMainWindow() const { return m_Window; }

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
		std::shared_ptr<SceneManager> m_sceneManager;
		//std::shared_ptr<Quad> m_quad;
		//std::shared_ptr<QuadMVP> m_quadMVP;
		// td::shared_ptr<ParticleSystem> m_particleSystem;
		
		float m_currentFrame { 0.0f };
		float m_lastFrame { 0.0f };

		std::vector<std::shared_ptr<Window>> m_windows;
	};
}