#pragma once
#include "PhysicBody.h"
#include "List.h"

class PhysWorld
{
public:
	PhysWorld(fPoint gravity);

	~PhysWorld();

	void Update(float simulationTime);

	bool CheckCollision(PhysicBody* body);

	void AddPhysicBody(PhysicBody* body);

	void DeleteRigidBody(PhysicBody* body);

	void BoxColBox(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	void CircleColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	void BoxColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger = false);

	void ResolveColForce(PhysicBody& b1, PhysicBody& b2, fPoint colPoint);

	float submergedVolume(PhysicBody* body, PhysicBody* water);

	void SetWind(fPoint windforce)
	{
		air = windforce;
	}

	void SetGravity(fPoint gravityforce)
	{
		gravity = gravityforce;
	}

	fPoint GetWind()
	{
		return air;
	}

	fPoint GetGravity()
	{
		return gravity;
	}

	// Detect collision point in 2 shapes
	fPoint CollisionPoint(PhysicBody& b1, PhysicBody& b2);

	// Dectet center of shape -> collision point(any point) vector
	fPoint CollisionDir(PhysicBody& b1, fPoint colPoint);

	// Resolve clamping case
	void ResolveClipping(PhysicBody& b1, PhysicBody& b2);

	/// <summary>
	/// Check intersectionPoint of 2 lines
	/// </summary>
	/// <param name="p1">init of line 1</param>
	/// <param name="p2">end of line 1</param>
	/// <param name="p3">init of line 2</param>
	/// <param name="p4">end of line 2</param>
	/// <returns></returns>
	fPoint IntersectionPoint(fPoint p1, fPoint p2, fPoint p3, fPoint p4);

private:

	fPoint gravity;

	float density = 1.0f;

	fPoint air = { 0, 0 };

	int resolveColForce = 5;

public:

	bool gravityOn = true;
	bool buoyancyOn = true;
	bool hydrioDragOn = true;
	bool aeroDragOn = true;
	bool clippingOn = true;
	bool frictioOn = true;

	List<PhysicBody*> physicBodies;
};