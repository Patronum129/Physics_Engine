#pragma once
#include "PhysicBody.h"
#include "List.h"


enum COL_TYPE
{
	NONE = -1,
	COLLISION,
	TRIGGER
};

class PhysWorld
{
public:
	PhysWorld(fPoint gravity);

	~PhysWorld();

	void Update(float dt);

	bool CheckCollision(PhysicBody* body);

	void AddPhysicBody(PhysicBody* body);

	void DelPhysicBody(PhysicBody* body);

	COL_TYPE BoxColBox(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	COL_TYPE CircleColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	COL_TYPE BoxColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	void ResolveColForce(PhysicBody& b1, PhysicBody& b2, fPoint colPoint);

	float submergedVolume(PhysicBody* body, PhysicBody* water);

	void SetairForce(fPoint airforce)
	{
		air = airforce;
	}

	void SetGravity(fPoint gravityforce)
	{
		gravity = gravityforce;
	}

	fPoint GetAirForce()
	{
		return air;
	}

	fPoint GetGravity()
	{
		return gravity;
	}

	fPoint CollisionPoint(PhysicBody& b1, PhysicBody& b2);

	fPoint CollisionDir(PhysicBody& b1, fPoint colPoint);

	void ResolveClamping(PhysicBody& b1, PhysicBody& b2);

private:

	fPoint gravity;
    float density = 1.0f;
	fPoint air = { 0,0 };
	int resolveColForce = 5;

public:

	List<PhysicBody*> physicBodies;
};

