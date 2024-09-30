#pragma once
#include <memory>
#include "DebugSystem.h"

struct GLFWwindow;

namespace nzgdc_demo
{
	class Camera;
	class QuadMVP;
	class Quad;

	class App
	{
	public:
		App();
		~App();

		void Run();

		App(const App&) = delete;
		App& operator=(const App&) = delete; 
		App(App&&) = delete; 
		App& operator=(App&&) = delete;

	private:
		void Update(float deltaTime);
		void Render(float deltaTime);
		void HandleFluidSimulator(float deltaTime);

	private:
		GLFWwindow* m_Window;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<Quad> m_quad;
		std::shared_ptr<QuadMVP> m_quadMVP;

		std::shared_ptr<DebugSystem> m_debugSystem;

		float m_currentFrame { 0.0f };
		float m_lastFrame { 0.0f };
	};
}