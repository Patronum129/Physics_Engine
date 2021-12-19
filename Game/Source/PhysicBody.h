#pragma once
#include "Point.h"
#include "List.h"
#include "Globals.h"

enum class BodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC,
	WATER,
};

enum class ShapeType
{
	CIRCLE,
	RECT,
};

enum class COL_TYPE
{
	NONE = 0,
	COLLISION,
	TRIGGER
};

class GameObject;

class PhysicBody
{
private:
	// Position
	fPoint position = { 0.0, 0.0 };
	fPoint lastPosition = { 0.0,0.0 };

	//Properties
	float restitution = 0.0f;
	float friction = 0.0f;
	float coefficientDrag = 0.0f;
	float hydrodynamicDrag = 0.3f;
	float mass = 1.0f;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint acceleration = { 0.0, 0.0 };
	float rotation = 0.0f;
	float gravityScale = 1.0f;
	float surface = 1.0f;
	float width = 1.0f;
	float height = 1.0f;
	float radius = 1.0f;

	BodyType type = BodyType::STATIC;
	ShapeType shape = ShapeType::RECT;
	COL_TYPE colType = COL_TYPE::COLLISION;

	float maximumVelocity = 1000.0f;

	fPoint totalForce = { 0.0, 0.0 };
	fPoint additionalForce = { 0.0, 0.0 };

	List<PhysicBody*> collisionList;

	GameObject* gObject;

public:
	PhysicBody();

	~PhysicBody();

	PhysicBody(iPoint pos, BodyType type, float width, float height, GameObject* gObj = nullptr, COL_TYPE colType = COL_TYPE::COLLISION);

	PhysicBody(iPoint pos, BodyType type, float radius, GameObject* gObj = nullptr, COL_TYPE colType = COL_TYPE::COLLISION);

	PhysicBody(PhysicBody& copy);

	void OnCollisionEnter(PhysicBody* col);

	void OnCollisionStay(PhysicBody* col);

	void OnCollisionExit(PhysicBody* col);

	void OnTriggerEnter(PhysicBody* col);

	void OnTriggerStay(PhysicBody* col);

	void OnTriggerExit(PhysicBody* col);

	void AddForceToCenter(fPoint force);

	// Check if point is contain in this body shape
	bool Contains(fPoint pos)
	{
		if (shape == ShapeType::RECT)
		{
			if (pos.x >= position.x - width / 2 && pos.x <= position.x + width / 2 &&
				pos.y >= position.y - height / 2 && pos.y <= position.y + height / 2)
			{
				return true;
			}
		}
		else
		{
			if (sqrt(pow(position.x - pos.x, 2) + pow(position.y - pos.y, 2)) <= radius)
			{
				return true;
			}
		}

		return false;
	}

	// Get & Set
	iPoint GetPosition()
	{
		int x = (int)METERS_TO_PIXELS(position.x);
		int y = (int)METERS_TO_PIXELS(position.y);

		iPoint pos = { (int)(METERS_TO_PIXELS(position.x)), (int)(METERS_TO_PIXELS(position.y)) };
		return pos;
	}
	void SetPosition(iPoint pos)
	{
		this->position.x = PIXELS_TO_METERS(pos.x);
		this->position.y = PIXELS_TO_METERS(pos.y);
	}

	float GetRestitution()
	{
		return restitution;
	}
	void SetRestitution(float res)
	{
		this->restitution = res;
	}

	float GetFriction()
	{
		return friction;
	}
	void SetFriction(float f)
	{
		this->friction = f;
	}

	float GetMass()
	{
		return mass;
	}
	void SetMass(float mass)
	{
		this->mass = mass;
	}

	float GetDragCoeficient()
	{
		return coefficientDrag;
	}
	void SetDragCoeficient(float drag)
	{
		this->coefficientDrag = drag;
	}

	float GetHydrodynamicDragCoeficient()
	{
		return hydrodynamicDrag;
	}
	void SetHydrodynamicDragCoeficient(float hydrodrag)
	{
		this->hydrodynamicDrag = hydrodrag;
	}

	fPoint GetLinearVelocity()
	{
		return velocity;
	}
	void SetLinearVelocity(fPoint velocity)
	{
		velocity.x > maximumVelocity ? maximumVelocity : velocity.x < -maximumVelocity ? -maximumVelocity : velocity.x;
		velocity.y > maximumVelocity ? maximumVelocity : velocity.y < -maximumVelocity ? -maximumVelocity : velocity.y;

		this->velocity = velocity;
	}

	void SetGravityScale(float gravityScale)
	{
		this->gravityScale = gravityScale;
	}
	float GetGravityScale()
	{
		return gravityScale;
	}

	float GetRotation()
	{
		return rotation;
	}
	void SetRotation(float rotation)
	{
		this->rotation = rotation;
	}

	float GetRadius()
	{
		return radius;
	}

	BodyType GetType()
	{
		return type;
	}
private:

	void ResetForces();

	friend class PhysicWorld;
};