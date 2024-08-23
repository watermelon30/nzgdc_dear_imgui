#include "FluidSimulator.h"

#include <glfw3.h>
#include <random>

#include "geometric.hpp"
#include "glad/glad.h"
#include "Shader/ComputeShader.h"
#include "Window/Window.h"

namespace nzgdc_demo
{
	void FluidSimulator::Show(bool bNewShow)
	{
		if (m_bShow == bNewShow)
		{
			return;
		}
		m_bShow = bNewShow;
		if (!m_window->IsValid())
		{
			// Lazy init
			m_window->Init(m_share);
			Init();
			InitGPU();
		}
		if (m_bShow)
		{
			m_window->Show();
		}
		else
		{
			m_window->Hide();
		}
	}

	FluidSimulator::FluidSimulator()
	{
		m_window = std::make_shared<Window>(800, 600, "FluidSimulator");
		m_window->OnWindowShouldClose = [](GLFWwindow* window)
		{
			Get().Show(false);
			glfwSetWindowShouldClose(window, false);
		};
	}

	void FluidSimulator::Update(float deltaTime)
	{
		if (m_bShow)
		{
			// Step(deltaTime);
			StepGPU();
			Render(deltaTime);
		}
	}

	void FluidSimulator::Render(float deltaTime)
	{
		m_window->Use();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(POINT_SIZE);

		RenderParticles();

		m_window->Present();
	}

	void FluidSimulator::Init()
	{
		m_window->Use();
		m_renderShader = std::make_shared<Shader>("res/shaders/fluid/fluid.vert", "res/shaders/fluid/fluid.frag");

		InitParticles();
	}

	void FluidSimulator::InitGPU()
	{
		m_window->Use();

		m_renderShader = std::make_shared<Shader>("res/shaders/fluid/fluid.vert", "res/shaders/fluid/fluid.frag");

		m_advectShader = std::make_shared<ComputeShader>("res/shaders/fluid/advectParticles.comp");
		m_transferToGridShader = std::make_shared<ComputeShader>("res/shaders/fluid/transferParticlesToGrid.comp");
		m_applyForcesShader = std::make_shared<ComputeShader>("res/shaders/fluid/applyForces.comp");
		m_solvePressureShader = std::make_shared<ComputeShader>("res/shaders/fluid/solvePressure.comp");
		m_projectVelocitiesShader = std::make_shared<ComputeShader>("res/shaders/fluid/projectVelocities.comp");
		m_transferToParticlesShader = std::make_shared<ComputeShader>("res/shaders/fluid/transferGridToParticles.comp");

		InitParticlesGPU();
	}

	void FluidSimulator::InitParticles()
	{
		particles.clear(); // Clear any existing particles
		particles.reserve(NUM_PARTICLES); // Reserve space for NUM_PARTICLES

		// Initialize random number generators for X and Y positions
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> randomX(BOX_CENTER.x - BOX_WIDTH / 2, BOX_CENTER.x + BOX_WIDTH / 2);
		std::uniform_real_distribution<float> randomY(BOX_CENTER.y - BOX_HEIGHT / 2, BOX_CENTER.y + BOX_HEIGHT / 2);

		for (int i = 0; i < NUM_PARTICLES; ++i)
		{
			Particle p;
			p.position = glm::vec2(randomX(rng), randomY(rng)); // Random position within the box
			p.velocity = glm::vec2(0.0f, -0.05f); // Initial velocity is zero
			particles.push_back(p);
		}

		// 
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * NUM_PARTICLES, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void FluidSimulator::InitParticlesGPU()
	{
		// Create and bind the SSBO for particles
		glGenBuffers(1, &particleSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
		// What does this do?
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Particle), particles.data(), GL_DYNAMIC_COPY);
		// What does this do?
		// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
		// TODO: https://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
		// https://ktstephano.github.io/rendering/opengl/ssbos
		
		// Create and bind the SSBO for grid
		glGenBuffers(1, &gridSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, gridSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, GRID_SIZE_X * GRID_SIZE_Y * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, gridSSBO);
	}

	void FluidSimulator::RenderParticles()
	{
		std::vector<float> particlePositions;
		particlePositions.reserve(particles.size() * 2);

		for (const auto& p : particles)
		{
			float normalizedX = (p.position.x / WINDOW_WIDTH) * 2.0f - 1.0f;
			float normalizedY = (p.position.y / WINDOW_HEIGHT) * 2.0f - 1.0f;

			particlePositions.push_back(normalizedX);
			particlePositions.push_back(normalizedY);
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlePositions.size() * sizeof(float), particlePositions.data());

		m_renderShader->Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, particles.size());

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void FluidSimulator::StepGPU()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
		// Step 1: Advection
		m_advectShader->Use();
		glUniform1f(glGetUniformLocation(m_advectShader->GetId(), "dt"), TIME_STEP);
		glUniform2f(glGetUniformLocation(m_advectShader->GetId(), "gridSize"), GRID_SIZE_X, GRID_SIZE_Y);
		glUniform1f(glGetUniformLocation(m_advectShader->GetId(), "restitution"), RESTITUTION);
		glDispatchCompute((NUM_PARTICLES + 255) / 256, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		
		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(p, particles.data(), NUM_PARTICLES * sizeof(Particle));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		// Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		
		return;

		// Step 2: Transfer particle velocities to the grid
		m_transferToGridShader->Use();
		glUniform1i(glGetUniformLocation(m_transferToGridShader->GetId(), "gridWidth"), GRID_SIZE_X);
		glUniform1i(glGetUniformLocation(m_transferToGridShader->GetId(), "gridHeight"), GRID_SIZE_Y);
		glDispatchCompute((NUM_PARTICLES + 255) / 256, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Step 3: Apply forces to the grid
		m_applyForcesShader->Use();
		glUniform2f(glGetUniformLocation(m_applyForcesShader->GetId(), "gravity"), GRAVITY.x, GRAVITY.y);
		glUniform1f(glGetUniformLocation(m_applyForcesShader->GetId(), "dt"), TIME_STEP);
		glUniform1i(glGetUniformLocation(m_applyForcesShader->GetId(), "gridWidth"), GRID_SIZE_X);
		glUniform1i(glGetUniformLocation(m_applyForcesShader->GetId(), "gridHeight"), GRID_SIZE_Y);
		glDispatchCompute((GRID_SIZE_X + 15) / 16, (GRID_SIZE_Y + 15) / 16, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Step 4: Solve pressure
		m_solvePressureShader->Use();
		glUniform1i(glGetUniformLocation(m_solvePressureShader->GetId(), "gridWidth"), GRID_SIZE_X);
		glUniform1i(glGetUniformLocation(m_solvePressureShader->GetId(), "gridHeight"), GRID_SIZE_Y);
		glDispatchCompute((GRID_SIZE_X + 15) / 16, (GRID_SIZE_Y + 15) / 16, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Step 5: Project velocities
		m_projectVelocitiesShader->Use();
		glUniform1i(glGetUniformLocation(m_projectVelocitiesShader->GetId(), "gridWidth"), GRID_SIZE_X);
		glUniform1i(glGetUniformLocation(m_projectVelocitiesShader->GetId(), "gridHeight"), GRID_SIZE_Y);
		glDispatchCompute((GRID_SIZE_X + 15) / 16, (GRID_SIZE_Y + 15) / 16, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Step 6: Transfer updated grid velocities back to particles
		m_transferToParticlesShader->Use();
		glUseProgram(m_transferToParticlesShader->GetId());
		glUniform1i(glGetUniformLocation(m_transferToParticlesShader->GetId(), "gridWidth"), GRID_SIZE_X);
		glUniform1i(glGetUniformLocation(m_transferToParticlesShader->GetId(), "gridHeight"), GRID_SIZE_Y);
		glUniform1f(glGetUniformLocation(m_transferToParticlesShader->GetId(), "blendFactor"), 0.05f);
		glDispatchCompute((NUM_PARTICLES + 255) / 256, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}
