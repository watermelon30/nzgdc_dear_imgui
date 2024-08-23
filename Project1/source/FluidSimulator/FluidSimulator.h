#pragma once
#include <memory>

#include "Shader.h"
#include "vec2.hpp"
#include "glad/glad.h"
#include "Window/Window.h"

namespace nzgdc_demo
{
	class ComputeShader;
}

namespace nzgdc_demo
{
	class Window;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	const int SCALE_FACTOR = 8;
	const int GRID_SIZE_X = WINDOW_WIDTH / SCALE_FACTOR;
	const int GRID_SIZE_Y = WINDOW_HEIGHT / SCALE_FACTOR;

	const float TIME_STEP = 0.01f;
	const int NUM_PARTICLES = 3000;
	const float RESTITUTION = 0.8f;

	const float BOX_WIDTH = 200.0f;
	const float BOX_HEIGHT = 200.0f;
	const glm::vec2 BOX_CENTER(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

	const glm::vec2 GRAVITY(0.0f, -9.81f);

	const float POINT_SIZE = 10.f;

	// Particle structure
	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
	};

	// Grid structure for PIC/FLIP
	struct Grid
	{
		glm::vec2 velocities[GRID_SIZE_X][GRID_SIZE_Y];
		float pressures[GRID_SIZE_X][GRID_SIZE_Y];

		Grid()
		{
			for (int i = 0; i < GRID_SIZE_X; ++i)
			{
				for (int j = 0; j < GRID_SIZE_Y; ++j)
				{
					velocities[i][j] = glm::vec2(0.0f, 0.0f);
					pressures[i][j] = 0.0f;
				}
			}
		}

		glm::vec2 interpolateVelocity(const glm::vec2& position)
		{
			// Bilinear interpolation of velocity at particle position
			int x0 = static_cast<int>(position.x);
			int y0 = static_cast<int>(position.y);

			int x1 = x0 + 1;
			int y1 = y0 + 1;

			float sx = position.x - x0;
			float sy = position.y - y0;

			glm::vec2 v00 = velocities[x0][y0];
			glm::vec2 v01 = velocities[x0][y1];
			glm::vec2 v10 = velocities[x1][y0];
			glm::vec2 v11 = velocities[x1][y1];

			return (1.0f - sx) * ((1.0f - sy) * v00 + sy * v01) +
				sx * ((1.0f - sy) * v10 + sy * v11);
		}

		void addParticleVelocity(const glm::vec2& position, const glm::vec2& velocity)
		{
			// Add velocity to the grid using a weighted sum
			int x = static_cast<int>(position.x);
			int y = static_cast<int>(position.y);

			velocities[x][y] += velocity;
		}

		void applyForces(float dt)
		{
			// Apply gravity
			glm::vec2 gravity(0.0f, -9.81f * dt);
			for (int i = 0; i < GRID_SIZE_X; ++i)
			{
				for (int j = 0; j < GRID_SIZE_Y; ++j)
				{
					velocities[i][j] += gravity;
				}
			}
		}

		void updateVelocities()
		{
			// Boundary conditions: zero velocity on boundaries
			for (int i = 0; i < GRID_SIZE_X; ++i)
			{
				velocities[i][0] = glm::vec2(0.0f, 0.0f);
				velocities[i][GRID_SIZE_Y - 1] = glm::vec2(0.0f, 0.0f);
			}
			for (int j = 0; j < GRID_SIZE_Y; ++j)
			{
				velocities[0][j] = glm::vec2(0.0f, 0.0f);
				velocities[GRID_SIZE_X - 1][j] = glm::vec2(0.0f, 0.0f);
			}
		}
	};

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

		void Init();
		void InitGPU();

	private:
		void InitParticles();
		void InitParticlesGPU();
		void RenderParticles();

		void StepGPU();

	private:
		std::shared_ptr<Shader> m_renderShader;

		std::shared_ptr<ComputeShader> m_advectShader;
		std::shared_ptr<ComputeShader> m_transferToGridShader;
		std::shared_ptr<ComputeShader> m_applyForcesShader;
		std::shared_ptr<ComputeShader> m_solvePressureShader;
		std::shared_ptr<ComputeShader> m_projectVelocitiesShader;
		std::shared_ptr<ComputeShader> m_transferToParticlesShader;

		bool m_bShow = false;
		std::shared_ptr<Window> m_window;

		unsigned int VAO, VBO;
		std::vector<Particle> particles;
		Grid grid;

		// GPU
		GLuint particleSSBO;
		GLuint gridSSBO;
	};
}
