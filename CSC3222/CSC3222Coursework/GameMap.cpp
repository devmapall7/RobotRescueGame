#include "GameMap.h"
#include "RobotRescueGame.h"
#include "RigidBody.h"
#include "GameSimsRenderer.h"
#include "BoundingBox.h"
#include "GameSimsPhysics.h"
#include "TextureManager.h"
#include "../../Common/Assets.h"
#include "../../Common/TextureLoader.h"
#include <fstream>
#include <iostream>

using namespace NCL;
using namespace CSC3222;
//using namespace std;
using namespace Rendering;

//These are all in format: x pos, y pos, xsize, ysize
Vector4 GameMap::tileSpecs[MAX_TILES] = {
	Vector4(101,13,16,16),	//Flat
	Vector4(96,56,16,16),	//Wall
	Vector4(144,56,16,16),	//Rough
};

//Random rough looking tiles for variety
Vector4 randomRoughTiles[4] = {
	Vector4(144,40,16,16),	//
	Vector4(144,56,16,16),	//
	Vector4(160,40,16,16),	//
	Vector4(160,56,16,16),	//
};

Vector4 buildingTypes[4] = {
	Vector4(376,56,32,48),	//Red teleporter
	Vector4(376,112,32,48), //blue teleporter
	Vector4(376,168,32,48), //green teleporter
	Vector4(144,256,64,64)  //Robot Home
};


GameMap::GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager, GameSimsPhysics* physics) {
	tileTexture = texManager.GetTexture("Colony.png");

	std::ifstream mapFile(Assets::DATADIR + filename);

	if (!mapFile) {
		std::cout << "GameMap can't be loaded in!" << std::endl;
		return;
	}

	mapFile >> mapWidth;
	mapFile >> mapHeight;

	mapData = new MapTileType[mapWidth * mapHeight];
	nodeData = new MapNode[mapWidth*mapHeight];

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			std::vector<MapNode*> openList;
			nodeData[(y * mapWidth) + x].posX = x;
			nodeData[(y * mapWidth) + x].posY = y;
			nodeData[(y * mapWidth) + x].closed = false;
			RigidBody *a;

			char type = 0;

			mapFile >> type;
			if (type - 48 == 1)
			{
					a = new RigidBody(1, Vector2(x * 16, (y - 1) * 16));
					physics->AddRigidBody(a);
					physics->AddCollider(new BoundingBox(a),0);
			}
			else if (type - 48 == 2)
			{
				a = new RigidBody(2, Vector2(x * 16, (y - 1) * 16));
				physics->AddRigidBody(a);
				physics->AddCollider(new BoundingBox(a),0);
			}
			mapData[tileIndex] = (MapTileType)(type - 48);
		}
	}
	mapFile >> structureCount;

	structureData = new StructureData[structureCount];

	for (int i = 0; i < structureCount; ++i) {
		int type = 0;
		int xTile = 0;
		int yTile = 0;

		RigidBody *a;

		mapFile >> type;
		mapFile >> xTile;
		mapFile >> yTile;

		structureData[i].structureType = (MapStructureType)type;
		structureData[i].startPos.x = xTile * 16;
		structureData[i].startPos.y = yTile * 16;
		if (type == 0 || type == 1 || type == 2)
		{
			a = new RigidBody(3+type, Vector2(xTile * 16 + 16, yTile * 16 + 8));
			physics->AddRigidBody(a);
			physics->AddCollider(new BoundingBox(a, 16, 24),0);
		}
		else if (type == 3)
		{
			a = new RigidBody(6, Vector2(xTile * 16 + 32, yTile * 16 + 16));
			physics->AddRigidBody(a);
			physics->AddCollider(new BoundingBox(a, 33, 32),0);
		}
	}
}

Vector2 GameMap::GetRandomPosition() const
{
	for (;;)
	{
		int x = rand() % mapWidth - 1;
		int y = rand() % mapHeight - 1;
		if (mapData[(y*mapWidth) + x] != Wall)
		{
			return Vector2(x * 16, y * 16);
		}
	}
}

Vector2 GameMap::GetRandomPositionWithinRadius(Vector2 pos, float r) const
{
	r /= 16;
	pos /= 16;
	for (;;)
	{
		int random = rand() % (int)r;
		int x = rand() % (int)r;
		int y = rand() % (int)r;
		if (rand() % 2)
		{
			pos.x += x;
			pos.y += y;
		}
		else
		{
			pos.x -= x;
			pos.y -= y;
		}
		if (pos.x >= mapWidth) pos.x = mapWidth - 1;
		else if (pos.x <= 0) pos.x = 1;
		if (pos.y >= mapHeight) pos.y = mapHeight - 1;
		else if (pos.y <= 0) pos.y = 1;
		if (mapData[(y*mapWidth) + x] != Wall)
		{
			return pos * 16;
		}
	}
}


bool GameMap::GeneratePath(Vector2 from, Vector2 to, std::vector<Vector2>& path) const
{
	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			nodeData[(y * mapWidth) + x].closed = false;
		}
	}
	int startX = from.x;
	int startY = from.y;
	int finishX = to.x;
	int finishY = to.y;
	std::vector<MapNode*> openList;
	MapNode* a = &nodeData[((startY / 16) * mapWidth) + (startX / 16)];
	MapNode* b = &nodeData[((finishY / 16) * mapWidth) + (finishX / 16)];
	a->g = 0;
	a->h = ManhattanDistance(a, b);
	openList.emplace_back(a);
//	cout << b->posX << ' ' << b->posY << endl;
	while (!openList.empty())
	{
		MapNode* p = BestNode(openList);
		//cout << p->posX << ' ' << p->posY << endl;
		//system("PAUSE");
		if (p == b)
		{
			MapNode* r = b;
			while (r != a)
			{
				path.emplace_back(Vector2(r->posX * 16 + 8, r->posY * 16 + 8));
				r = r->bestParent;
			}
			path.emplace_back(Vector2(a->posX * 16 + 8, a->posY * 16 + 8));
			return true;
		}
		else
		{
		if (p->posX > 0)
		{
			int tileType = mapData[((p->posY)*mapWidth) + (p->posX - 1)];
			if (tileType != Wall)
			{
				MapNode* neighbour = &nodeData[((p->posY)*mapWidth) + (p->posX - 1)];
				neighbour->g = p->g + 1;
				neighbour->h = ManhattanDistance(neighbour, b);
				//	cout << neighbour->posX << ' ' << neighbour->posY << endl << openList.at(i)->posX << ' ' << openList.at(i)->posY << endl;
				//	cout << openList.at(i)->posX << ' ' << neighbour->posX << ' ' << endl << openList.at(i)->posY << ' ' << neighbour->posY << endl;
				//	system("PAUSE");
				if (neighbour->closed);
				else if (nodeData[((p->posY)*mapWidth) + (p->posX - 1)].g + nodeData[((p->posY)*mapWidth) + (p->posX - 1)].h < neighbour->g + neighbour->h);
					else if (nodeData[((p->posY)*mapWidth) + (p->posX - 1)].g + nodeData[((p->posY)*mapWidth) + (p->posX - 1)].h > neighbour->g + neighbour->h) nodeData[((p->posY)*mapWidth) + (p->posX - 1)].bestParent=p;
					else 
					{
						neighbour->bestParent = p;
						openList.emplace_back(neighbour);
						
					}
				}
			}
		}
		if (p->posX < mapWidth-2)
		{
			int tileType = mapData[((p->posY)*mapWidth) + (p->posX + 1)];
			if (tileType != Wall)
			{
				MapNode* neighbour = &nodeData[((p->posY)*mapWidth) + (p->posX + 1)];
				neighbour->g = p->g + 1;
				neighbour->h = ManhattanDistance(neighbour, b);
				if (neighbour->closed);
				else if (nodeData[((p->posY)*mapWidth) + (p->posX + 1)].g + nodeData[((p->posY)*mapWidth) + (p->posX + 1)].h < neighbour->g + neighbour->h);
				else if (nodeData[((p->posY)*mapWidth) + (p->posX + 1)].g + nodeData[((p->posY)*mapWidth) + (p->posX + 1)].h > neighbour->g + neighbour->h) nodeData[((p->posY)*mapWidth) + (p->posX = 1)].bestParent = p;
				else
				{
					neighbour->bestParent = p;
					openList.emplace_back(neighbour);

				}
			}
		}
		if (p->posY > 0)
		{
			int tileType = mapData[((p->posY-1)*mapWidth) + (p->posX)];
			if (tileType != Wall)
			{
				MapNode* neighbour = &nodeData[((p->posY-1)*mapWidth) + (p->posX)];
				neighbour->g = p->g + 1;
				neighbour->h = ManhattanDistance(neighbour, b);
				if (neighbour->closed);
				else if (nodeData[((p->posY-1)*mapWidth) + (p->posX)].g + nodeData[((p->posY-1)*mapWidth) + (p->posX)].h < neighbour->g + neighbour->h);
				else if (nodeData[((p->posY-1)*mapWidth) + (p->posX)].g + nodeData[((p->posY-1)*mapWidth) + (p->posX)].h > neighbour->g + neighbour->h) nodeData[((p->posY-1)*mapWidth) + (p->posX)].bestParent = p;
				else
				{
					neighbour->bestParent = p;
					openList.emplace_back(neighbour);

				}
			}
		}
		if (p->posY < mapHeight-2)
		{
			int tileType = mapData[((p->posY+1)*mapWidth) + (p->posX)];
			if (tileType != Wall)
			{
				MapNode* neighbour = &nodeData[((p->posY+1)*mapWidth) + (p->posX)];
				neighbour->g = p->g + 1;
				neighbour->h = ManhattanDistance(neighbour, b);
				if (neighbour->closed);
				else if (nodeData[((p->posY + 1)*mapWidth) + (p->posX)].g + nodeData[((p->posY + 1)*mapWidth) + (p->posX)].h < neighbour->g + neighbour->h);
				else if (nodeData[((p->posY + 1)*mapWidth) + (p->posX)].g + nodeData[((p->posY + 1)*mapWidth) + (p->posX)].h > neighbour->g + neighbour->h) nodeData[((p->posY + 1)*mapWidth) + (p->posX)].bestParent = p;
				else
				{
					neighbour->bestParent = p;
					openList.emplace_back(neighbour);

				}
			}
		}
		//system("PAUSE");
		for (int i = 0; i < openList.size(); i++)
		{
			if (openList.at(i) == p) { openList.erase(openList.begin() + i); p->closed = true; 
		//	system("PAUSE");
			}
		}
		//cout << p->posX << ' ' << p->posY << endl;
	//	cout << openList.size() << endl;
		if (openList.size() == 0) return false;
		}
	}

MapNode* GameMap::BestNode(std::vector<MapNode*>& openList) const
{
	float bestF = openList.at(0)->g + openList.at(0)->h;
	MapNode* bestNode = openList.at(0);
	for (auto&i : openList)
	{
		if (i->closed);
		else
		{
			float iF = i->g + i->h;
			if (iF < bestF)
			{
				bestF = iF;
				bestNode = i;
			}
		}
	}
	return bestNode;
}

GameMap::~GameMap()	{
	delete[] mapData;
	delete[] structureData;
}

void GameMap::DrawMap(GameSimsRenderer & r) {
	int tileSize = 16;

	srand(0); //so we get consistent floor tiles

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileType = mapData[(y * mapWidth) + x];

			Vector4 tileData;

			if (tileType == MapTileType::Rough) {
				int randomFlatTile = rand() % 4;
				tileData = randomRoughTiles[randomFlatTile];
			}
			else {
				tileData = tileSpecs[tileType];
			}

			Vector2 texPos  = Vector2(tileData.x, tileData.y);
			Vector2 texSize = Vector2(tileData.z, tileData.w);

			Vector2 screenPos = Vector2(x * 16, y * 16);

			r.DrawTextureArea((OGLTexture*)tileTexture, texPos, texSize, screenPos, false);
		}
	}

	for (int i = 0; i < structureCount; ++i) {
		Vector4 buildingData = buildingTypes[structureData[i].structureType];

		Vector2 texPos = Vector2(buildingData.x, buildingData.y);
		Vector2 texSize = Vector2(buildingData.z, buildingData.w);

		Vector2 screenPos = structureData[i].startPos;

		r.DrawTextureArea((OGLTexture*)tileTexture, texPos, texSize, screenPos, false);
	}
}