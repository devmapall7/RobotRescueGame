#include "BoundingCircle.h"

using namespace NCL;
using namespace CSC3222;

BoundingCircle::BoundingCircle()
{

}
BoundingCircle::BoundingCircle(RigidBody* a)
{
	obj = a;
}
Vector2 BoundingCircle::getPosition()
{
	return obj->GetPosition() + Vector2(8,8);
}
BoundingCircle::~BoundingCircle()
{
}
