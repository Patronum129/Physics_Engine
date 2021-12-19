#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Load map
	//app->map->Load("map1.tmx");
	//img = app->tex->Load("Assets/Textures/map1.png");
	//app->audio->PlayMusic("Assets/Audio/Music/Retro_Platforming_David_Fesliyan.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 0);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::Activate()
{
	Module::Activate();

	//app->physics->Activate();
	//app->playerModule->Activate();
	app->map->Activate();
	//app->enemies->Activate();

	InitMapLevel();
}

void Scene::Deactivate()
{
	Module::Deactivate();

	//app->playerModule->Deactivate();
	app->map->Deactivate();
	//app->physics->Deactivate();
	//app->enemies->Deactivate();
}
void Scene::InitMapLevel()
{
	app->map->mapData.Clear();

	app->map->Load("level_1.tmx");

	MapData* mapData = &app->map->mapData;

	// Physworld box
	uint size = mapData->layers.count();

	MapLayer* collisionLayer = NULL;

	for (uint i = 0; i < size; i++) {
		if (mapData->layers[i]->name == "Collisions") {
			collisionLayer = mapData->layers[i];
			break;
		}
	}

	int tileWidth = app->map->mapData.tileWidth;
	int tileHeight = app->map->mapData.tileHeight;


	//for (int y = 0; y < collisionLayer->height; y++)
	//{
	//	for (int x = 0; x < collisionLayer->width; x++)
	//	{
	//		ColliderLayerType colType = (ColliderLayerType)collisionLayer->Get(x, y);

	//		switch (colType)
	//		{
	//		case ColliderLayerType::SPAWN:
	//			if (!fromGameSaved)
	//			{
	//				app->playerModule->SetPosition(x * tileWidth, y * tileHeight - tileHeight / 2);
	//				app->playerModule->playerscore = 0;
	//			}
	//			else
	//			{
	//				app->playerModule->SetPosition(playerPosition.x, playerPosition.y);
	//				app->playerModule->SetState(playerState);
	//			}
	//			break;
	//		case ColliderLayerType::END:
	//			pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2, tileWidth, tileHeight, false);
	//			pbody->bodyType = PhysBodyType::END;
	//			break;
	//		case ColliderLayerType::NORMAL:
	//			pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2, tileWidth, tileHeight, false);
	//			pbody->bodyType = PhysBodyType::GROUND;
	//			break;
	//		case ColliderLayerType::SPIKE:
	//			pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2 + 3, tileWidth, tileHeight - 6, false);
	//			pbody->bodyType = PhysBodyType::SPIKES;
	//			break;
	//		case ColliderLayerType::PLATFORM:
	//			pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 4, tileWidth, tileHeight / 2, false);
	//			pbody->bodyType = PhysBodyType::PLATFORM;
	//			break;
	//		}
	//	}
	//}

	//// Spawn enemies
	//if (!fromGameSaved) {
	//	uint groupCount = mapData->objectGroups.Count();

	//	ObjectGroup* enemyGroup = NULL;

	//	for (uint i = 0; i < groupCount; i++) {
	//		if (mapData->objectGroups[i]->name == "Entities") {
	//			enemyGroup = mapData->objectGroups[i];
	//			break;
	//		}
	//	}

	//	if (enemyGroup) {
	//		uint enemyCount = enemyGroup->objects.Count();

	//		for (uint i = 0; i < enemyCount; i++) {
	//			Object* obj = enemyGroup->objects[i];

	//			iPoint position = { obj->x, obj->y };

	//			app->enemies->AddEntity(obj->id, (EntityModule::EntityType)obj->type, position);
	//		}
	//	}
	//}
	//else {
	//	pugi::xml_document saveFile;
	//	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	//	if (result != NULL)
	//	{
	//		pugi::xml_node save_node = saveFile.child("game_state");

	//		app->enemies->LoadEntities(save_node.child("entities"));
	//	}
	//}
}