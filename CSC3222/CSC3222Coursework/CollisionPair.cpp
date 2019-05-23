#include "CollisionPair.h"

using namespace NCL;
using namespace CSC3222;


CollisionPair::CollisionPair()
{
	first = nullptr;
	second = nullptr;
}

CollisionPair::CollisionPair(CollisionVolume* first, CollisionVolume* second)
{
	this->first = first;
	this->second = second;
}

CollisionPair::~CollisionPair()
{
}
