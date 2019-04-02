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
	mapFile = JsonParser::ParseJsonFile(mapFile["path"]);

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
					for (auto &obj : pref["objects"]){
						//Recorre Properties
						if (obj.contains("properties")) {
							for (auto &prop : obj["properties"]) {
								if (prop.contains("name") && prop["name"] == "prefab") {
									// Crea el prefab asignado al nombre "value"

									//Numero por el que hay que dividir x e y para obtener posiciones reales
									int divPos = obj["width"];

									// Crea el objeto a escala 1 e y 0
									float scale = 1;
									if (mapFile.contains("scale"))
										scale = mapFile["scale"];
									GameObject* o = PrefabManager::getInstance()->Instantiate(prop["value"], mapScene, nullptr, Vector3(obj["x"] / divPos, 0, obj["y"] / divPos),scale );

									//Si existe un prefab con el nombre, lo rellenamos
									if (o != nullptr) {
										mapScene->Add(o);

										cout << "Loaded prefab: " << pref["prefabName"] << " succesfully" << endl << endl;
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