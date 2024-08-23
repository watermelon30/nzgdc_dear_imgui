#include "FluidSimulator.h"

#include "Shader.h"
#include "glad/glad.h"

namespace nzgdc_demo
{
	FluidSimulator::FluidSimulator()
	{
		Init();
	}

	void FluidSimulator::Init()
	{
		m_shader = std::make_shared<Shader>("res/shaders/fluid.vs", "res/shaders/fluid.frag");
		
		float vertices[] = {
			0.0f,  0.0f
	   };

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void FluidSimulator::Update(float deltaTime)
	{
		// Update logic here
	}

	void FluidSimulator::Render(float deltaTime)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(200.0f);
		
		m_shader->Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
	}
}
