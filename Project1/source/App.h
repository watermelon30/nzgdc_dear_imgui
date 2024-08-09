#pragma once

struct GLFWwindow;

namespace nzgdc_demo
{
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
		GLFWwindow* m_Window;
	};
}