#include "JsonHelper.h"

#include <fstream>

#include "json/reader.h"
#include "json/writer.h"

bool nzgdc_demo::JsonHelper::LoadJson(const std::string& targetPath, Json::Value& outData)
{
	std::ifstream file(targetPath);
	if (!file.is_open())
	{
		// TODO: Log error (Failed to open the file)
		return false;
	}

	Json::CharReaderBuilder readerBuilder;
	std::string errors;

	if (!Json::parseFromStream(readerBuilder, file, &outData, &errors))
	{
		// TODO: Log error (Failed to parse JSON)
		return false;
	}
	return true;
}
bool nzgdc_demo::JsonHelper::SaveToJson(const std::string& targetPath, const Json::Value& targetJson)
{
	std::ofstream file(targetPath);

	if (!file.is_open())
	{
		// TODO: Log error
		return false;
	}
	Json::StreamWriterBuilder writerBuilder;
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	writer->write(targetJson, &file);
	return true;
}
