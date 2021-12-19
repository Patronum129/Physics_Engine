#ifndef __SCENE_H__
#define __SCENE_H__

#include "App.h"
#include "Globals.h"
#include "GameObject.h"
#include "List.h"
struct SDL_Texture;

class PhysicWorld;
class PhysicBody;

class Scene : public Module
{
protected:

	int ID;

	App* _app;

	List<SDL_Texture*> sceneTextures;
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	void InitScene();
	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DestroyGameObject(GameObject* gameObject);

	int getID()
	{
		return ID;
	}

	int lastID = 0;

	int playerX;
	int playerY;

	SDL_Texture* bg[4];

	SDL_Rect rect = { 10,10,10,10 };
	SDL_Rect rect2 = { 10,10,10,10 };
	SDL_Rect rect3 = { 10,10,10,10 };
	PhysicBody* body;
	PhysicBody* body2;

	PhysicBody* walls[3];
	PhysicBody* floor[6];

	PhysicWorld* world;

	GameObject* testGO = nullptr;
	GameObject* testGO2 = nullptr;

	List<GameObject*> gameObjects;
private:
	SDL_Texture* img;
};

#endif // __SCENE_H__