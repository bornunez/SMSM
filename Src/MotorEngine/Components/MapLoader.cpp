#include "..\Component.h"
#include "MapLoader.h"
#include "..\Loaders\PrefabManager.h"
#include "../../../Src/Game/Mapa/Spawner.h"
#include "../../../Src/Game/Mapa/Entry.h"
#include "../../../Src/Game/Mapa/RoomManager.h"


MapLoader::~MapLoader()
{
}

void MapLoader::Awake()
{
}

void MapLoader::LoadFromFile(json obj)
{
	//Init
	mapFile = JsonParser::ParseJsonFile(obj["path"]);

	//Cargar GameObjects
	cout << "\n\n==================================================\n";
	cout << "============      CARGA DE MAPA	       ============\n";
	cout << "==================================================\n\n";
	int tilewidth = mapFile["tilewidth"];

	if (!mapFile["layers"].empty()) {
		cout << "Existen GameObjects a cargar" << endl << endl;
		for (auto &pref : mapFile["layers"])
			if (pref.is_object()) {
				//Recorre objects
				if (pref.contains("objects")) {
					float scale = 1;
					if (obj.contains("scale"))
						scale = obj["scale"];
					cout << "Scala del mapa: " << scale << endl;
					for (auto &mapObj : pref["objects"]){
						json newObj; //Objeto que vamos a construir, basicamente vamos a "construir" uno 
						float objScale = 1;
						//Numero por el que hay que dividir x e y para obtener posiciones reales
						float width = mapObj["width"]/tilewidth;
						float height = mapObj["height"]/tilewidth;
						Vector3 pos = Vector3((int)mapObj["x"]/(int)tilewidth + width / 2.0, 0, (int)mapObj["y"]/(int)tilewidth + height/2.0);


						//Recorre Properties
						if (mapObj.contains("properties")) {
							GameObject* o = scene->GenerateEmptyGameObject("Empty", nullptr, pos, objScale*scale);
							// Crea el objeto a escala 1 e y 0
							for (auto &prop : mapObj["properties"]) {

								if (prop.contains("name")) {
									string propName = prop["name"];
									//Escala del objeto local al mapa
									if (propName == "scale")
										objScale = prop["value"];

									//Instanciacion de prefabs
									else if (propName == "prefab") {
										// Crea el prefab asignado al nombre "value"
										cout << "Instanciado objeto: " << prop["value"] << " en la posicion " << pos.x  << " , " << pos.z << endl;
										o = PrefabManager::getInstance()->Instantiate(prop["value"], scene, nullptr,pos, objScale * scale,o);
									}

									else if (propName == "spawner") {
										//Generamos un spawner
										int index = prop["value"];
										o->setName("Spawner");

										Spawner* s = new Spawner(o,index);
										for (auto &p : mapObj["properties"]) {
											if (p["type"] == "bool") {
												if (p["value"] == true)
													s->AddEnemy(p["name"]);
											}
										}

										o->AddComponent(s);
									}

									else if (propName == "entry") {
										//Se genera una nueva entrada con el index asignado
										int index = prop["value"];
										o->setName("Entry");
										cout << "Nueva Entry en " << pos << " de tamaño " << width << " " << height << endl;
										Entry* e = new Entry(o, index, width, height);
										o->AddComponent(e);

									}

									else if (propName == "gate") {
										int index = prop["value"];
										o->setName("Gate");
										o->SetActive(false);
										RoomManager::getInstance()->getInstance()->GetRoom(index)->AddGate(o);
									}
									
								}

							}
						}
					}
				}
			}
	}
	cout << "==================================================\n\n";
}

json FindProperty(json obj, string property) {
	for (auto &prop : obj["properties"]) {
		if (prop.contains("name") && prop["name"] == property)
			return prop;
	}
	return nullptr;
}