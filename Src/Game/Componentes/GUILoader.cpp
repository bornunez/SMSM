#include "GUILoader.h"



GUILoader::~GUILoader()
{
}

void GUILoader::LoadFromFile(json obj)
{
	string frameWndName = obj["FrameWndName"];
	string WndLook = obj["FrameWndLook"];
	float WndPosX = obj["WndPosX"];
	float WndPosY = obj["WndPosY"];
	float WndSizeX = obj["WndSizeX"];
	float WndSizeY = obj["WndSizeY"];

	string backgroundMatName = " ";

	if (obj.contains("backgroundMat"))
		string backgroundMatName = obj["backgroundMat"];
	
	GUIManager::Instance()->AddWindow(frameWndName, WndLook, WndPosX, WndPosY, WndSizeX, WndSizeY, backgroundMatName);

	if (!obj["Buttons"].empty()) {
		cout << "Existen botones a cargar" << endl << endl;
		for (auto &pref : obj["Buttons"])	// Recorremos los botones
			if (pref.is_object()) {				
				
				// Recorremos las variables del objeto para crear el boton, aqui podemos tener un switch para elegir
				// entre las funciones locales, para pasarle correctamente la funcion al boton del GUIManager


			}
	}
}

void GUILoader::Update()
{
}

void GUILoader::Start()
{
}

void GUILoader::OnEnable()
{
}

void GUILoader::OnDisable()
{
}
