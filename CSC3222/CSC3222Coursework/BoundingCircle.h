#pragma once
#include "CollisionVolume.h"
namespace NCL {
	namespace CSC3222 {
		class BoundingCircle :
			public CollisionVolume
		{
		public:
			BoundingCircle();
			virtual Vector2 getPosition();
			BoundingCircle(RigidBody* a);
			~BoundingCircle();
		};
	}
}

