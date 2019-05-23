#pragma once
#include <vector>
#include "RigidBody.h"
#include "../../Common/Vector2.h"
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class CollisionVolume 
		{
		protected:
			RigidBody *obj;
		public:
			CollisionVolume();
			virtual Vector2 getPosition();
			RigidBody* getObj();
			virtual float getWidth();
			virtual float getHeight();
			~CollisionVolume();
		};
	}
}

