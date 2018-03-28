#pragma once

#include <string>
#include <vector>

struct FlareMapEntity {
	std::string type;
	float x;
	float y;
};

class FlareMap {
	public:
		FlareMap();
		FlareMap(const std::string& filename);

		// FlareMap(const FlareMap& rhs): mapWidth(rhs.mapWidth), mapHeight(rhs.mapHeight){
		// 	mapData = new int*[mapHeight];
		// 	for (int i = 0; i < mapHeight; i++){
		// 		mapdata[i] = new int[mapWidth];
		// 	}
		// }

		~FlareMap();

		void Load(const std::string& fileName);

		int mapWidth = -1;
		int mapHeight = -1;

		int **mapData = nullptr;
		std::vector<FlareMapEntity> entities;

	private:

		bool ReadHeader(std::ifstream &stream);
		bool ReadLayerData(std::ifstream &stream);
		bool ReadEntityData(std::ifstream &stream);

};
