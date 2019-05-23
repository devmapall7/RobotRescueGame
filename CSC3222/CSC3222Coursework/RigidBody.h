#pragma once
#include "../../Common/Vector2.h"

/*Types:
1 - Wall
2 - Rough Ground
3 - Red Teleporter
4 - Blue Teleporter
5 - Green Teleporter
6 - Robot Home
7 - PlayerRobot
8 - EnemyRobot
9 - CollectableRobot
10 - Laser
*/


namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class RigidBody	{
			friend class GameSimsPhysics;
		public:
			RigidBody();
			RigidBody(int type, Vector2 pos);
			~RigidBody();

			Vector2 GetPosition() const {
				return position;
			}

			void SetPosition(const Vector2& newPosition) {
				position = newPosition;
			}

			Vector2 GetVelocity() const {
				return velocity;
			}

			virtual void SetCollected(const bool & col)
			{

			}

			virtual bool GetCollected()
			{
				return false;
			}

			void SetVelocity(const Vector2& newVelocity) {
				velocity = newVelocity;
			}

			void AddForce(const Vector2& newForce) {
				force += newForce;
			}

			void AddImpulse(const float& impulse, const Vector2& colNormal)
			{
				velocity += (colNormal*impulse) *inverseMass;
			}

			int GetType() const
			{
				return type;
			}
			void SetType(const int& newType)
			{
				type = newType;
			}
			bool GetDynamic() const
			{
				return dynamic;
			}
			void setRoughGround();

		protected:
			float speed = 0;
			bool onRoughGround = 0;

			Vector2 position;
			Vector2 velocity;
			Vector2 force;

			float inverseMass;
			float elasticity;

			int type;
			bool dynamic;
		};
	}
}
