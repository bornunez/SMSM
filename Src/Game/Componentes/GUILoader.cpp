#include "GUILoader.h"

GUILoader::~GUILoader()
{
}

void GUILoader::LoadFromFile(json obj)
{
	cout << "Empieza a cargar el GUI" << endl;

	string frameWndName = obj["frameWndName"];
	string WndLook = obj["frameWndLook"];
	float WndPosX = obj["wndPosX"];
	float WndPosY = obj["wndPosY"];
	float WndSizeX = obj["wndSizeX"];
	float WndSizeY = obj["wndSizeY"];

	string backgroundMatName = " ";

	if (obj.contains("backgroundMat"))
		string backgroundMatName = obj["backgroundMat"];
	
	GUIManager::Instance()->AddWindow(frameWndName, WndLook, WndPosX, WndPosY, WndSizeX, WndSizeY, backgroundMatName);

	cout << "Se crea window" << endl;

	if (!obj["buttons"].empty()) {
		cout << "Existen botones a cargar" << endl << endl;
		for (auto &but : obj["buttons"])	// Recorremos los botones
			if (but.is_object()) {				
				
				// Recorremos las variables del objeto para crear el boton, aqui podemos tener un switch para elegir
				// entre las funciones locales, para pasarle correctamente la funcion al boton del GUIManager

				string stateWnd = but["buttonName"];
				string buttonScheme = but["buttonScheme"];
				float buttonPosX = but["buttonPosX"];
				float buttonPosY = but["buttonPosY"];
				float buttonSizeX = but["buttonSizeX"];
				float buttonSizeY = but["buttonSizeY"];
				string buttonText = but["buttonText"];
				string buttonFunctionName = but["buttonFunctionName"];

				GUIManager::Instance()->CreateButton(stateWnd, buttonScheme, buttonPosX, buttonPosY, buttonSizeX, buttonSizeY, buttonText, buttonFunctionName);

				cout << "Se ha creado el boton: " << stateWnd << endl;
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
