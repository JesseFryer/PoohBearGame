#include "renderer\Renderer.h"

#include "AnimatedSprite.h"
#include "Player.h"
#include "Collectable.h"
#include "Coin.h"
#include "Tile.h"

#include "utils\MapLoader.h"
#include "utils\SpriteSheet.h"

#include <iostream>

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
std::vector<glm::vec4> GenerateAnimation(SpriteSheet& spriteSheet, std::vector<unsigned int> spriteIndices);
void LoadMap(std::vector<Tile*>& tiles, MapLoader& loader, const std::string& mapName);

float ZOOM = 0.0f;

int main()
{
	Renderer2D renderer;
	GLFWwindow* window = renderer.GetWindow();
	glfwSetScrollCallback(window, ScrollCallback);

	// Load Textures
	float poohTexture = renderer.LoadTexture("res/sprites/PoohBear.png");
	float coinTexture = renderer.LoadTexture("res/sprites/coin.png");
	float tilesTexture = renderer.LoadTexture("res/sprites/TileSet.png");
	SpriteSheet poohSheet(256, 32, 32, 32);
	SpriteSheet coinSheet(208, 16, 16, 16);

	// Setup entity containers
	std::vector<AnimatedSprite*> sprites;
	std::vector<Tile*> tiles;

	// Init tiles.
	Tile initTile(GrassMiddle, glm::vec2(0.0f));
	initTile.Init(tilesTexture);

	// Load map.
	MapLoader mapLoader;
	mapLoader.LoadMap("Level 1", "res/maps/map1.txt");
	LoadMap(tiles, mapLoader, "Level 1");

	//for (float y = 0.0f; y < 1600.0f; y+=16.0f)
	//{
	//	for (int x = 0.0f; x < 1600.0f; x+=16.0f)
	//	{
	//		Tile* tile = new Tile(DirtBottomLeft, glm::vec2(x, y));
	//		tiles.push_back(tile);
	//	}
	//}

	Player player;
	player.SetPosition(glm::vec2(0.0f, 256.0f));
	player.SetSpriteSheet(poohTexture);
	std::vector<unsigned int> playerIdleIndices = { 0, 1, 2, 3, 4, 5, 6, 7 };
	std::vector<glm::vec4> playerIdle(GenerateAnimation(poohSheet, playerIdleIndices));
	player.AddAnimation(IDLE, 10.0f, playerIdle);
	
	Coin coin;
	Coin coin1;
	Coin coin2;
	Coin coin3;
	
	coin.SetSize(glm::vec2(16.0f));
	coin.SetPosition(glm::vec2(64.0f, 256.0f));
	coin.SetSpriteSheet(coinTexture);

	coin1.SetSize(glm::vec2(16.0f));
	coin1.SetPosition(glm::vec2(32.0f, 256.0f));
	coin1.SetSpriteSheet(coinTexture);

	coin2.SetSize(glm::vec2(16.0f));
	coin2.SetPosition(glm::vec2(16.0f, 256.0f));
	coin2.SetSpriteSheet(coinTexture);

	coin3.SetSize(glm::vec2(16.0f));
	coin3.SetPosition(glm::vec2(76.0f, 256.0f));
	coin3.SetSpriteSheet(coinTexture);

	std::vector<unsigned int> spinIndices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	std::vector<glm::vec4> spin(GenerateAnimation(coinSheet, spinIndices));
	coin.AddAnimation(IDLE, 32.0f, spin);
	coin1.AddAnimation(IDLE, 10.0f, spin);
	coin2.AddAnimation(IDLE, 45.0f, spin);
	coin3.AddAnimation(IDLE, 5.0f, spin);
	
	sprites.push_back(&player);
	sprites.push_back(&coin);
	sprites.push_back(&coin1);
	sprites.push_back(&coin2);
	sprites.push_back(&coin3);

	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		float timeStep = currentTime - lastTime;
		lastTime = currentTime;
		
		bool inputs[5] = {false, false, false, false, false};
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputs[0] = true;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputs[1] = true;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputs[2] = true;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputs[3] = true;
		renderer.ZoomCamera(ZOOM);
		ZOOM = 0.0f;

		player.Update(inputs, timeStep);
		coin.Update(timeStep);
		coin1.Update(timeStep);
		coin2.Update(timeStep);
		coin3.Update(timeStep);

		glm::vec2 camPos(player.GetPosition()[0] + (player.GetSize()[0] * 0.5), player.GetPosition()[1] + (player.GetSize()[1] * 0.5));
		renderer.SetCameraPosition(camPos);

		// ------------------------ Render here ------------------------
		glm::vec4 clearColour(0.2f, 0.6f, 0.8f, 1.0f);
		renderer.ClearScreen(clearColour);

		renderer.StartBatch();

		for (Tile* tile : tiles) tile->Draw(renderer);
		for (AnimatedSprite* sprite : sprites) sprite->Draw(renderer);

		renderer.SubmitBatch();
		// -------------------------------------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ZOOM = (float) yoffset;
}
std::vector<glm::vec4> GenerateAnimation(SpriteSheet& spriteSheet, std::vector<unsigned int> spriteIndices)
{
	std::vector<glm::vec4> animation;
	for (unsigned int spriteIndex : spriteIndices)
	{
		unsigned int col = spriteIndex % spriteSheet.GetTilesWide();
		unsigned int row = spriteIndex / spriteSheet.GetTilesHigh();
		animation.push_back(spriteSheet.GetTexCoords(row, col, 1, 1));
	}
	return animation;
}
void LoadMap(std::vector<Tile*>& tiles, MapLoader& loader, const std::string& mapName)
{
	std::unordered_map<int, TileType> tileIDToType;
	tileIDToType[1] = GrassMiddle;
	tileIDToType[2] = GrassLeft;
	tileIDToType[3] = DirtLeft;
	tileIDToType[4] = DirtRight;
	tileIDToType[5] = DirtMiddle;
	tileIDToType[6] = GrassRight;
	tileIDToType[7] = DirtBottomMiddle;
	tileIDToType[8] = DirtBottomLeft;
	tileIDToType[9] = DirtBottomRight;

	std::vector< std::vector<int> > map = loader.GetMap(mapName);
	float y = 0.0f;
	for (size_t rowIndex = map.size() - 1; rowIndex > 0; rowIndex--)
	{
		float x = 0.0f;
		for (int tileID : map[rowIndex])
		{
			if (tileID != 0)
			{
				Tile* tile = new Tile(tileIDToType[tileID], glm::vec2(x, y));
				tiles.push_back(tile);
			}
			x += 16.0f;
		}
		y += 16.0f;
	}
}