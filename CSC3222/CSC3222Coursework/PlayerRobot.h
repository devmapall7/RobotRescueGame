#pragma once
#include "Robot.h"
#include <vector>
namespace NCL {
	namespace CSC3222 {
		class PlayerRobot : public Robot	{
		public:
			PlayerRobot();
			~PlayerRobot();

			bool UpdateObject(float dt) override;

			std::vector<RigidBody*>	collected;
		};
	}
}

