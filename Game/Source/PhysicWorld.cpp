#include "PhysicWorld.h"
#include <algorithm>

PhysicWorld::PhysicWorld(fPoint gravity)
{
	this->gravity = gravity;
}

PhysicWorld::~PhysicWorld()
{
	physicBodies.clear();
}

void PhysicWorld::Update(float simulationTime)
{
	for (int i = 0; i < physicBodies.count(); i++)
	{
		if (physicBodies[i]->type == BodyType::STATIC || physicBodies[i]->type == BodyType::WATER || physicBodies[i] == nullptr) continue;

		// Step #0 Reset acceleration and forces
		physicBodies[i]->ResetForces();

		// Step #1 Calculate Forces (TotalForces = GravityForce + AdditionalForce)
		//	gravity
		if (gravityOn)
		{
			physicBodies[i]->AddForceToCenter(gravity * physicBodies[i]->gravityScale * physicBodies[i]->mass);
		}

		//	Drag	(0.5 * density * relative velocity square * surface * Drag coeficient)
		if (aeroDragOn)
		{
			fPoint dragForce;
			fPoint relativeVelocity;
			float relativeVelocityModule;
			float magnitudDrag;

			// Calcular velocidad relativa entre viento y body
			relativeVelocity.x = 0 - physicBodies[i]->GetLinearVelocity().x;
			relativeVelocity.y = 0 - physicBodies[i]->GetLinearVelocity().y;

			// Calcular el modulo de la velocidad relativa
			relativeVelocityModule = relativeVelocity.Module();

			// Calcular el magnitud del drag
			magnitudDrag = 0.5f * density * 1 * pow(relativeVelocityModule, 2) * physicBodies[i]->coefficientDrag;

			fPoint nor = relativeVelocity.Normalize();

			// Calcular la fuerza de drag
			dragForce = nor * magnitudDrag;

			physicBodies[i]->AddForceToCenter(dragForce);
		}

		physicBodies[i]->totalForce = physicBodies[i]->additionalForce;
		physicBodies[i]->additionalForce = { 0,0 };

		// Step #2 Calculate Newton's Second law (acceleration)
		physicBodies[i]->acceleration = physicBodies[i]->totalForce / physicBodies[i]->mass;

		// Check if collision Stay
		for (int j = 0; j < physicBodies[i]->colList.count(); j++)
		{
			// Collision
			
			if (physicBodies[i]->colList[j]->type == BodyType::WATER && buoyancyOn)
			{
				//Buoyancy (Density * gravity * area of the object flooded)
				fPoint buoyancyForce;
				float magnitudbuoyancy;
				fPoint direct = { 0, -10 };
				fPoint direction = direct.Normalize();

				float mod = gravity.Module();

				// 1 = 100%
				float submerge = 0.5;

				//printf("%f\n", submerge);

				// *2 = 200% para que pueda subir
				magnitudbuoyancy = density * mod * submerge * 2 * physicBodies[i]->gravityScale;

				buoyancyForce = direction * magnitudbuoyancy;

				physicBodies[i]->AddForceToCenter(buoyancyForce);

				// Calcular la fuerza de drag hidrodinamica
				if (hydrioDragOn)
				{
					fPoint dragForce = (physicBodies[i]->velocity * -1) * physicBodies[i]->hydrodynamicDrag;

					physicBodies[i]->AddForceToCenter(dragForce);
				}
			}
			else
			{
				//FUERZA DE FRICCIÓN
				if (physicBodies[i]->colList[j]->type == BodyType::STATIC && frictioOn)
				{
					fPoint dragForce = (physicBodies[i]->GetLinearVelocity() * -1) * physicBodies[i]->friction;

					physicBodies[i]->AddForceToCenter(dragForce);
				}
				//IMPEDIR QUE ENTRE DENTRO DE UN RIGIDBODY
				fPoint colPoint = CollisionPoint(*physicBodies[i], *physicBodies[i]->colList[j]);
				ResolveColForce(*physicBodies[i], *physicBodies[i]->colList[j], colPoint);

			
			}
		}

		// If velocity is few, ignore
		if (abs(physicBodies[i]->velocity.x) < 0.1f) physicBodies[i]->velocity.x = 0;
		if (abs(physicBodies[i]->velocity.y) < 0.1f) physicBodies[i]->velocity.y = 0;

		// Save lastPosition
		physicBodies[i]->lastPosition = physicBodies[i]->position;
		// Step #3 Integrate with Verlet	
		physicBodies[i]->position += physicBodies[i]->velocity * simulationTime + physicBodies[i]->acceleration * (simulationTime * simulationTime * 0.5f);
		physicBodies[i]->velocity += physicBodies[i]->acceleration * simulationTime;
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

bool PhysicWorld::CheckCollision(PhysicBody* body)
{
	// Check if body is colliding with any other body on rigidBodies
	for (int i = 0; i < physicBodies.count(); i++)
	{
		// If not is self
		if (i != physicBodies.find(body))
		{
			if (body->shape == ShapeType::RECT && physicBodies[i]->shape == ShapeType::RECT)
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

void PhysicWorld::AddPhysicBody(PhysicBody* body)
{
	physicBodies.add(body);
}

void PhysicWorld::DelPhysicBody(PhysicBody* body)
{
	physicBodies.del(physicBodies.At(physicBodies.find(body)));
}

void PhysicWorld::BoxColBox(PhysicBody& b1, PhysicBody& b2)
{
	// No collision case
	if (b1.position.x > b2.position.x + b2.width ||
		b1.position.x + b1.width < b2.position.x ||
		b1.position.y > b2.position.y + b2.height ||
		b1.position.y + b1.height < b2.position.y)
	{
		for (int i = 0; i < b1.colList.count(); i++)
		{
			// Collision Exit
			if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionExit(&b2);
				
				b1.colList.remove(b1.colList.At(b1.colList.find(&b2)));
				return;
			}
		}
	}

	// Collision case
	for (int i = 0; i < b1.colList.count(); i++)
	{
		// Collision stay
		if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
		{
			b1.OnCollisionStay(&b2);
			ResolveColForce(b1, b2, CollisionPoint(b1, b2));
			return;
		}
	}

	// Collision Enter
	b1.OnCollisionEnter(&b2);
	ResolveColForce(b1, b2, CollisionPoint(b1, b2));
	b1.colList.add(&b2);

	return;
}

void PhysicWorld::CircleColCircle(PhysicBody& b1, PhysicBody& b2)
{
	float distX = b1.position.x - b2.position.x;
	float distY = b1.position.y - b2.position.y;
	float distance = sqrt(pow(distX, 2) + pow(distY, 2));

	//Collision
	if (distance <= b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.colList.count(); i++)
		{
			if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionStay(&b2);
				fPoint colPoint = CollisionPoint(b1, b2);
				ResolveColForce(b1, b2, colPoint);

				return;
			}
		}

		b1.colList.add(&b2);

		b1.OnCollisionEnter(&b2);
		fPoint colPoint = CollisionPoint(b1, b2);
		ResolveColForce(b1, b2, colPoint);

		return;
	}

	//No Collision
	if (distance > b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.colList.count(); i++)
		{
			if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionExit(&b2);
				
				b1.colList.remove(b1.colList.At(b1.colList.find(&b2)));

				return;
			}
		}
	}
}

void PhysicWorld::BoxColCircle(PhysicBody& b1, PhysicBody& b2)
{
	PhysicBody* circ;
	PhysicBody* rect;
	fPoint colPoint;

	float height;
	float width;
	float distance;

	if (b1.shape == ShapeType::RECT)
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

	distance = (colPoint - circ->position).magnitude();

	// Collision case
	if (distance <= circ->radius)
	{
		for (int i = 0; i < b1.colList.count(); i++)
		{
			if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
			{
				b1.OnCollisionStay(&b2);
				ResolveColForce(b1, b2, colPoint);
				return;
			}
		}

		b1.colList.add(&b2);

		b1.OnCollisionEnter(&b2);
		ResolveColForce(b1, b2, colPoint);
		return;
	}

	// No collision case
	for (int i = 0; i < b1.colList.count(); i++)
	{
		if (physicBodies.find(b1.colList[i]) == physicBodies.find(&b2))
		{
			b1.OnCollisionExit(&b2);

			b1.colList.remove(b1.colList.At(b1.colList.find(&b2)));

			return;
		}
	}
}

void PhysicWorld::ResolveColForce(PhysicBody& b1, PhysicBody& b2, fPoint colPoint)
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
		//printf("Canno't resolve collision force!");
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

		if (b1.shape == ShapeType::RECT)
		{
			//printf("Can not resolve force rect & rect");
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
	b1Vel.x = abs(b1Vel.x) < 1 ? 0 : b1Vel.x;
	b1Vel.y = abs(b1Vel.y) < 1 ? 0 : b1Vel.y;

	dinBody->SetLinearVelocity(b1Vel);
}

fPoint PhysicWorld::CollisionPoint(PhysicBody& b1, PhysicBody& b2)
{
	fPoint collisionPoint;

	// Check RECT RECT collision point
	if (b1.shape == ShapeType::RECT && b2.shape == ShapeType::RECT)
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
		fPoint dir = b1.position - b2.position;

		dir *= -1;
		// PUNTO DE COLISION!!!!!
		collisionPoint = b1.position + dir.Normalize() * b1.radius;
	}
	// Check CIRCLE RECT collision point
	else // CIRCLE col RECT || RECT col CIRCLE
	{
		float height = b2.height * 0.5f;
		float width = b2.width * 0.5f;

		collisionPoint = b1.position - b2.position;

		collisionPoint.x = MAX(-width, MIN(width, collisionPoint.x));
		collisionPoint.y = MAX(-height, MIN(height, collisionPoint.y));

		// PUNTO DE COLISION!!!!!
		collisionPoint = b2.position + collisionPoint;
	}
	return collisionPoint;
}

fPoint PhysicWorld::CollisionDir(PhysicBody& b1, fPoint colPoint)
{
	fPoint dir = b1.position - colPoint;

	dir = dir.Normalize();

	return dir;
}