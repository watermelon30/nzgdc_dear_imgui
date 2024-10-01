#pragma once

#include "Quad.h"
#include "Texture.h"

namespace nzgdc_demo
{
	class QuadMVP : public Quad
	{
	public:
		explicit QuadMVP(const Shader& shader, const std::string_view texturePath = "") :
			Quad(shader),
			m_texture(texturePath)
		{}

		virtual void Render() override;

		void SetView(const glm::mat4& view) const;
		void SetProjection(const glm::mat4& projection) const;

	private:
		Texture m_texture;
	};
}