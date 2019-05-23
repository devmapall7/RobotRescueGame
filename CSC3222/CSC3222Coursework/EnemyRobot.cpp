#include "EnemyRobot.h"
#include "TextureManager.h"
#include"GameMap.h"
#include "../../Common/TextureLoader.h"
using namespace NCL;
using namespace CSC3222;
//using namespace std;
using namespace Rendering;

EnemyRobot::EnemyRobot() : Robot()	{
	texture = texManager->GetTexture("Turret Bot overgrown.png");
	speed = 1024;
	moving = false;
}

EnemyRobot::~EnemyRobot()	{
}

bool EnemyRobot::UpdateObject(float dt) {
	thinkTime -= dt;

	if (moving) {
		UpdateAnimFrame(dt);
	}
	
	if (thinkTime < 0) {
		moving		= false;
		velocity	= Vector2();

		thinkTime += 10.5f;

		Vector2 testForce(0,0);

		float rough = 1;

		if (onRoughGround) rough = 0.3;

		float testX = this->GetPosition().x - playerRobot->GetPosition().x;
		float testY = this->GetPosition().y - playerRobot->GetPosition().y;
		float distance = testX * testX + testY * testY;
		// Attack mode
		if (distance<6400) {
			Vector2 newPos = Vector2(playerRobot->GetPosition().x / 16, playerRobot->GetPosition().y / 16);
			std::vector<Vector2> path;
			if (game->GetMap()->GeneratePath(this->GetPosition(), newPos, path));
			moving = true;
			testForce.x = -speed*rough;
			AddForce(testForce);
			currentAnimDir = MovementDir::Left;
		}
		// Patrol mode
		else  {
			moving = true;
			// Protect mode
			if (game->BonusPresent())
			{
				Vector2 newPos = game->GetMap()->GetRandomPositionWithinRadius(game->GetBonusPos(),64);
				std::vector<Vector2> path;
				if (game->GetMap()->GeneratePath(this->GetPosition(), newPos, path));
			}
			else
			{
				Vector2 newPos = game->GetMap()->GetRandomPosition();
				std::vector<Vector2> path;
				//cout << newPos;
				if (game->GetMap()->GeneratePath(this->GetPosition(), newPos, path))
				{
					Vector2 d = this->GetPosition() - newPos;
			//		cout << d << endl;
					Vector2 n = d;
					n.Normalise();
					int i = 0;
					do
					{
						testForce = n * 256 * rough;
						AddForce(testForce);
					} while (n.x > 0.1 && n.y > 0.1);
				}
			//	cout << path.size() << endl;
				
			}
			testForce.x = speed*rough;
			currentAnimDir = MovementDir::Right;
		}

		/*if (choice == 3) {
			moving = true;
			testForce.y = -speed*rough;
			AddForce(testForce);
			currentAnimDir = MovementDir::Up;
		}

		if (choice == 4) {
			moving = true;
			testForce.y = speed*rough;
			AddForce(testForce);
			currentAnimDir = MovementDir::Down;
		}*/
	}
	onRoughGround = 0;
	return true;
}