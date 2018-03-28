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
	int mapWidth = -1;
	int mapHeight = -1;

	int **mapData = nullptr;
	std::vector<FlareMapEntity> entities;


	FlareMap();
	FlareMap(const std::string& filename);

	FlareMap(const FlareMap& rhs): mapWidth(rhs.mapWidth), mapHeight(rhs.mapHeight){
		mapData = new int*[mapHeight];
		for (int i = 0; i < mapHeight; i++){
			mapData[i] = new int[mapWidth];
		}

		for(int y=0; y < mapHeight; y++) {
		    for(int x=0; x < mapWidth; x++) {
				mapData[y][x] = rhs.mapData[y][x];
		    }
		}
	}

	~FlareMap();

	void Load(const std::string& fileName);

private:
	bool ReadHeader(std::ifstream &stream);
	bool ReadLayerData(std::ifstream &stream);
	bool ReadEntityData(std::ifstream &stream);

};
