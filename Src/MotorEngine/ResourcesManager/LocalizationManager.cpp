#include "LocalizationManager.h"
#include <fstream>
#include <streambuf>
#include "JsonParser.h"


LocalizationManager::LocalizationManager() : rootFolder_("")
{
}

LocalizationManager::~LocalizationManager()
{
}

void LocalizationManager::Init(string rootFolder)
{
	rootFolder_ = rootFolder;
}

// Returns true if the language was successfully added (it fails when its already added)
bool LocalizationManager::RegisterLanguage(string language_id, string language_termination)
{
	auto it = languages_.find(language_id);
	if (it == languages_.end())
	{
		// Language not found, add it
		languages_[language_id] = language_termination;
		return true;
	}
	else // Language already added, dont add it again
		return false;
}

// Returns wheter the language you are trying to select exists or not
bool LocalizationManager::SelectLanguage(string language_id)
{
	auto it = languages_.find(language_id);
	if (it != languages_.end())
	{
		// Language found, select it
		selectedLanguage_.id = (*it).first;
		selectedLanguage_.fileTermination = (*it).second;
		return true;
	}
	else // Language not found
		return false;
}

// Adds a fileName to the list of files that are going to be parsed
void LocalizationManager::RegisterFile(string fileName)
{
	bool found = (find(fileNames_.begin(), fileNames_.end(), fileName) != fileNames_.end());

	if (!found)
		fileNames_.push_back(fileName);
}

void LocalizationManager::LoadFromFiles()
{
	for(pair<string, string> l: languages_)
	{
		for (string fName : fileNames_) {
			// Parse json in 'rootFolder' with name 'fName' and fileTermination 'l.second'
			string name = rootFolder_ + fName + l.second + ".json";

			json parsedJson = JsonParser::ParseJsonFile(name);

			for (auto& el : parsedJson.items()) {
				localizatedData_[l.first][el.key()] = el.value();
			}
		}
	}
}

// Returns the localizated string that corresponds with this id
string LocalizationManager::GetString(string string_id)
{
	return localizatedData_[selectedLanguage_.id][string_id];
}

