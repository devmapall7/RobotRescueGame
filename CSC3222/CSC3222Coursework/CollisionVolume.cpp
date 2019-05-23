#include "CollisionVolume.h"
#include "RigidBody.h"
#include "../../Common/Vector2.h"


using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume()
{

}
RigidBody* CollisionVolume::getObj()
{
	return obj;
}
Vector2 CollisionVolume::getPosition()
{
	return obj->GetPosition();
}
float CollisionVolume::getHeight()
{
	return 8;
}
float CollisionVolume::getWidth()
{
	return 8;
}
CollisionVolume::~CollisionVolume()
{
	delete obj;
}
