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

	// copy constructor for memory allocation
	FlareMap(const FlareMap& rhs);

	~FlareMap();

	void Load(const std::string& fileName);

private:
	bool ReadHeader(std::ifstream &stream);
	bool ReadLayerData(std::ifstream &stream);
	bool ReadEntityData(std::ifstream &stream);

};
