#include "FluidSimulator.h"

#include <glfw3.h>
#include <iostream>
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
		m_window = std::make_shared<Window>(WINDOW_WIDTH , WINDOW_HEIGHT, "FluidSimulator");
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

		InitBuffers();
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
	}

	void FluidSimulator::InitBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &positionSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionSSBO);
		InitPosition();
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(glm::vec2), positions.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glGenBuffers(1, &velocitySSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW);
		ResetVelocitySSBO();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocitySSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
		glBindVertexArray(0);
	}

	void FluidSimulator::RenderParticles()
	{
		m_renderShader->Use();
		glBindVertexArray(VAO);
		glUniform1ui(glGetUniformLocation(m_renderShader->GetId(), "windowWidth"), WINDOW_WIDTH);
		glUniform1ui(glGetUniformLocation(m_renderShader->GetId(), "windowHeight"), WINDOW_HEIGHT);
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glUseProgram(0);
	}

	void FluidSimulator::StepGPU()
	{
		// Step 1: Advection
		m_advectShader->Use();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocitySSBO);
		glUniform1f(glGetUniformLocation(m_advectShader->GetId(), "dt"), TIME_STEP);
		glUniform2f(glGetUniformLocation(m_advectShader->GetId(), "gridSize"), GRID_SIZE_X, GRID_SIZE_Y);
		glUniform2f(glGetUniformLocation(m_advectShader->GetId(), "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
		glUniform1f(glGetUniformLocation(m_advectShader->GetId(), "restitution"), RESTITUTION);
		glUniform1ui(glGetUniformLocation(m_advectShader->GetId(), "numParticles"), NUM_PARTICLES);
		int numWorkgroups = (NUM_PARTICLES + local_size_x - 1) / local_size_x;
		glDispatchCompute(numWorkgroups, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		
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

	void FluidSimulator::InitPosition()
	{
		// Initialize random number generators for X and Y positions
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> randomX(BOX_CENTER.x - BOX_WIDTH / 2, BOX_CENTER.x + BOX_WIDTH / 2);
		std::uniform_real_distribution<float> randomY(BOX_CENTER.y - BOX_HEIGHT / 2, BOX_CENTER.y + BOX_HEIGHT / 2);

		for (int i = 0; i < NUM_PARTICLES ; i++) {
			positions.push_back(glm::vec2(randomX(rng), randomY(rng))); // Random position within the box
		}
	}

	void FluidSimulator::ResetVelocitySSBO()
	{
		glm::vec2* velocities = (glm::vec2*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(glm::vec2), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		for (int i = 0; i < NUM_PARTICLES ; i++) {
			velocities[i] = glm::vec2(1000.0f, -1000.f); // Initial velocity
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}
