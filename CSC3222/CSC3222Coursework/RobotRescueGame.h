#pragma once
#include <vector>
#include"../../Common/Vector2.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class PlayerRobot;
		class TextureManager;

		class RobotRescueGame {
		public:
			RobotRescueGame();
			~RobotRescueGame();

			void Update(float dt);

			void AddNewObject(SimObject* object, int type);

			bool BonusPresent();

			Maths::Vector2 GetBonusPos();

			GameMap* GetMap() ;

		protected:
			void InitialiseGame();

			void AddEnemyRobot(const Maths::Vector2& position);
			void AddCollectableRobot(const Maths::Vector2& position);

			TextureManager*		texManager;
			GameSimsRenderer*	renderer;
			GameSimsPhysics*	physics;
			GameMap* currentMap;

			PlayerRobot* testRobot;

			float gameTime;

			int currentScore;
			int lives;

			bool bonusRobot;
			Maths::Vector2 bonusPos;

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;
		};
	}
}

