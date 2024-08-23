#pragma once
#include <memory>

#include "glad/glad.h"

namespace nzgdc_demo
{
	class Shader;
	class Window;

	class FluidSimulator
	{
	public:
		FluidSimulator();

		void Init();
		void Update(float deltaTime);
		void Render(float deltaTime);

	private:
		std::shared_ptr<Shader> m_shader;
		bool m_bShow = false;
		GLuint VBO, VAO;
	};
}
