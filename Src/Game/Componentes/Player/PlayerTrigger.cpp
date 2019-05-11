#include "PlayerTrigger.h"
#include "../../../../Src/MotorEngine/Scene.h"


PlayerTrigger::PlayerTrigger(GameObject * o) : Component(o)
{
}

PlayerTrigger::PlayerTrigger(GameObject* o, int w, int h, int _offset_x, int _offset_y ) : Component(o), offset_x(_offset_x), offset_y(_offset_y), width(w),height(h)
{
}

void PlayerTrigger::LoadFromFile(json obj)
{
	if (obj.contains("x")) offset_x = obj["x"];
	else offset_x = 0;
	
	if (obj.contains("y")) offset_y = obj["y"];
	else offset_y = 0;

	if (obj.contains("w")) width = obj["w"];
	else width = 0;

	if (obj.contains("h")) height = obj["h"];
	else height = 0;
}

void PlayerTrigger::Awake()
{
	Component::Awake();
	player = scene->getGameObject("Player");
	if (player == nullptr) {
#ifdef C_DEBUG
		cout << "ERROR: No se asigno la variable player de trigger" << endl;
#endif
	}
}

void PlayerTrigger::Update()
{
	//cout << "Trigger Update" << endl;
	Vector3 playerPos = player->getGlobalPosition();
	Vector3 pos = gameObject->getGlobalPosition();
	pos += Vector3(offset_x, 0, offset_y);

	//cout << "PlayerPos : " << playerPos << " TriggerPos : " << pos << endl;

	if (playerPos.x > pos.x - width / 2 && playerPos.x < pos.x + width / 2) {
		if (playerPos.z > pos.z - height / 2 && playerPos.z < pos.z + height / 2) {
			//El jugador esta dentro del area
#ifdef C_DEBUG
			cout << "El jugador ha entrado en el Trigger [ " << gameObject->getName() << " ]" << endl;	
#endif
			OnTriggerEnter();
		}
	}
}

PlayerTrigger::~PlayerTrigger()
{
}
