#pragma once
#include"CollisionVolume.h"
namespace NCL {
	namespace CSC3222 {
		class CollisionPair
		{
		public:
			CollisionPair();
			CollisionPair(CollisionVolume* first, CollisionVolume* second);
			~CollisionPair();
			CollisionVolume *first;
			CollisionVolume *second;
		};
	}
}

