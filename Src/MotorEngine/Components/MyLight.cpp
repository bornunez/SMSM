#include "MyLight.h"
#include "OgreSceneManager.h"

MyLight::MyLight(GameObject * o) : Component(o)
{
}

void MyLight::LoadFromFile(json obj)
{
	Ogre::SceneManager* scnMgr = gameObject->getSceneManager();

	//Creamos el objeto de la luz
	if (obj.contains("name"))
		light = scnMgr->createLight(obj["name"]);
	else
		cout << "ERROR: La luz tiene que tener un nombre" << endl;

	//Se lo vinculamos al GO
	gameObject->getNode()->attachObject(light);

	//Y parseamos los parametros en funcion del tipo
	if (obj.contains("lightTpye")) {
		type = getType(obj["lightTpye"]);
		light->setType(type);
	}
	else
		cout << "ERROR: La luz necesita un tipo " << endl;

	if (obj.contains("specular")) {
		json spec = obj["specular"];
		light->setSpecularColour(spec["r"], spec["g"], spec["b"]);
	}
	if (obj.contains("diffuse")) {
		json dif = obj["diffuse"];
		light->setDiffuseColour(dif["r"], dif["g"], dif["b"]);
	}

	if ((type == Ogre::Light::LT_DIRECTIONAL || type == Ogre::Light::LT_SPOTLIGHT) && obj.contains("dir")) {
		json dir = obj["dir"];
		gameObject->getNode()->setDirection(dir["x"], dir["y"], dir["z"]);
	}
	if ((type == Ogre::Light::LT_POINT || type == Ogre::Light::LT_SPOTLIGHT) && obj.contains("pos")) {
		json pos = obj["pos"];
		gameObject->getNode()->setDirection(pos["x"], pos["y"], pos["z"]);
	}

	light->setAttenuation(50, 1,0.1, 0);

	cout << light->getAttenuationConstant() << " " << light->getAttenuationLinear() << " " << light->getAttenuationQuadric() << " " << light->getAttenuationRange()<< endl;

}

MyLight::~MyLight()
{
}

Ogre::Light::LightTypes MyLight::getType(string type)
{
	Ogre::Light::LightTypes lightType = Ogre::Light::LT_POINT;

	if (type == "point")
		lightType = Ogre::Light::LT_POINT;
	else if (type == "spotlight")
		lightType = Ogre::Light::LT_SPOTLIGHT;
	else if (type == "directional")
		lightType = Ogre::Light::LT_DIRECTIONAL;
	else
		cout << "Error en el tipo de luz" << endl;

	return lightType;
}
