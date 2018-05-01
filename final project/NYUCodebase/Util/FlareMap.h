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
	int mapWidth;
	int mapHeight;
	int **mapData;

	std::vector<FlareMapEntity> entities;

	FlareMap();
	FlareMap(const std::string& filename);

	// copy constructor for memory allocation
	FlareMap(const FlareMap& rhs);

	~FlareMap();

	void Load(const std::string& fileName);
	// bool compare(const std::string& orig, const std::string& comp);

private:
	bool ReadHeader(std::ifstream &stream);
	bool ReadLayerData(std::ifstream &stream);
	bool ReadEntityData(std::ifstream &stream);

};
