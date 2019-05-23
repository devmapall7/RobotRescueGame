#include "BoundingBox.h"

using namespace NCL;
using namespace CSC3222;

BoundingBox::BoundingBox()
{
}
BoundingBox::BoundingBox(RigidBody* a)
{
	position = a->GetPosition();
	position.x += 8;
	position.y += 8;
	height = 8;
	width = 8;
	obj = a;
}
BoundingBox::BoundingBox(RigidBody* a, int width, int height)
{
	position = a->GetPosition();
	this->width = width;
	this->height = height;
	obj = a;
}
Vector2 BoundingBox::getPosition()
{
	return position;
}
float BoundingBox::getHeight()
{
	return height;
}
float BoundingBox::getWidth()
{
	return width;
}
BoundingBox::~BoundingBox()
{
}
