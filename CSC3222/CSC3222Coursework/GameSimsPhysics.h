#pragma once
#include <vector>
#include"BoundingBox.h"
#include"BoundingCircle.h"
#include "../../Common/Vector2.h"
#include"CollisionPair.h"

namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void FixedPhysics(float realDT, std::vector<CollisionPair> & collisions);
			void Update(float dt, std::vector<CollisionPair> & collisions);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c, const bool& dynamic);
			void RemoveCollider(CollisionVolume* c, const bool& dynamic);
			void CollisionResolution(CollisionPair & a, float& penDistance, Vector2& colNormal);

		protected:
			void IntegrateAcceleration(float dt);
			void CollisionDetection(float dt, std::vector<CollisionPair> & collisions);
			
			void Projection(CollisionPair & a, float& penDistance, Vector2& colNormal);
			void Impulse(CollisionPair & a, Vector2& colNormal);
			void IntegrateVelocity(float dt);
			void ResetForce();
			void BroadPhase(std::vector<CollisionPair>& collisions);
			void DynamicStaticCollision(std::vector<CollisionPair>& collisions);
			void DynamicDynamicCollision(std::vector<CollisionPair>& collisions);
			void InsertionSort();
			bool CheckCollision(CollisionPair & a, float& penDistance, Vector2& colNormal);
			bool CircleCircleCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal);
			bool BoxBoxCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal);
			bool CircleBoxCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal);
			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	dynamicColliders;
			std::vector<CollisionVolume*>	staticColliders;
			bool staticSorted = false;
		};
	}
}

