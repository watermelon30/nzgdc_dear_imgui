#pragma once

namespace nzgdc_demo
{
	class DebugWindowBase
	{
	public:
		DebugWindowBase() = default;
		virtual ~DebugWindowBase() = default;
		virtual void Render() = 0;
	};
}
