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
	img = app->tex->Load("Assets/Textures/map1.png");
	//app->audio->PlayMusic("Assets/Audio/Music/Retro_Platforming_David_Fesliyan.ogg");
	app->render->camera.x = app->render->camera.y = 0;
	world = new PhysicWorld({ 0,10 });

	InitScene();

	walls[0] = new PhysicBody({ 0,0 }, BodyType::STATIC, 1280, 5);
	walls[1] = new PhysicBody({ 0,0 }, BodyType::STATIC, 5, 720);
	walls[2] = new PhysicBody({ 1275,0 }, BodyType::STATIC, 5, 720);

	floor[0] = new PhysicBody({ 0,576 }, BodyType::STATIC, 560, 144);
	floor[1] = new PhysicBody({ 560,672 }, BodyType::STATIC, 192, 48);
	floor[2] = new PhysicBody({ 752,576 }, BodyType::STATIC, 560, 144);
	floor[3] = new PhysicBody({ 96,416 }, BodyType::STATIC, 32, 32);
	floor[4] = new PhysicBody({ 384,544 }, BodyType::STATIC, 32, 32);
	floor[5] = new PhysicBody({ 928,416 }, BodyType::STATIC, 32, 32);

	for (int i = 0; i < 3; i++)
	{
		world->AddPhysicBody(walls[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		world->AddPhysicBody(floor[i]);
	}

	return true;
}

void Scene::InitScene()
{
	// Obstacles
	GameObject* g = new GameObject("wall", "Wall", _app);
	// +8 = offset, porque pivot de b2Body es el centro, y de tectura es izquierda superior.
	g->rBody = new PhysicBody({ 100,100 }, BodyType::STATIC, 16, 16);
	world->AddPhysicBody(g->rBody);
	gameObjects.add(g);

	GameObject* j = new GameObject("floor", "floor", _app);
	j->rBody = new PhysicBody({ 100,100 }, BodyType::STATIC, 16, 16);
	world->AddPhysicBody(j->rBody);
	gameObjects.add(j);
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PreUpdate();
		if (gameObjects[i]->pendingToDelete == true)
		{
			if (gameObjects[i]->rBody != nullptr)
			{
				world->DelPhysicBody(gameObjects[i]->rBody);
			}
			gameObjects[i]->CleanUp();
			gameObjects.del(gameObjects.At(gameObjects.find(gameObjects[i])));
		}
	}
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	world->Update(1.0 / 60);
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->Update();
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 0);

	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PostUpdate();
	}

	app->render->DrawRectangle({ 0,0,1280,5 }, 255, 0, 0);
	app->render->DrawRectangle({ 1275,0,5,720 }, 255, 0, 0);
	app->render->DrawRectangle({ 0,0,5,720 }, 255, 0, 0);
	app->render->DrawRectangle({ 0,576,560,144 }, 255, 0, 255, 70);
	app->render->DrawRectangle({ 560,672,192,48 }, 255, 0, 255, 70);
	app->render->DrawRectangle({ 752,576,560,144 }, 255, 0, 255, 70);
	app->render->DrawRectangle({ 96,416,32,32 }, 255, 0, 255, 70);
	app->render->DrawRectangle({ 384,544,32,32 }, 255, 0, 255, 70);
	app->render->DrawRectangle({ 928,416,32,32 }, 255, 0, 255, 70);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	RELEASE(world);
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->CleanUp();
		}
	}

	gameObjects.clear();

	for (int i = 0; i < sceneTextures.count(); i++)
	{
		if (sceneTextures[i])
		{
			_app->tex->UnLoad(sceneTextures[i]);
			sceneTextures[i] = nullptr;
		}
	}

	return true;
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	int index = gameObjects.find(gameObject);

	if (index >= 0)
	{
		gameObjects.del(gameObjects.At(index));
	}
}