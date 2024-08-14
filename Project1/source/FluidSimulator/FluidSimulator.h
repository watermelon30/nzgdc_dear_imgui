#pragma once
#include <memory>

#include "Window/Window.h"

namespace nzgdc_demo
{
	class Window;

	class FluidSimulator
	{
	public:
		static FluidSimulator& Get()
		{
			static FluidSimulator instance;
			return instance;
		}

		FluidSimulator();

		void Show(bool bNewShow);
		void Update(float deltaTime);
		void Render(float deltaTime);

		GLFWwindow* m_share = NULL;

	private:
		bool m_bShow = false;
		std::shared_ptr<Window> m_window;
	};
}
