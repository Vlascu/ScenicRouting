#pragma once
#include <nlohmann/json.hpp>

#include "CommonAliases.h"

using Json = nlohmann::json;

class JsonHandler
{
public:
	static std::vector<char*> GetElementsArray(char* data, const char* tag)
	{
		Json json = Json::parse((char*)data);
		std::vector<char*> elements;

		for (auto& element : json[tag])
		{
			std::string elementString = element.dump();
			char* buffer = new char[elementString.size() + 1];
			std::memcpy(buffer, elementString.c_str(), elementString.size() + 1);

			elements.push_back(buffer);
		}

		return elements;
	}

	template < class T>
	static T GetElement(char* data, const char* tag)
	{
		Json json = Json::parse(data);

		return json[tag].get<T>();
	}

	static bool HasElement(char* data, const char* tag)
	{
		Json json = Json::parse(data);

		return json.contains(tag);
	}

	static Tags GetTags(char* data)
	{
		Json json = Json::parse(data);
		Tags tags;

		if (json.contains("tags"))
		{
			for (auto& [key, value] : json["tags"].items()) {
				char* keyBuffer = new char[key.size() + 1];
				std::memcpy(keyBuffer, key.c_str(), key.size() + 1);

				std::string valueStr = value.is_string() ? value.get<std::string>() : value.dump();
				char* valueBuffer = new char[valueStr.size() + 1];
				std::memcpy(valueBuffer, valueStr.c_str(), valueStr.size() + 1);

				tags[keyBuffer] = valueBuffer;
			}
		}

		return tags;
	}

	static std::vector<long long > GetNodesIds(char* data)
	{
		Json json = Json::parse(data);

		Json nodes = json["nodes"];

		std::vector<long long> ids;

		for (auto& id : nodes)
		{
			ids.push_back(id.get<long long>());
		}

		return ids;
	}

	static std::vector<double> GetRowDurations(char* data)
	{
		std::vector<double> rowDurations;
		Json json = Json::parse(data);

		for (const auto& duration : json)
		{
			if (duration.is_number())
			{
				rowDurations.push_back(duration.get<double>());
			}
		}

		return rowDurations;
	}
};