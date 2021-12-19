#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Defs.h"
#include "Log.h"


Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
    //Initializing player struct data
    p = new Player1;
    pState = IDLE;
 
	/*
    p->player = app->physics->CreateCircle(20, 300, 7, b2_dynamicBody);
    p->player->body->SetFixedRotation(true);
	p->player->listener = this;
    p->player->type = PhysBody::Type::PLAYER;
	*/
    //Idle anim
    p->idlePlayerAnim.PushBack({ 8, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 86, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 164, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 242, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 320, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 398, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 476, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 554, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 632, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 710, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 788, 17, 50, 50 });
    p->idlePlayerAnim.loop = false;
    p->idlePlayerAnim.mustFlip = true;
    p->idlePlayerAnim.speed = 0.01f;
    //Walking anim
    p->walkingPlayerAnim.PushBack({ 8, 79, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 86, 77, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 164, 78, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 242, 81, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 320, 79, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 398, 77, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 476, 78, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 554, 81, 50, 50 });
    p->walkingPlayerAnim.loop = true;
    p->walkingPlayerAnim.mustFlip = true;
    p->walkingPlayerAnim.speed = 0.1f;
    //Jump anim
    p->jumpingPlayerAnim.PushBack({ 8, 188, 50, 50 });
    p->jumpingPlayerAnim.loop = true;
    p->walkingPlayerAnim.mustFlip = true;
    p->jumpingPlayerAnim.speed = 0.1f;
    //Death anim
    p->deathPlayerAnim.PushBack({ 5, 133, 50, 50 });
    p->deathPlayerAnim.PushBack({ 79, 140, 50, 50 });
    p->deathPlayerAnim.PushBack({ 156, 141, 50, 50 });
    p->deathPlayerAnim.PushBack({ 233, 142, 50, 50 });
    p->deathPlayerAnim.loop = false;
    p->deathPlayerAnim.mustFlip = true;
    p->deathPlayerAnim.speed = 1.0f;

    playerTexture = app->tex->Load("Assets/textures/Tileset.png");

	app->audio->LoadFx("Assets/audio/fx/jumpSound.wav");
	app->audio->LoadFx("Assets/audio/fx/winSound.wav");
	app->audio->LoadFx("Assets/audio/fx/gameOverSound.wav");

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	//---------------------------------------
    switch (pState)
    {
    case IDLE:
        currentAnim = &p->idlePlayerAnim;
        break;
    case WALK:
        currentAnim = &p->walkingPlayerAnim;
        break;
    case JUMP:
        currentAnim = &p->jumpingPlayerAnim;
        break;
    case DEATH:
        currentAnim = &p->deathPlayerAnim;
        break;
    }

	/*
	//Player Render
    if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { p->player->body->SetTransform({ PIXEL_TO_METERS(30), PIXEL_TO_METERS(330) }, 0.0f); }
    if (app->gameState == 1)
    {
        app->render->DrawTexture(playerTexture, METERS_TO_PIXELS(p->player->body->GetPosition().x - 23), METERS_TO_PIXELS(p->player->body->GetPosition().y) - 20,
            &(currentAnim->GetCurrentFrame()), 1);
    }
	*/
    bool ret = true;

    //Player movement
    maxSpeedX = 2;
    minSpeedX = -2;

	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing Player");
	return true;
}