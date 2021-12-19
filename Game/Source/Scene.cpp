#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "PhysicWorld.h"

#include "Defs.h"
#include "Log.h"

iPoint position;
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
	app->render->camera.x = app->render->camera.y = 0;

	world = new PhysWorld({ 0, 10 });

	body = new PhysicBody({ 200, 600 }, BodyType::STATIC, rect.w, rect.h, COL_TYPE::COLLISION);

	body2 = new PhysicBody({ 300, 240 }, BodyType::STATIC, rect2.w, rect2.h, COL_TYPE::COLLISION);

	body3 = new PhysicBody({ 300, 200 }, BodyType::DYNAMIC, 2, COL_TYPE::COLLISION);

	//body3->SetMass(20);
	body3->SetRestitution(0.7f);
	body3->SetDragCoeficient(0.01f);
	body3->SetGravityScale(8);
	body3->SetHydrodynamicDragCoeficient(0.3f);
	body3->SetFriction(1.0f);

	//body4 = new RigidBody({ 445, 500 }, RigidBodyType::STATIC, 10);

	//body4->SetGravityScale(2.0f);

	world->AddPhysicBody(body);

	world->AddPhysicBody(body2);

	world->AddPhysicBody(body3);
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

	world->Update(1.0 / 60);

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
	{
		world->Update(1.0 / 60);
	}
	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		world->Update(1.0 / 60);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		body3->AddForceToCenter({ 0, -200 });
		//body3->SetLinearVelocity({ body3->GetLinearVelocity().x,-200 });
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		body3->AddForceToCenter({ -200, 0 });
		//body3->SetLinearVelocity({ -200,0 });
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		body3->AddForceToCenter({ 200, 0 });
		//body3->SetLinearVelocity({ 200,0 });
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		body3->AddForceToCenter({ 0, 200 });
		//body3->SetLinearVelocity({ 0,200 });
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 0);

	rect.x = body->GetPosition().x;
	rect.y = body->GetPosition().y;

	app->render->DrawRectangle(rect, 0, 255, 0, 155);

	rect2.x = body2->GetPosition().x;
	rect2.y = body2->GetPosition().y;

	app->render->DrawRectangle(rect2, 255, 255, 0, 255);

	app->render->DrawCircle(body3->GetPosition().x, body3->GetPosition().y, body3->GetRadius(), 255, 0, 0);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	RELEASE(world);

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
}