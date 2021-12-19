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

	p = new Player1;
	pState = IDLE;

	//Idle anim
	p->idlePlayerAnim.PushBack({ 28, 19, 50, 50 });
	p->idlePlayerAnim.PushBack({ 109, 20, 50, 50 });
	p->idlePlayerAnim.PushBack({ 190, 19, 50, 50 });
	p->idlePlayerAnim.PushBack({ 269, 18, 50, 50 });
	p->idlePlayerAnim.loop = true;
	p->idlePlayerAnim.mustFlip = true;
	p->idlePlayerAnim.speed = 0.01f;
	//Walking anim
	p->walkingPlayerAnim.PushBack({ 29, 82, 50, 50 });
	p->walkingPlayerAnim.PushBack({ 108, 81, 50, 50 });
	p->walkingPlayerAnim.PushBack({ 189, 82, 50, 50 });
	p->walkingPlayerAnim.PushBack({ 271, 81, 50, 50 });
	p->walkingPlayerAnim.loop = true;
	p->walkingPlayerAnim.mustFlip = true;
	p->walkingPlayerAnim.speed = 0.1f;
	//Jump anim
	p->jumpingPlayerAnim.PushBack({ 431, 80, 50, 50 });
	p->jumpingPlayerAnim.PushBack({ 428, 16, 50, 50 });
	p->jumpingPlayerAnim.loop = true;
	p->walkingPlayerAnim.mustFlip = true;
	p->jumpingPlayerAnim.speed = 0.1f;

	playerTexture = app->tex->Load("Assets/textures/Mage80x64.png");

	world = new PhysicWorld({ 0,10 });

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
	world->Update(1.0 / 60);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 0);

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