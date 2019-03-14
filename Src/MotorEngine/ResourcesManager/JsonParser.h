#pragma once
#include <string>

// Json --------------------------
#include "json.hpp"
using json = nlohmann::json;
// -------------------------------

using namespace std;

class JsonParser
{
public:
	JsonParser();
	~JsonParser();

	static json ParseJsonFile(string filePath);

private:
	static bool doesFileExist(string filePath);
};

