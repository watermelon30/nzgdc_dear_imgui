#pragma once
#include "Quad.h"

namespace nzgdc_demo
{
	class QuadMVP : public Quad
	{
	public:
		QuadMVP(const Shader& shader)
		: Quad(shader)
		{ }
		
		virtual void Render() const override;

		void SetView(const glm::mat4& view) const;
		void SetProjection(const glm::mat4& projection) const;
	};
}
