#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody()	{
	inverseMass = 1.0f;
	elasticity  = 1.0f;
	dynamic = 1;
}

RigidBody::RigidBody(int type, Vector2 pos)
{
	
		inverseMass = 0.0f;
		elasticity = 1.0f;
		this->type = type;
		dynamic = 0;
		position = pos;
}

void RigidBody::setRoughGround()
{
	onRoughGround = true;
}

RigidBody::~RigidBody()	{

}
