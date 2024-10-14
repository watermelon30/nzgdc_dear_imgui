#pragma once

#include "Quad.h"
#include "Texture.h"

struct QuadMvpData
{
	nzgdc_demo::QuadData quadData;
	std::string texturePath;
};

namespace nzgdc_demo
{
	class QuadMVP : public Quad
	{
	public:
		explicit QuadMVP(const Shader& shader, const std::string_view texturePath = "") :
			Quad(shader),
			m_texture(texturePath)
		{}

		QuadMvpData GetQuadMvpData() noexcept;
		void SetQuadMvpData(const QuadMvpData& data);
		void Render() override;
		void SetView(const glm::mat4& view) const;
		void SetProjection(const glm::mat4& projection) const;

		static void ParseJson(const Json::Value& inJson, QuadMvpData& outQuadMvpData);

		inline static std::string settingsPath {"res/assets/QuadMvp.json"}; 

	private:
		Texture m_texture;
		std::string m_texturePath;
	};
}