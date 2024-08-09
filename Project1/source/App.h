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

	private:
		GLFWwindow* m_Window;
	};
}