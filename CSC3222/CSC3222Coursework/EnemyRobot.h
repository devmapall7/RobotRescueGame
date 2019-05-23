#pragma once
#include "Robot.h"
#include "PlayerRobot.h"
#include "RobotRescueGame.h"

/*

Modes of operation:
1 - Patrol Mode
2 - Attack Mode
3 - Protect Mode

*/

namespace NCL {
	namespace CSC3222 {
		class EnemyRobot : public Robot {

		public:
			EnemyRobot();
			~EnemyRobot();

			bool UpdateObject(float dt) override;

			PlayerRobot* playerRobot;
			RobotRescueGame* game;
		protected:
			float	thinkTime;
		//	int mode;
			bool	moving;
		};
	}
}

