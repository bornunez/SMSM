#pragma once
#include "../Component.h"
#include <OgreLight.h>


/*
El componente luz debe ir en un hijo del GO principal ya que la direccion de la luz cambia la del objeto al que esta anyadida

 Componente luz json ejemplo:

 {
	"name":"light",
	"position":{"x":10,"y":0,"z":10},
	"components":[
		{
			"type":"Light",
			"name":"Point_0",
			"lightTpye":"point",
			"diffuse":{"r":1,"g":1,"b":1}
		}
	]
 }
*/

class MyLight :
	public Component
{
protected:
	Ogre::Light * light;
	Ogre::Light::LightTypes type;
public:
	MyLight(GameObject* o);
	virtual void LoadFromFile(json obj);
	virtual ~MyLight();

	Ogre::Light::LightTypes getType(string type);
};

