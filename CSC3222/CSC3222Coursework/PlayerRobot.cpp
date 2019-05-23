#include "PlayerRobot.h"
#include "Laser.h"
#include "TextureManager.h"
#include"BoundingCircle.h"
#include "RobotRescueGame.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

PlayerRobot::PlayerRobot() : Robot()	{
	texture = texManager->GetTexture("Turret Bot.png");
	speed = 512;
	position = Vector2(280, 224);
	//position = Vector2(312, 24);
}

PlayerRobot::~PlayerRobot()	{

}

bool PlayerRobot::UpdateObject(float dt) {
	Vector2 testForce(0,0);
	float rough = 1;
	if (onRoughGround) rough = 0.3;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		testForce.x = -speed*rough;
		AddForce(testForce);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		testForce.x = speed*rough;
		AddForce(testForce);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);

		testForce.y = -speed*rough;
		AddForce(testForce);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		testForce.y = speed*rough;
		AddForce(testForce);
	}


	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_SPACE)) {
		Vector2 laserSpeed = velocity * 15.0f;
		Laser* shot = new Laser(laserSpeed);
		shot->SetPosition(position + Vector2(16,16));
		shot->SetCollider(new BoundingCircle(shot));
		game->AddNewObject(shot,10);
	}
	onRoughGround = 0;
	return true;
}