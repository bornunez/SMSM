#include "..\Component.h"
#include "MapLoader.h"
#include "..\Loaders\PrefabManager.h"
#include "../../../Src/Game/Mapa/Spawner.h"


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
						int divPos = mapObj["width"];
						Vector3 pos = Vector3(mapObj["x"] / divPos, 0, (int)mapObj["y"] / (int)divPos);


						//Recorre Properties
						if (mapObj.contains("properties")) {
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
										GameObject* o = PrefabManager::getInstance()->Instantiate(prop["value"], scene, nullptr,pos, objScale * scale);		
									}

									else if (propName == "spawner") {
										//Generamos un spawner
										int index = prop["value"];
										GameObject* o = scene->GenerateEmptyGameObject("Spawner", nullptr, pos, objScale * scale);
										Spawner* s = new Spawner(o,index);
										o->AddComponent(s);
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