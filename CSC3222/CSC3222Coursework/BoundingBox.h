#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"
namespace NCL {
	namespace CSC3222 {
		class BoundingBox :
			public CollisionVolume
		{
		public:
			BoundingBox();
			BoundingBox(RigidBody* a);
			BoundingBox(RigidBody* a, int width, int height);
			~BoundingBox();
			virtual Vector2 getPosition();
			float getWidth();
			float getHeight();
		protected:
			Vector2 position;
			float width;
			float height;
		};
	}
}
