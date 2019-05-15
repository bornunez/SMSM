#include "GameSceneManager.h"
#include <filesystem>

GameSceneManager::GameSceneManager(Game* g, string scenesFolder) : game(g), rootFolder_(scenesFolder)
{
}

GameSceneManager::~GameSceneManager() {

	for (auto current = scenes_.begin(); current != scenes_.end(); ++current) {
		delete current->second;
	}
	scenes_.clear();
}

struct pathToString
{
	std::string operator()(const std::experimental::filesystem::directory_entry& entry) const
	{
		return entry.path().filename().string();
	}
};

void readDirectoryFileNames(const std::string& name, std::vector<string>& v)
{
	std::experimental::filesystem::path p(name);
	std::experimental::filesystem::directory_iterator start(p);
	std::experimental::filesystem::directory_iterator end;
	std::transform(start, end, std::back_inserter(v), pathToString());
}

void GameSceneManager::Init()
{
	std::vector<string> fileNames;
	readDirectoryFileNames(rootFolder_, fileNames);
	for (string name : fileNames) {
		PreloadScene(rootFolder_ + name);
	}
}

void GameSceneManager::PreloadScene(string filePath)
{
	cout << endl << "PRELOADING FILE: " << filePath << endl;
	// Parse json in 'filePath'
	json parsedJson = JsonParser::ParseJsonFile(filePath);
	string sceneName = parsedJson["sceneName"];

	// Add it to sceneFiles map with key 'sceneName' specified in the json
	sceneFiles_[sceneName] = parsedJson;
}

void GameSceneManager::LoadScene(string sceneName)
{
	if (activeScene_.scene != nullptr)
		activeScene_.scene->SetActive(false);

	// Load new scene
	Scene* newScene = new Scene(game);

	// Load scene with json stored is sceneFiles
	newScene->Load(sceneFiles_[sceneName]);

	scenes_[sceneName] = newScene;
}

// Unload scene and load it again
void GameSceneManager::RealoadScene(string sceneName)
{
	// Unload scene
	Scene* s = scenes_[sceneName];
	if (s != nullptr)
	{
		std::cout << "Limpiamos la escena" << std::endl;
		s->Release();
		scenes_[sceneName] = nullptr;
	}

	// Load scene again
	s = new Scene(game);

	// Load scene with json stored is sceneFiles
	s->Load(sceneFiles_[sceneName]);

	scenes_[sceneName] = s;

	if(!activeScene_.scene->IsStarted()) 
		activeScene_.scene->Start();
}

// Set sceneName as the new active scene
void GameSceneManager::ChangeScene(string sceneName)
{
	Scene* s = scenes_[sceneName];

	if (s != nullptr) {

		// Deactivate current scene
		if (activeScene_.scene != nullptr)
			activeScene_.scene->SetActive(false);

		// Set new scene as current scene and activate it
		activeScene_.name = sceneName;
		activeScene_.scene = s;
		activeScene_.scene->SetActive(true);
	}

	if (!activeScene_.scene->IsStarted())
		activeScene_.scene->Start();
}

void GameSceneManager::UnloadScene(string sceneName)
{
	if (sceneName != activeScene_.name)
	{
		scenes_[sceneName]->Release();
		scenes_[sceneName] = nullptr;
	}
}