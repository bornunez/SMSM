#pragma once
#include "Scene.h"

struct ScenePair
{
	string name;
	Scene* scene;
};

class GameSceneManager
{
public:
	GameSceneManager(Game* g, string scenesFolder);
	~GameSceneManager();

	void Init();
	void LoadScene(string sceneName);
	void RealoadScene(string sceneName);
	void ChangeScene(string sceneName);
	void UnloadScene(string sceneName);

	Scene* GetActiveScene() { return activeScene_.scene; }

private:
	void PreloadScene(string filePath);

	Game* game;

	ScenePair activeScene_;

	std::map<string, Scene*> scenes_;

	std::map<string, json> sceneFiles_;
	string rootFolder_;
};

