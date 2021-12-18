#pragma once
#include "Point.h"
#include "List.h"

enum class ShapeType
{
	CIRCLE,
	RECTANGLE,
};

enum BodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC,
	WATER,
};

class PhysicBody
{
private:
	fPoint position = { 0.0, 0.0 };
	fPoint lastPosition = { 0.0,0.0 };

	float restitution = 0.0f;
	float friction = 0.0f;
	float coefficientDrag = 0.0f;
	float hydrodynamicDrag = 0.3f;
	float mass = 1.0f;

	fPoint velocity = { 0.0f, 0.0f };
	fPoint acceleration = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float gravityScale = 1.0f;
	float surface = 1.0f;
	float width = 1.0f;
	float height = 1.0f;
	float radius = 1.0f;

	ShapeType shape = ShapeType::RECTANGLE;
	BodyType type = STATIC;

	float maxVelocity = 1000.0f;

	fPoint totalForce = { 0.0, 0.0 };
	fPoint additionalForce = { 0.0, 0.0 };

	List<PhysicBody*> collisionList;
public:
	PhysicBody();

	~PhysicBody();

	PhysicBody(fPoint pos, float width, float height, BodyType type);

	PhysicBody(fPoint pos, float radius, BodyType type);

	PhysicBody(PhysicBody& copy);

	void OnCollision(PhysicBody* col);

	void OnCollisionTouch(PhysicBody* col);

	void OnCollisionLeave(PhysicBody* col);

	void AddForceToCenter(fPoint force);

	fPoint GetPosition()
	{
		return position;
	}
	void SetPosition(fPoint pos)
	{
		this->position = pos;
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
		velocity.x > maxVelocity ? maxVelocity : velocity.x < -maxVelocity ? -maxVelocity : velocity.x;
		velocity.y > maxVelocity ? maxVelocity : velocity.y < -maxVelocity ? -maxVelocity : velocity.y;

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
	int GetRadius()
	{
		return radius;
	}

	bool Contains(fPoint pos)
	{
		if (shape == ShapeType::RECTANGLE)
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

private:

	void ResetForces();

	friend class PhysWorld;
};

