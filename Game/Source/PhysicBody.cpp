#include "PhysicBody.h"

PhysicBody::PhysicBody()
{

}

PhysicBody::~PhysicBody()
{
}

PhysicBody::PhysicBody(fPoint pos, float width, float height, BodyType type)
{
	this->position = pos;
	this->type = type;
	this->width = width;
	this->height = height;
	this->shape = ShapeType::RECTANGLE;
}

PhysicBody::PhysicBody(fPoint pos, float radius, BodyType type)
{
	this->position = pos;
	this->type = type;
	this->radius = radius;
	this->shape = ShapeType::CIRCLE;
}

PhysicBody::PhysicBody(PhysicBody& cp)
{
	this->position = cp.position;
	this->acceleration = cp.acceleration;
	this->velocity = cp.velocity;
	this->friction = cp.friction;
	this->coefficientDrag = cp.coefficientDrag;
	this->mass = cp.mass;
	this->restitution = cp.restitution;
	this->rotation = cp.rotation;
	this->type = cp.type;
	this->width = cp.width;
	this->height = cp.height;
	this->radius = cp.radius;
	this->shape = cp.shape;
	this->gravityScale = cp.gravityScale;
}

void PhysicBody::OnCollision(PhysicBody* col)
{
}

void PhysicBody::OnCollisionTouch(PhysicBody* col)
{
}

void PhysicBody::OnCollisionLeave(PhysicBody* col)
{
}

void PhysicBody::AddForceToCenter(fPoint force)
{
	additionalForce += force;
}

void PhysicBody::ResetForces()
{
	acceleration = totalForce = { 0,0 };
}
