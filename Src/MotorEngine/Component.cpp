#include "Component.h"

#include "GameObject.h"



Component::Component(GameObject * _gameObject, bool _enabled) : gameObject(_gameObject), enabled(_enabled)
{
	scene = gameObject->getScene();
	//cout << "Component: " << gameObject->getName() << endl;
}

void Component::LoadFromFile(json obj)
{
	if (obj.contains("enabled"))
		enabled = obj["enabled"];
}

Component::~Component()
{
}

bool Component::isActiveAndEnabled()
{
	return enabled && gameObject->isActive();
}

void Component::Disable()
{
	SetEnabled(false);
}

void Component::Enable()
{
	SetEnabled(true);
}

void Component::SetEnabled(bool _enabled)
{
	//En caso de que estuvieramos activos y nos desactiven, llamamos al OnDisable
	if (enabled && !_enabled) {
		OnDisable();
	}
	//Del mismo modo, solo cuando pasemos de estar inactivos a estar activos, se llamara al OnEnable
	else if(!enabled && _enabled) {
		OnEnable();
	}
	//Finalmente, actualizamos 
	enabled = _enabled;

}