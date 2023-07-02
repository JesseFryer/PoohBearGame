#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(unsigned int sheetWidth, unsigned int sheetHeight, unsigned int tileWidth, unsigned int tileHeight)
{
	texXIncrement = static_cast<float>(tileWidth) / sheetWidth;
	texYIncrement = static_cast<float>(tileHeight) / sheetHeight;
	tilesWide = sheetWidth / tileWidth;
	tilesHigh = sheetHeight / tileHeight;
}
glm::vec4 SpriteSheet::GetTexCoords(
	unsigned int spriteIndexX, 
	unsigned int spriteIndexY, 
	unsigned int tilesWide, 
	unsigned int tilesHigh,
	bool flipped)
{
	float x1 = spriteIndexX * texXIncrement;
	float x2 = x1 + (tilesWide * texXIncrement);
	float y1 = spriteIndexY * texYIncrement;
	float y2 = y1 + (tilesHigh * texYIncrement);
	if (flipped) return glm::vec4(x2, y1, x1, y2);
	return glm::vec4(x1, y1, x2, y2);
}

unsigned int SpriteSheet::GetTilesHigh()
{
	return tilesHigh;
}

unsigned int SpriteSheet::GetTilesWide()
{
	return tilesWide;
}
