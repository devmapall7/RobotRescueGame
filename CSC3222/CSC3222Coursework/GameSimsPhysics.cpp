#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include"BoundingBox.h"
#include"BoundingCircle.h"

using namespace NCL;
using namespace CSC3222;
using namespace std;

float timeRemaining=0;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt, vector<CollisionPair> & collisions) {
	IntegrateAcceleration(dt);
	CollisionDetection(dt, collisions);
	IntegrateVelocity(dt);
	ResetForce();
}
void GameSimsPhysics::FixedPhysics(float realDT, vector<CollisionPair> & collisions)
{
	timeRemaining += realDT;
	const float subTimeDelta = 1.0f / 120;

	while (timeRemaining > subTimeDelta)
	{
		Update(subTimeDelta, collisions);
		timeRemaining -= subTimeDelta;
	}
}
void GameSimsPhysics::ResetForce()
{
	for (auto const& value : allBodies) value->force = Vector2(0, 0);
}
void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c, const bool& dynamic) {
	if (dynamic) dynamicColliders.emplace_back(c);
	else staticColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c, const bool& dynamic) {
	if (dynamic)
	{
		auto at = std::find(dynamicColliders.begin(), dynamicColliders.end(), c);
		if (at != dynamicColliders.end())
		{
			//maybe delete too?
			dynamicColliders.erase(at);
		}
	}
	else
	{
		auto at = std::find(staticColliders.begin(), staticColliders.end(), c);
		if (at != staticColliders.end())
		{
			//maybe delete too?
			staticColliders.erase(at);
		}
	}
}

void GameSimsPhysics::IntegrateAcceleration(float dt) 
{
	for (auto const& value : allBodies)
	{
		Vector2 acceleration = value->force*value->inverseMass;
		value->velocity += acceleration * dt;
		value->velocity *= 0.959f;
	}

}

void GameSimsPhysics::CollisionDetection(float dt, vector<CollisionPair> & collisions)
{
	float penDistance;
	Vector2 colNormal;
	BroadPhase(collisions);
	for (int i = 0; i < collisions.size(); i++)
	{
		if (!CheckCollision(collisions.at(i), penDistance, colNormal))
		{
			collisions.erase(collisions.begin() + i);
			i--;
		}
		else if (collisions.at(i).first->getObj()->GetType()==1 || collisions.at(i).second->getObj()->GetType() == 1 || collisions.at(i).first->getObj()->GetType() == 6 || collisions.at(i).second->getObj()->GetType() == 6) CollisionResolution(collisions.at(i), penDistance, colNormal);
	}
}

void GameSimsPhysics::CollisionResolution(CollisionPair & a, float& penDistance, Vector2& colNormal)
{
	Projection(a, penDistance, colNormal);
	Impulse(a, colNormal);
}

void GameSimsPhysics::Projection(CollisionPair & a, float& penDistance, Vector2& colNormal)
{
	float totalMass = a.first->getObj()->inverseMass + a.second->getObj()->inverseMass;
	if (strcmp(typeid(*a.first).name(), "class NCL::CSC3222::BoundingCircle") == 0 && strcmp(typeid(*a.second).name(), "class NCL::CSC3222::BoundingCircle") == 0)
	{
		a.first->getObj()->position += colNormal * penDistance*(a.first->getObj()->inverseMass / totalMass);
		a.second->getObj()->position -= colNormal * penDistance*(a.second->getObj()->inverseMass / totalMass);
	}
	else
	{
		a.first->getObj()->position -= colNormal * penDistance*(a.first->getObj()->inverseMass / totalMass);
		a.second->getObj()->position += colNormal * penDistance*(a.second->getObj()->inverseMass / totalMass);
	}
	

}

void GameSimsPhysics::Impulse(CollisionPair & a, Vector2& colNormal)
{
	Vector2 relVel = a.second->getObj()->velocity - a.first->getObj()->velocity;
	float imp = (relVel*colNormal)*-2 / (a.second->getObj()->inverseMass + a.first->getObj()->inverseMass);
	a.first->getObj()->AddImpulse(-imp, colNormal);
	a.second->getObj()->AddImpulse(imp, colNormal);
	
}

bool GameSimsPhysics::CheckCollision(CollisionPair & a, float& penDistance, Vector2& colNormal)
{
	if (strcmp(typeid(*a.first).name(), "class NCL::CSC3222::BoundingCircle") == 0)
	{
		if (strcmp(typeid(*a.second).name(), "class NCL::CSC3222::BoundingCircle") == 0) return CircleCircleCollision(a.first, a.second, penDistance, colNormal);
		else return CircleBoxCollision(a.first, a.second, penDistance, colNormal);
	}
	else if (strcmp(typeid(*a.first).name(), "class NCL::CSC3222::BoundingBox") == 0)
	{
		if (strcmp(typeid(*a.second).name(), "class NCL::CSC3222::BoundingBox") == 0) return BoxBoxCollision(a.first, a.second, penDistance, colNormal);
		else return CircleBoxCollision(a.second, a.first, penDistance, colNormal);
	}
	return false;
}

void GameSimsPhysics::BroadPhase(std::vector<CollisionPair>& collisions)
{
	InsertionSort();
	DynamicStaticCollision(collisions);
	DynamicDynamicCollision(collisions);

}


void GameSimsPhysics::DynamicStaticCollision(std::vector<CollisionPair>& collisions)
{
	int startj = 0;
	for (int i = 0; i < dynamicColliders.size(); i++)
	{
		for (int j = startj; j < staticColliders.size(); j++)
		{
			if (staticColliders.at(j)->getPosition().x + staticColliders.at(j)->getWidth() < dynamicColliders.at(i)->getPosition().x - dynamicColliders.at(i)->getWidth())
			{
				//startj++;
				//j++;
			}
			else if (staticColliders.at(j)->getPosition().x - staticColliders.at(j)->getWidth() > dynamicColliders.at(i)->getPosition().x + dynamicColliders.at(i)->getWidth())
			{
				break;
			}
			else
			{
				CollisionPair* test = new CollisionPair(dynamicColliders.at(i), staticColliders.at(j));
				collisions.emplace_back(*test);
			}
		}
	}

}


void GameSimsPhysics::DynamicDynamicCollision(std::vector<CollisionPair>& collisions)
{
	std::vector<CollisionVolume*> a;
	a.emplace_back(dynamicColliders.at(0));
	int i;
	int j;
	for (i = 1; i < dynamicColliders.size(); ++i)
	{
		for (j = 0; j < a.size(); ++j)
		{
		/*	cout << i << ' ' << j << endl;
			cout << dynamicColliders.at(i)->getPosition().x  << ' ' << a.at(j)->getPosition().x  << endl;
			system("PAUSE");*/
			if (dynamicColliders.at(i)->getPosition().x - dynamicColliders.at(i)->getWidth() < a.at(j)->getPosition().x + a.at(j)->getWidth())
			{
				CollisionPair* test = new CollisionPair(dynamicColliders.at(i), a.at(j));
				collisions.emplace_back(*test);
			}
			else
			{
				a.erase(a.begin() + j);
				j--;
			}
		}
		a.emplace_back(dynamicColliders.at(i));
	}
 //cout << collisions.size() << endl;
//	cout << collisions.at(0).first->getPosition() << ' '<< collisions.at(0).second->getPosition() << endl;
//	system("PAUSE");
}

void GameSimsPhysics::InsertionSort()
{
	if (!staticSorted)
	{
		int i = 1;
		while (i < staticColliders.size())
		{
			int j = i;
			CollisionVolume* a;
			while ((j > 0) && (staticColliders.at(j - 1)->getPosition().x - staticColliders.at(j-1)->getWidth() > staticColliders.at(j)->getPosition().x - staticColliders.at(j)->getWidth()))
			{
				a = staticColliders.at(j);
				staticColliders.at(j) = staticColliders.at(j - 1);
				staticColliders.at(j - 1) = a;
				j--;
			}
			i++;
		}
		staticSorted = true;
	}
	int i = 1;
	while (i < dynamicColliders.size())
	{
		int j = i;
		CollisionVolume* a;
		while (j > 0 && dynamicColliders.at(j - 1)->getPosition().x - dynamicColliders.at(j-1)->getWidth() > dynamicColliders.at(j)->getPosition().x - dynamicColliders.at(j)->getWidth())
		{

			a = dynamicColliders.at(j);
			dynamicColliders.at(j) = dynamicColliders.at(j - 1);
			dynamicColliders.at(j - 1) = a;
			j--;
		}
		i++;
	}
}

bool GameSimsPhysics::CircleCircleCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal)
{
	Vector2 first, second;
	first = a->getPosition();
	second = b->getPosition();
	float x = first.x - second.x;
	float y = first.y - second.y;
	float dist = x * x + y * y;
	if (dist < 16 * 16)
	{
		penDistance = 16 - sqrt(dist);
		colNormal = first - second;
		colNormal.Normalise();
		return true;
	}
	return false;
}
bool GameSimsPhysics::BoxBoxCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal)
{
	Vector2 first, second;
	first = a->getPosition();
	second = b->getPosition();
	float fWidth, sWidth, fHeight, sHeight;
	fWidth = a->getWidth();
	sWidth = b->getWidth();
	fHeight = a->getHeight();
	sHeight = b->getHeight();
	if ((abs(first.x - second.x) < (fWidth + sWidth)
		&& abs(first.y - second.y) < (fHeight + sHeight)))
	{
		if ((fWidth + sWidth) - (first.x - second.x) >= (fHeight + sHeight) - (first.y - second.y))
		{
			penDistance = (fHeight + sHeight) - (first.y - second.y);
			colNormal = Vector2(0, 1);
		}
		else
		{
			penDistance = (fWidth + sWidth) - (first.x - second.x);
			colNormal = Vector2(1, 0);
		}
		return true;
	}
	return false;
}
bool GameSimsPhysics::CircleBoxCollision(CollisionVolume* a, CollisionVolume* b, float& penDistance, Vector2& colNormal)
{
	Vector2 first, second;
	first = a->getPosition();
	second = b->getPosition();
	float testX = first.x;
	float testY = first.y;
	if (first.x < second.x - b->getWidth()) testX = second.x - b->getWidth();
	else if (first.x > second.x + b->getWidth()) testX = second.x + b->getWidth();

	if (first.y < second.y - b->getHeight()) testY = second.y - b->getHeight();
	else if (first.y > second.y + b->getHeight()) testY = second.y + b->getHeight();
	float x = first.x - testX;
	float y = first.y - testY;
	if ((x*x + y * y) < 8 * 8)
	{
		Vector2 testVec(testX, testY);
		colNormal = testVec - first;
		colNormal.Normalise(); 
		penDistance = 8 - sqrt((x*x + y * y));
		return true;
	}
	return false;
}
void GameSimsPhysics::IntegrateVelocity(float dt)
{
	for (auto const& value : allBodies)
	{
		value->position += value->velocity * dt;
	}
}