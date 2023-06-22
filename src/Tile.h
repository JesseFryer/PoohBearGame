#pragma once
#include "renderer\Renderer.h"

enum TileType
{
	GrassMiddle,
	GrassLeft,
	GrassRight,

	DirtMiddle,
	DirtLeft,
	DirtRight,
	DirtBottomLeft,
	DirtBottomMiddle,
	DirtBottomRight,

	GrassPlatform,
};

class Tile
{
public:
	Tile(TileType type, glm::vec2 postition);
	TileType GetType();

	void Draw(Renderer2D& renderer);

	// Must call Init on first Tile instance created, only call once.
	void Init(float texID);

private:
	TileType m_tileType;
	glm::vec2 m_position;
};
