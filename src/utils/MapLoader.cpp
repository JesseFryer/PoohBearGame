#include "MapLoader.h"

#include <iostream>
#include <unordered_map>
#include <fstream>

static std::unordered_map<std::string, std::vector<std::vector<int>> > s_maps;

void MapLoader::LoadMap(const std::string& mapName, const std::string& filepath)
{
	std::ifstream mapFile(filepath, std::ios::in, std::ios::binary);
	if (mapFile) 
	{
		std::vector< std::vector<int> > map;
		std::vector<int> row;
		std::string line;
		while (std::getline(mapFile, line))
		{
			for (size_t i = 0; i < line.size(); i++)
			{
				row.push_back(line[i] - '0'); // convert from char to integer.
			}
			map.push_back(row);
			row.clear();
		}
		s_maps[mapName] = map;
	}
	else std::cout << "could not open map file" << std::endl;	
}
std::vector<std::vector<int>> MapLoader::GetMap(const std::string& mapName)
{
    return s_maps[mapName];
}
