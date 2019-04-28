#pragma once
#include "../../../Src/MotorEngine/Component.h"


// Componente para la carga de elementos del gui para cada escena

// Ventana principal, botones, funciones callbacks...
// Tambien utilizado para aplicar un fondo a la ventana del menu si fuera neesario
// Tendrá una lista de botones para crearlos

class GUILoader : public Component
{
public:
	GUILoader(GameObject* obj) : Component(obj) {};
	~GUILoader();

	void LoadFromFile(json obj);
	virtual void Update();
	virtual void Start();
	virtual void OnEnable();
	virtual void OnDisable();

};

