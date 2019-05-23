#include "CollectableRobot.h"
#include "GameSimsRenderer.h"
#include "TextureManager.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Vector4.h"

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;
//using namespace std;

Vector4 spriteDetails[5] = {
	Vector4(40 ,174,16, 18),
	Vector4(72 ,174,16, 18),
	Vector4(104,174,16, 18),
	Vector4(136,174,16, 18),
	Vector4(17 ,51 ,14, 13)
};

CollectableRobot::CollectableRobot() : SimObject()	{
	collected = false;

	int r = rand() % 5;
	type = (RobotType)r;

	if (type == Special) {
		texture = texManager->GetTexture("Green Bot Sprites.png");
	}
	else {
		texture = texManager->GetTexture("Gum Bot Sprites.png");
	}
}

CollectableRobot::CollectableRobot(RobotType type) : CollectableRobot() {
	this->type = type;
}

CollectableRobot::~CollectableRobot()	{

}

void CollectableRobot::DrawObject(GameSimsRenderer &r) {
	Vector2 texPos(spriteDetails[type].x, spriteDetails[type].y);
	Vector2 texSize(spriteDetails[type].z, spriteDetails[type].w);

	Vector2 screenPos = Vector2(position.x,position.y+16);

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, false);
}

void CollectableRobot::SetCollected(const bool & col)
{
	collected = col;
}

bool CollectableRobot::GetCollected()
{
	return collected;
}

bool CollectableRobot::UpdateObject(float dt) {
	if (collected) {
		float testX = this->GetPosition().x - playerRobot->GetPosition().x;
		float testY = this->GetPosition().y - playerRobot->GetPosition().y;
		float distance = sqrt(testX * testX + testY * testY);
		if (distance>128) this->SetPosition(playerRobot->GetPosition());
		AddForce((this->GetPosition()-playerRobot->GetPosition()) * (-20) - this->velocity * 0.999f);
		if (playerRobot->collected.size() == 0) return false;
	}
	onRoughGround = 0;
	return true;
}