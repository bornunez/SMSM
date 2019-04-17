#include "..\Component.h"
#include "MapLoader.h"
#include "..\Loaders\PrefabManager.h"


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
						//Recorre Properties
						if (mapObj.contains("properties")) {
							// Crea el objeto a escala 1 e y 0
							for (auto &prop : mapObj["properties"]) {
								float objScale = 1;
								if (prop.contains("name") && prop["name"] == "scale")
									objScale = prop["value"];
								else if (prop.contains("name") && prop["name"] == "prefab") {
									// Crea el prefab asignado al nombre "value"

									//Numero por el que hay que dividir x e y para obtener posiciones reales
									int divPos = mapObj["width"];

									cout << "Instanciado objeto: " << prop["value"] << " en la posicion " << mapObj["x"]  << " , " << (int)mapObj["y"] / (int)divPos << endl;
									GameObject* o = PrefabManager::getInstance()->Instantiate(prop["value"], scene, nullptr, Vector3(mapObj["x"] / divPos, 0, (int)mapObj["y"] / (int)divPos),objScale * scale );

										
									////Si existe un prefab con el nombre, lo rellenamos
									//if (o != nullptr) {
									//	mapScene->Add(o);

									//	cout << "Loaded prefab: " << pref["prefabName"] << " succesfully" << endl << endl;
									//}
								}
							}
						}
					}
				}
			}
	}
	cout << "==================================================\n\n";
}