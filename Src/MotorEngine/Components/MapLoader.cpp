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
#ifdef C_DEBUG
	cout << "\n\n==================================================\n";
	cout << "============      CARGA DE MAPA	       ============\n";
	cout << "==================================================\n\n";
#endif
	int tilewidth = mapFile["tilewidth"];

	if (!mapFile["layers"].empty()) {
#ifdef C_DEBUG
		cout << "Existen GameObjects a cargar" << endl << endl;
#endif
		for (auto &pref : mapFile["layers"])
			if (pref.is_object()) {
				//Recorre objects
				if (pref.contains("objects")) {
					float scale = 1;
					if (obj.contains("scale"))
						scale = obj["scale"];
#ifdef C_DEBUG
					cout << "Scala del mapa: " << scale << endl;
#endif
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
							json yProp = FindProperty(mapObj, "y");
							if (yProp != nullptr)
								pos.y = yProp["value"];


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
#ifdef C_DEBUG
										cout << "Instanciado objeto: " << prop["value"] << " en la posicion " << pos.x  << " , " << pos.z << endl;
#endif
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
#ifdef C_DEBUG
										cout << "Nueva Entry en " << pos << " de tamaño " << width << " " << height << endl;
#endif
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
#ifdef C_DEBUG
	cout << "==================================================\n\n";
#endif
}

json MapLoader::FindProperty(json obj, string property) {
	for (auto &prop : obj["properties"]) {
		if (prop.contains("name") && prop["name"] == property)
			return prop;
	}
	return nullptr;
}