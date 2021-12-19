#include "PhysicBody.h"
#include "GameObject.h"

PhysicBody::PhysicBody()
{

}

PhysicBody::~PhysicBody()
{
}

PhysicBody::PhysicBody(iPoint pos, BodyType type, float width, float height, GameObject* gObj, COL_TYPE colType)
{
	this->position.x = PIXELS_TO_METERS(pos.x);
	this->position.y = PIXELS_TO_METERS(pos.y);
	this->type = type;
	this->shape = ShapeType::RECT;
	this->width = PIXELS_TO_METERS(width);
	this->height = PIXELS_TO_METERS(height);
	this->gObject = gObj;
	this->colType = colType;
}

PhysicBody::PhysicBody(iPoint pos, BodyType type, float radius, GameObject* gObj, COL_TYPE colType)
{
	this->position.x = PIXELS_TO_METERS(pos.x);
	this->position.y = PIXELS_TO_METERS(pos.y);
	this->type = type;
	this->shape = ShapeType::CIRCLE;
	this->radius = PIXELS_TO_METERS(radius);
	this->gObject = gObj;
	this->colType = colType;
}

PhysicBody::PhysicBody(PhysicBody& copy)
{
	this->position = copy.position;
	this->acceleration = copy.acceleration;
	this->velocity = copy.velocity;
	this->friction = copy.friction;
	this->coefficientDrag = copy.coefficientDrag;
	this->mass = copy.mass;
	this->restitution = copy.restitution;
	this->rotation = copy.rotation;
	this->type = copy.type;
	this->width = copy.width;
	this->height = copy.height;
	this->radius = copy.radius;
	this->shape = copy.shape;
	this->gravityScale = copy.gravityScale;
	this->gObject = copy.gObject;
	this->colType = copy.colType;
}

void PhysicBody::OnCollisionEnter(PhysicBody* col)
{
	//printf("Col enter\n");
	if (gObject != nullptr)
	{
		gObject->OnCollisionEnter(col);
	}
}

void PhysicBody::OnCollisionStay(PhysicBody* col)
{
	//printf("Col stay\n");
}

void PhysicBody::OnCollisionExit(PhysicBody* col)
{
	//printf("Col exit\n");
	if (gObject != nullptr)
	{
		gObject->OnCollisionExit(col);
	}
}

void PhysicBody::OnTriggerEnter(PhysicBody* col)
{
	//printf("Trigger enter\n");
	if (gObject != nullptr)
	{
		gObject->OnTriggerEnter(col);
	}
}

void PhysicBody::OnTriggerStay(PhysicBody* col)
{
	//printf("Trigger stay\n");
}

void PhysicBody::OnTriggerExit(PhysicBody* col)
{
	//printf("Trigger exit\n");
	if (gObject != nullptr)
	{
		gObject->OnTriggerExit(col);
	}
}

void PhysicBody::AddForceToCenter(fPoint force)
{
	additionalForce += force;
}

void PhysicBody::ResetForces()
{
	acceleration = totalForce = { 0,0 };
}