#pragma once
#include <string>

namespace Json
{
	class Value;
}
namespace nzgdc_demo
{
	class JsonHelper
	{
	public:
		static bool LoadJson(const std::string& targetPath, Json::Value& outData);
		static bool SaveToJson(const std::string& targetPath, const Json::Value& targetJson);

	};
}
