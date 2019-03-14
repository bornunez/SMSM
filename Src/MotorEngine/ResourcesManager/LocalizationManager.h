
#pragma once
#include <cstdlib>
#include <string>
#include <map>
#include <iostream>
#include <list>

// Json --------------------------
#include "json.hpp"
using json = nlohmann::json;
// -------------------------------

using namespace std;

// ------ Initialization ---------
// 1 -> Register Languages
// 2 -> Register Filenames
// 3 -> LoadFromFiles
//
// -> SelectLanguage
// -> GetString
// -------------------------------

struct Language
{
	string id;
	string fileTermination;
};

class LocalizationManager 
{
public:
	LocalizationManager();
	~LocalizationManager();

	void Init(string rootFolder);

	bool RegisterLanguage(string language_id, string language_termination);
	bool SelectLanguage(string language_id);

	void RegisterFile(string fileName);
	void LoadFromFiles();

	string GetString(string string_id);

private:

	// Folder where the language files are located
	string rootFolder_;

	// Current language selected
	Language selectedLanguage_;

	// List of files that are going to be parsed
	std::list<string> fileNames_;

	// Dictionary of Languages <id, folderTermination>
	std::map<string, string> languages_;

	// Dictionary of parsed data <language_id <text_id, text>>
	std::map<string, std::map<string, string>> localizatedData_;
};