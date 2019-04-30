#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/MotorEngine/GUIManager.h"

#include <map>

// Componente para la carga de elementos del gui para cada escena

// Ventana principal, botones, funciones callbacks...
// Tambien utilizado para aplicar un fondo a la ventana del menu si fuera neesario
// Tendrá una lista de botones para crearlos

class GUILoader : public Component
{
public:
	GUILoader(GameObject* obj) : Component(obj) {};
	~GUILoader();

	virtual void LoadFromFile(json obj);
	virtual void Update();
	virtual void Start();
	virtual void OnEnable();
	virtual void OnDisable();

	std::map<std::string, CEGUI::FrameWindow *> Windows;
	std::map<std::string, bool> WindowsStates;

	std::map<std::string, CEGUI::Window *> Buttons;
	std::map<std::string, bool> ButtonsStates;

private:

};

