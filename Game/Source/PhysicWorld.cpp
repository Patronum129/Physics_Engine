#include "PhysicWorld.h"
#include <algorithm>

PhysWorld::PhysWorld(fPoint gravity)
{
	this->gravity = gravity;
}

PhysWorld::~PhysWorld()
{
	physicBodies.clear();
}

void PhysWorld::Update(float dt)
{
	for (int i = 0; i < physicBodies.count(); i++)
	{
		if (physicBodies[i]->type == BodyType::STATIC || physicBodies[i]->type == BodyType::WATER || physicBodies[i] == nullptr) continue;

		// Step #0 Reset acceleration and forces
		physicBodies[i]->ResetForces();

		// Step #1 Calculate Forces (TotalForces = GravityForce + AdditionalForce)
		//	gravity
		physicBodies[i]->AddForceToCenter(gravity * physicBodies[i]->gravityScale * physicBodies[i]->GetMass());

		//	Drag	(0.5 * density * relative velocity square * surface * Drag coeficient)
		fPoint dragForce;
		fPoint relativeVelocity;
		float relativeVelocityModule;
		float magnitudDrag;

		// Calcular velocidad relativa entre viento y body
		relativeVelocity.x = air.x - physicBodies[i]->GetLinearVelocity().x;
		relativeVelocity.y = air.y - physicBodies[i]->GetLinearVelocity().y;

		// Calcular el modulo de la velocidad relativa
		relativeVelocityModule = relativeVelocity.Module();

		// Calcular el magnitud del drag
		magnitudDrag = 0.5f * density * physicBodies[i]->surface * pow(relativeVelocityModule, 2) * physicBodies[i]->GetDragCoeficient();

		fPoint relativeDir = relativeVelocity.Normalize();

		// Calcular la fuerza de drag
		dragForce = relativeDir * magnitudDrag;

		physicBodies[i]->AddForceToCenter(dragForce);

		// Chek if collision Stay
		for (int j = 0; j < physicBodies[i]->collisionList.count(); j++)
		{
			if (physicBodies[i]->collisionList[j]->type == BodyType::WATER)
			{
				//Buoyancy (Density * gravity * area of the object flooded)
				fPoint buoyancyForce;
				float magnitudbuoyancy;
				fPoint direct = { 0, -10 };
				fPoint direction = direct.Normalize();

				float mod = gravity.Module();

				// 1 = 100%
				float submerge = submergedVolume(physicBodies[i], physicBodies[i]->collisionList[j]);

				printf("%f\n", submerge);

				// *2 = 200% para que pueda subir
				magnitudbuoyancy = density * mod * submerge * 2 * physicBodies[i]->GetGravityScale();

				buoyancyForce = direction * magnitudbuoyancy;

				physicBodies[i]->AddForceToCenter(buoyancyForce);

				// Calcular la fuerza de drag hidrodinamica
				dragForce = (physicBodies[i]->velocity * -1) * physicBodies[i]->hydrodynamicDrag;

				physicBodies[i]->AddForceToCenter(dragForce);
			}
			else
			{
				float submerge = submergedVolume(physicBodies[i], physicBodies[i]->collisionList[j]);

				if (submerge >= 1)
				{

				}

				fPoint colPoint = CollisionPoint(*physicBodies[i], *physicBodies[i]->collisionList[j]);
				ResolveColForce(*physicBodies[i], *physicBodies[i]->collisionList[j], colPoint);
			}
		}

		physicBodies[i]->totalForce = physicBodies[i]->additionalForce;
		physicBodies[i]->additionalForce = { 0,0 };

		// Step #2 Calculate Newton's Second law (acceleration)
		physicBodies[i]->acceleration = physicBodies[i]->totalForce / physicBodies[i]->mass;

		// Check if next position is colling
		fPoint nextPosition = physicBodies[i]->position + physicBodies[i]->velocity * dt + physicBodies[i]->acceleration * (dt * dt * 0.5f);

		if (abs(physicBodies[i]->velocity.x) < 0.1f) physicBodies[i]->velocity.x = 0;
		if (abs(physicBodies[i]->velocity.y) < 0.1f) physicBodies[i]->velocity.y = 0;

		// Step #3 Integrate with Verlet
		physicBodies[i]->lastPosition = physicBodies[i]->position;
		physicBodies[i]->position += physicBodies[i]->velocity * dt + physicBodies[i]->acceleration * (dt * dt * 0.5f);
		physicBodies[i]->velocity += physicBodies[i]->acceleration * dt;
	}

	// Despues de mover todos los objetos comparan la colision.
	for (int i = 0; i < physicBodies.count(); i++)
	{
		if (physicBodies[i]->type == BodyType::DYNAMIC)
		{
			// Step #4: solve collisions
			CheckCollision(physicBodies[i]);
		}
	}
}

bool PhysWorld::CheckCollision(PhysicBody* body)
{
	// Check if body is colliding with any other body on rigidBodies
	for (int i = 0; i < physicBodies.count(); i++)
	{
		// If not is self
		if (i != physicBodies.find(body))
		{
			if (body->shape == ShapeType::RECTANGLE && physicBodies[i]->shape == ShapeType::RECTANGLE)
			{
				BoxColBox(*body, *physicBodies[i]);
			}
			else if (body->shape == ShapeType::CIRCLE && physicBodies[i]->shape == ShapeType::CIRCLE)
			{
				CircleColCircle(*body, *physicBodies[i]);
			}
			else // CIRCLE col RECT || RECT col CIRCLE
			{
				BoxColCircle(*body, *physicBodies[i]);
			}
		}
	}
	return true;
}

void PhysWorld::AddPhysicBody(PhysicBody* body)
{
	physicBodies.add(body);
}

void PhysWorld::DelPhysicBody(PhysicBody* body)
{
	physicBodies.del(physicBodies.At(physicBodies.find(body)));
}

COL_TYPE PhysWorld::BoxColBox(PhysicBody& b1, PhysicBody& b2, bool trigger)
{
	// No collision case
	if (b1.position.x > b2.position.x + b2.width ||
		b1.position.x + b1.width < b2.position.x ||
		b1.position.y > b2.position.y + b2.height ||
		b1.position.y + b1.height < b2.position.y)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			// Collision Exit
			if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionLeave(&b2);
				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
				;
			}
		}
		return NONE;
	}

	// Collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		if (trigger)
		{
			return TRIGGER;
		}
		// Collision stay
		if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
		{
			b1.OnCollisionTouch(&b2);
			ResolveColForce(b1, b2, CollisionPoint(b1, b2));
			return COLLISION;
		}
	}

	// Collision Enter
	b1.collisionList.add(&b2);
	b1.OnCollision(&b2);
	ResolveColForce(b1, b2, CollisionPoint(b1, b2));
	return COLLISION;
}

COL_TYPE PhysWorld::CircleColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger)
{
	float distX = b1.position.x - b2.position.x;
	float distY = b1.position.y - b2.position.y;
	float distance = sqrt(pow(distX, 2) + pow(distY, 2));

	//Collision
	if (distance <= b1.radius + b2.radius)
	{
		if (trigger)
		{
			return TRIGGER;
		}
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionTouch(&b2);
				return COLLISION;
			}
		}

		b1.collisionList.add(&b2);
		b1.OnCollision(&b2);

		fPoint colPoint = CollisionPoint(b1, b2);

		ResolveColForce(b1, b2, colPoint);
		return COLLISION;
	}

	//No Collision
	if (distance > b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionLeave(&b2);
				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
			}
		}
	}
}

COL_TYPE PhysWorld::BoxColCircle(PhysicBody& b1, PhysicBody& b2, bool trigger)
{
	PhysicBody* circ;
	PhysicBody* rect;
	fPoint colPoint;

	float height;
	float width;
	float distance;

	if (b1.shape == ShapeType::RECTANGLE)
	{
		rect = &b1;
		circ = &b2;
	}
	else
	{
		rect = &b2;
		circ = &b1;
	}

	colPoint = CollisionPoint(*circ, *rect);

	distance = (colPoint - circ->GetPosition()).magnitude();

	// Collision case
	if (distance <= circ->GetRadius())
	{
		if (trigger)
		{
			return TRIGGER;
		}

		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionTouch(&b2);
				ResolveColForce(b1, b2, colPoint);
				return COLLISION;
			}
		}
		b1.collisionList.add(&b2);
		b1.OnCollision(&b2);
		ResolveColForce(b1, b2, colPoint);
		return COLLISION;
	}

	// Cliping case

	// No collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		if (physicBodies.find(b1.collisionList[i]) == physicBodies.find(&b2))
		{
			b1.OnCollisionLeave(&b2);
			b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
		}
	}

	return NONE;
}

/// <summary>
/// Just resolve Dynamic body && Static body
/// </summary>
/// <param name="b1"></param>
/// <param name="b2"></param>
/// <param name="colPoint"></param>
void PhysWorld::ResolveColForce(PhysicBody& b1, PhysicBody& b2, fPoint colPoint)
{
	PhysicBody* dinBody;
	PhysicBody* staticBody;

	if (b1.type == BodyType::DYNAMIC && b2.type == BodyType::STATIC)
	{
		dinBody = &b1;
		staticBody = &b2;
	}
	else if (b2.type == BodyType::DYNAMIC && b1.type == BodyType::STATIC)
	{
		dinBody = &b2;
		staticBody = &b1;
	}
	else if (b2.type == BodyType::WATER || b1.type == BodyType::WATER)
	{
		dinBody = &b1;
		staticBody = &b2;
		return;
	}
	else
	{
		printf("Canno't resolve collision force!");
		return;
	}

	fPoint b1Vel = dinBody->GetLinearVelocity();

	fPoint colCondition;

	// init Col Point codition
	if (b1.shape == b2.shape)
	{
		if (b1.shape == ShapeType::CIRCLE)
		{
			// circle && circle
			//colCondition = colPoint;

			colCondition = CollisionDir(*dinBody, colPoint);

			fPoint direction = CollisionDir(*dinBody, colPoint);
			float velMagnitud = b1Vel.magnitude();
			b1Vel = direction * velMagnitud * b1.restitution;
		}

		if (b1.shape == ShapeType::RECTANGLE)
		{
			printf("Can not resolve force rect & rect");
			return;
		}
	}
	else
	{
		// circle && rect
		colCondition = CollisionDir(*dinBody, colPoint);

		if (abs(colCondition.x) > abs(colCondition.y))
		{
			// colision en eje x
			if (colCondition.x > 0 && b1Vel.x <= 0 || colCondition.x < 0 && b1Vel.x >= 0)
			{
				//if (b1Vel.x == 0)b1Vel.x = resolveColForce * colCondition.x;
				b1Vel.x *= -dinBody->restitution;
			}
		}
		else
		{
			if (colCondition.y < 0 && b1Vel.y >= 0 || colCondition.y > 0 && b1Vel.y <= 0)
			{
				//if (b1Vel.y == 0)b1Vel.y = -resolveColForce * colCondition.y;
				b1Vel.y *= -dinBody->restitution;
			}
		}
	}

	if (colCondition.y > 0 && dinBody->acceleration.y < 0 || colCondition.y < 0 && dinBody->acceleration.y > 0)
	{
		b1.acceleration.y = 0;
	}
	if (colCondition.x > 0 && dinBody->acceleration.x < 0 || colCondition.x < 0 && dinBody->acceleration.x > 0)
	{
		b1.acceleration.x = 0;
	}

	// Si el movimiento es muy flojo, ignora
	b1Vel.x = abs(b1Vel.x) < dinBody->restitution * 10 ? 0 : b1Vel.x;
	b1Vel.y = abs(b1Vel.y) < dinBody->restitution * 10 ? 0 : b1Vel.y;

	dinBody->SetLinearVelocity(b1Vel);
}

fPoint PhysWorld::CollisionPoint(PhysicBody& b1, PhysicBody& b2)
{
	fPoint collisionPoint;

	// Check RECT RECT collision point
	if (b1.shape == ShapeType::RECTANGLE && b2.shape == ShapeType::RECTANGLE)
	{
		// PUNTO DE COLISION???
		collisionPoint = { b1.velocity.x, b1.velocity.y };

		// col eje X
		if (b1.velocity.y == 0)
		{
			if (b1.velocity.x > 0)
			{
				collisionPoint.x += b1.width;
			}
			else if (b1.velocity.x < 0)
			{
				collisionPoint.x -= b1.width;
			}
		}
		// col eje y
		else if (b1.velocity.x == 0)
		{
			if (b1.velocity.y > 0)
			{
				collisionPoint.x += b1.height;
			}
			else if (b1.velocity.y < 0)
			{
				collisionPoint.x -= b1.height;
			}
		}

		// NO FUNCIONA FALTA CASO DE MOVIMIENTO DIAGONAL
	}
	// Check CIRCLE CIRCLE collision point
	if (b1.shape == ShapeType::CIRCLE && b2.shape == ShapeType::CIRCLE)
	{
		fPoint dir = b1.GetPosition() - b2.GetPosition();

		dir *= -1;
		// PUNTO DE COLISION!!!!!
		collisionPoint = b1.GetPosition() + dir.Normalize() * b1.radius;
	}
	// Check CIRCLE RECT collision point
	else // CIRCLE col RECT || RECT col CIRCLE
	{
		float height = b2.height * 0.5f;
		float width = b2.width * 0.5f;

		collisionPoint = b1.GetPosition() - b2.GetPosition();

		collisionPoint.x = MAX(-width, MIN(width, collisionPoint.x));
		collisionPoint.y = MAX(-height, MIN(height, collisionPoint.y));

		// PUNTO DE COLISION!!!!!
		collisionPoint = b2.GetPosition() + collisionPoint;
	}
	return collisionPoint;
}

fPoint PhysWorld::CollisionDir(PhysicBody& b1, fPoint colPoint)
{
	fPoint dir = b1.GetPosition() - colPoint;

	dir = dir.Normalize();

	return dir;
}

void PhysWorld::ResolveClamping(PhysicBody& b1, PhysicBody& b2)
{
	PhysicBody* dinBody = nullptr;
	PhysicBody* staticBody = nullptr;

	if (b1.type == BodyType::DYNAMIC && b2.type == BodyType::STATIC)
	{
		dinBody = &b1;
		staticBody = &b2;
	}
	else if (b2.type == BodyType::DYNAMIC && b1.type == BodyType::STATIC)
	{
		dinBody = &b2;
		staticBody = &b1;
	}

	if (dinBody->shape != ShapeType::CIRCLE || staticBody->shape != ShapeType::RECTANGLE)
	{
		printf("Can not resolve clamping");
		return;
	}

	// calcular interseccion de linea de last position->position y static body


}

float PhysWorld::submergedVolume(PhysicBody* body, PhysicBody* water)
{
	if (body->shape == ShapeType::CIRCLE)
	{
		//Obtain the water Y position substracting his height because the pos of the body is in the center
		float waterYpos = water->GetPosition().y - water->height / 2;

		float bodySubmergedHeight = body->GetPosition().y + body->radius - waterYpos;

		if (bodySubmergedHeight > body->radius * 2) return 1.0f;

		float totalsubmergedarea = bodySubmergedHeight * body->radius * 2;

		totalsubmergedarea /= (body->radius * 2 * body->radius * 2);

		return totalsubmergedarea;
	}

	if (body->shape == ShapeType::RECTANGLE)
	{
		//Obtain the water Y position substracting his height because the pos of the body is in the center
		float waterYpos = water->GetPosition().y - water->height / 2;

		float bodySubmergedHeight = body->GetPosition().y + body->height / 2 - waterYpos;

		float totalsubmergedarea = bodySubmergedHeight * body->width;

		totalsubmergedarea /= body->width * body->height;

		return totalsubmergedarea;
	}
}