#include "PhysicBody.h"

PhysicBody::PhysicBody()
{

}

PhysicBody::~PhysicBody()
{
}

PhysicBody::PhysicBody(fPoint pos, BodyType type, float width, float height, COL_TYPE colType)
{
	this->position = pos;
	this->type = type;
	this->shape = ShapeType::RECT;
	this->width = width;
	this->height = height;
	this->colType = colType;
}

PhysicBody::PhysicBody(fPoint pos, BodyType type, float radius, COL_TYPE colType)
{
	this->position = pos;
	this->type = type;
	this->shape = ShapeType::CIRCLE;
	this->radius = radius;
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
}

void PhysicBody::OnCollision(PhysicBody* col)
{
	printf("Col enter\n");
}

void PhysicBody::OnCollisionTouch(PhysicBody* col)
{
	printf("Col stay\n");
}

void PhysicBody::OnCollisionLeave(PhysicBody* col)
{
	printf("Col exit\n");
}

void PhysicBody::OnTriggerEnter(PhysicBody* col)
{
	printf("Trigger enter\n");
}

void PhysicBody::OnTriggerStay(PhysicBody* col)
{
	printf("Trigger stay\n");
}

void PhysicBody::OnTriggerExit(PhysicBody* col)
{
	printf("Trigger exit\n");
}

void PhysicBody::AddForceToCenter(fPoint force)
{
	additionalForce += force;
}

void PhysicBody::ResetForces()
{
	acceleration = totalForce = { 0,0 };
}
