#pragma once

#include <vector>
#include <string>

class MapLoader
{

public:
	void LoadMap(const std::string& mapName, const std::string& filepath);
	std::vector < std::vector<int> > GetMap(const std::string& mapName);
};
