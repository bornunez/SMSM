#include "JsonParser.h"
#include <fstream>
#include <iostream>
#include <streambuf>
#include <experimental/filesystem>





JsonParser::JsonParser()
{
}


JsonParser::~JsonParser()
{
}

json JsonParser::ParseJsonFile(string filePath)
{
	if (doesFileExist(filePath)) {

		// Read file to a string
		ifstream f(filePath);
		string fileString((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

		// Parse json
		json parsedJson = json::parse(fileString);

		return parsedJson;
	}
	else
	{
		throw std::invalid_argument("File does not exist in the specified path");
	}
}

bool JsonParser::doesFileExist(string filePath)
{
	return std::experimental::filesystem::exists(filePath);
}
