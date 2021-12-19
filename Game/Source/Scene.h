#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "PhysicBody.h"
#include "PhysicWorld.h"

struct SDL_Texture;

class PhysicBody;
class PhysWorld;

#define BOUNCER_TIME 200

struct Bouncer
{
	Bouncer() : texture(NULL), hit_timer(0), fx(0)
	{}

	SDL_Texture* texture;
	Uint32 hit_timer;
	uint fx;
};

enum lightTypes
{
	tiny,
	medium,
	big
};
class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called to activate module
	void Activate();

	// called to deactivate module
	void Deactivate();

	void InitMapLevel();
private:
	SDL_Texture* img;

public:
	SDL_Rect rect = { 10,10,500,500 };
	SDL_Rect rect2 = { 10,10,20,20 };

	PhysicBody* body;
	PhysicBody* body2;
	PhysicBody* body3;
	PhysicBody* body4;

	PhysWorld* world;
};

#endif // __SCENE_H__