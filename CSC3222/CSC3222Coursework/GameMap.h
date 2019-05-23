#pragma once
#include <string>
#include <vector>
#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

namespace NCL {
	namespace Rendering {
		class TextureBase;
	}
	using namespace Maths;
	namespace CSC3222 {
		class GameSimsRenderer;
		class SimObject;
		class TextureManager;

		enum MapTileType {
			Flat,
			Wall,
			Rough,
			MAX_TILES
		};

		enum MapStructureType {
			RedTeleporter,
			GreenTeleporter,
			BlueTeleporter
		};

		struct StructureData {
			Vector2				startPos;
			MapStructureType	structureType;
		};

		struct MapNode {
			MapNode* bestParent;
			int posX;
			int posY;
			float g;
			float h;
			bool closed;
		};


	//	MapNode* nodeData;

		class GameMap	{
		public:
			GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager, GameSimsPhysics* physics);
			~GameMap();

			void DrawMap(GameSimsRenderer & r);

			int GetMapWidth() const {
				return mapWidth;
			}

			int GetMapHeight() const {
				return mapHeight;
			}

			float ManhattanDistance(MapNode* a, MapNode* b) const
			{
				return (abs(a->posX - b->posX) + abs(a->posY - b->posY));
			}

			Vector2 GetRandomPosition() const;
			Vector2 GetRandomPositionWithinRadius(Vector2 pos, float r) const;
			bool GeneratePath(Vector2 from, Vector2 to, std::vector<Vector2>& path) const;
			MapNode* BestNode(std::vector<MapNode*>& openlist) const;
		protected:
			int mapWidth;
			int mapHeight;
			int structureCount;

			static Vector4 tileSpecs[MAX_TILES];

			Rendering::TextureBase*	tileTexture;
			MapTileType*	mapData;
			MapNode* nodeData;
			StructureData*	structureData;		
		};
	}
}

