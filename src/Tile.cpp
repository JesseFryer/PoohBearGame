#include "Tile.h"
#include "utils\SpriteSheet.h"
#include <unordered_map>

const glm::vec2 SIZE(16.0f);

static std::unordered_map<TileType, glm::vec4> s_texCoords;
static float s_texID = 0.0f;

Tile::Tile(TileType type, glm::vec2 position)
{
	m_tileType = type;
	m_position = position;
}
TileType Tile::GetType()
{
	return m_tileType;
}

void Tile::Draw(Renderer2D& renderer)
{
	renderer.AddQuad(m_position, SIZE, s_texCoords[m_tileType], s_texID);
}

void Tile::Init(float texID)
{
	s_texID = texID;
	SpriteSheet tileSheet(256, 256, 32, 32);
	s_texCoords[GrassLeft]		   = tileSheet.GetTexCoords(0, 7, 1, 1);
	s_texCoords[GrassMiddle]	   = tileSheet.GetTexCoords(1, 7, 1, 1);
	s_texCoords[GrassRight]        = tileSheet.GetTexCoords(2, 7, 1, 1);
	s_texCoords[DirtLeft]          = tileSheet.GetTexCoords(0, 6, 1, 1);
	s_texCoords[DirtMiddle]        = tileSheet.GetTexCoords(1, 6, 1, 1);
	s_texCoords[DirtRight]		   = tileSheet.GetTexCoords(2, 6, 1, 1);
	s_texCoords[DirtBottomLeft]	   = tileSheet.GetTexCoords(0, 5, 1, 1);
	s_texCoords[DirtBottomMiddle]  = tileSheet.GetTexCoords(1, 5, 1, 1);
	s_texCoords[DirtBottomRight]   = tileSheet.GetTexCoords(2, 5, 1, 1);
	s_texCoords[GrassPlatform]   = tileSheet.GetTexCoords(3, 4, 1, 1);
}
