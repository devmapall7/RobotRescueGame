#include "RobotRescueGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerRobot.h"
#include "EnemyRobot.h"
#include "CollectableRobot.h"
#include "BoundingCircle.h"

#include"../../Common/Vector2.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
//using namespace std;
using namespace CSC3222;

RobotRescueGame::RobotRescueGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics();
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

RobotRescueGame::~RobotRescueGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}

GameMap* RobotRescueGame::GetMap() 
{
	return currentMap;
}

void RobotRescueGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;
	std::vector<CollisionPair> collisions;

	renderer->Update(dt);

	physics->FixedPhysics(dt, collisions);

	for (int i = 0; i < collisions.size(); i++)
	{
		if (collisions.at(i).first->getObj()->GetType() == 2)
		{
			collisions.at(i).second->getObj()->setRoughGround();
		}
		else if (collisions.at(i).second->getObj()->GetType() == 2)
		{
			collisions.at(i).first->getObj()->setRoughGround();
		}
		else if (collisions.at(i).first->getObj()->GetType() == 3)
		{
			Vector2 newPos(312, 24);
			collisions.at(i).second->getObj()->SetPosition(newPos);
	
		}
		else if (collisions.at(i).second->getObj()->GetType() == 3)
		{
			Vector2 newPos(312, 24);
			collisions.at(i).first->getObj()->SetPosition(newPos);
		
		}
		else if (collisions.at(i).first->getObj()->GetType() == 4)
		{
			Vector2 newPos(408, 264);
			collisions.at(i).second->getObj()->SetPosition(newPos);
	
		}
		else if (collisions.at(i).second->getObj()->GetType() == 4)
		{
			Vector2 newPos(408, 264);
			collisions.at(i).first->getObj()->SetPosition(newPos);
	
		}
		else if (collisions.at(i).first->getObj()->GetType() == 5)
		{
			Vector2 newPos(24, 56);
			collisions.at(i).second->getObj()->SetPosition(newPos);
		
		}
		else if (collisions.at(i).second->getObj()->GetType() == 5)
		{
			Vector2 newPos(24, 56);
			collisions.at(i).first->getObj()->SetPosition(newPos);
		}
		else if (collisions.at(i).first->getObj()->GetType() == 7 && collisions.at(i).second->getObj()->GetType() == 9)
		{
			if (!collisions.at(i).second->getObj()->GetCollected())
			{
				collisions.at(i).second->getObj()->SetCollected(true);
				testRobot->collected.emplace_back(collisions.at(i).second->getObj());
			}
		}
		else if (collisions.at(i).second->getObj()->GetType() == 7 && collisions.at(i).first->getObj()->GetType() == 9)
		{
			if (!collisions.at(i).first->getObj()->GetCollected())
			{
				collisions.at(i).first->getObj()->SetCollected(true);
				testRobot->collected.emplace_back(collisions.at(i).first->getObj());
			}
		}
		else if (collisions.at(i).second->getObj()->GetType() == 7 && collisions.at(i).first->getObj()->GetType() == 6)
		{
			if (testRobot->collected.size() > 0)
			{
				for (int i = 0; i < testRobot->collected.size(); i++)
				{
					testRobot->collected.erase(testRobot->collected.begin() + i);
					i--;
					currentScore += 100;
				}
			}
		}
		else if (collisions.at(i).first->getObj()->GetType() == 7 && collisions.at(i).second->getObj()->GetType() == 6)
		{
			for (int i = 0; i < testRobot->collected.size(); i++)
			{
				testRobot->collected.erase(testRobot->collected.begin() + i);
				i--;
				currentScore += 100;
			}
		}
	}
	testRobot;
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
		//	delete (*i);
			i=gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}

	renderer->DrawString("Robot Rescue! ", Vector2(380, 700));

	renderer->DrawString("Current Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives: " + std::to_string(lives), Vector2(850, 10));

	renderer->Render();
}

bool RobotRescueGame::BonusPresent()
{
	return bonusRobot;
}

Vector2 RobotRescueGame::GetBonusPos()
{
	return bonusPos;
}

void RobotRescueGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("GameSimsRobotMap.txt", gameObjects, *texManager,physics);

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	testRobot = new PlayerRobot();
	testRobot->SetCollider(new BoundingCircle(testRobot));
	AddNewObject(testRobot,7);

	for (int i = 0; i < 2; ++i) {
		float randomX = 32.0f + (rand() % 416);
		float randomY = 32.0f + (rand() % 256);
		AddEnemyRobot(Vector2(randomX, randomY));
	}

	for (int i = 0; i < 1; ++i) {
		float randomX = 32.0f + (rand() % 416);
		float randomY = 32.0f + (rand() % 256);
		AddCollectableRobot(Vector2(randomX, randomY));
	}

	gameTime		= 0;
	currentScore	= 0;
	lives			= 3;
}

void RobotRescueGame::AddNewObject(SimObject* object, int type) {
	object->SetType(type);
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider(),object->GetDynamic());
	}
}

void RobotRescueGame::AddEnemyRobot(const Vector2& position) {
	EnemyRobot* robot = new EnemyRobot();
	robot->playerRobot = testRobot;
	robot->game = this;
	robot->SetPosition(position);
	robot->SetCollider(new BoundingCircle(robot));
	AddNewObject(robot,8);
}

void RobotRescueGame::AddCollectableRobot(const Vector2& position) {
	CollectableRobot* robot = new CollectableRobot();
	robot->playerRobot = testRobot;
	robot->SetPosition(position);
	robot->SetCollider(new BoundingCircle(robot));
	AddNewObject(robot,9);
}