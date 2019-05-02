#include "WeaponBullet.h"


WeaponBullet::WeaponBullet(GameObject* obj):RigidBodyComponent(obj)
{

}


WeaponBullet::~WeaponBullet()
{
}

void WeaponBullet::Start()
{
	RigidBodyComponent::Start();
	physicRB->setGravity(btVector3(0, grav, 0));
	physicRB->setDamping(linDamp, angDamp);
	//physicRB->applyTorqueImpulse(btVector3(0, 0.1, 0));
	direccion = scene->getGameObject("Player")->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;

	Vector3 auxVec = Vector3(scene->getGameObject("Player")->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z);
	//scene->Instantiate("Bullet", gameObject->getChild("ShootPoint")->getGlobalPosition(), 0.01);
	// gameObject->getGlobalPosition() + forward*0.4 + Vector3(0, -0.22, 0), 0.01);
	auxVec = Quaternion(Degree(-90), Vector3::UNIT_Y) * auxVec;
	float angle = atan2(auxVec.x, auxVec.z);
	btQuaternion q;
	q.setX(0);
	q.setY(1 * sin(angle / 2));
	q.setZ(0);
	q.setW(cos(angle / 2));
	btRigidBody* rb = getComponent<RigidBodyComponent>()->getRB();
	rb->getWorldTransform().setRotation(q);
	
}

void WeaponBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Params from file
	grav = obj["gravity"];
	linDamp = obj["linDamp"];
	angDamp = obj["angDamp"];
	speed = obj["speed"];
	recoilTime = obj["recoilTime"];
}

void WeaponBullet::collisionHandler(int id)
{
	//Destruye la bala cuando colisiona con algo que no sea el jugador
	if (!hit && id != 0) {
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void WeaponBullet::Update()
{
	if(!hit)
		physicRB->applyCentralImpulse(btVector3(direccion.x*speed, direccion.y*speed, direccion.z*speed));
	else //Se posria hacer aqui un contador para que desapareciese la bala
	{
		
	}
}
