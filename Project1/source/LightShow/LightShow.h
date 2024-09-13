#pragma once

namespace nzgdc_demo
{
	struct LightShowData
	{
	};

	class LightShow
	{
	public:
		explicit LightShow(const LightShowData& _data);

		void Activate();
		void Render();

		LightShow(const LightShow&) = delete;
		LightShow& operator=(const LightShow&) = delete;
		LightShow(LightShow&&) = delete;
		LightShow& operator=(LightShow&&) = delete;

	private:
		void RenderImGui();
	};
}