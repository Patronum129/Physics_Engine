#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "GameScene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

GameScene::GameScene() : Module()
{
	name.Create("gameScene");
}

// Destructor
GameScene::~GameScene()
{}

// Called before render is available
bool GameScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	
	return true;
}

// Called each loop iteration
bool GameScene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool GameScene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool GameScene::PostUpdate(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool GameScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void GameScene::Activate()
{
	Module::Activate();

	app->map->Activate();

	InitMapLevel();
}

void GameScene::Deactivate()
{
	Module::Deactivate();

	app->map->Deactivate();
}

void GameScene::InitMapLevel()
{
	app->map->mapData.Clear();
	app->map->Load("map1.tmx");
	
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
}

