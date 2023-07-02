#include "renderer\Renderer.h"

#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Tile.h"

#include "utils\MapLoader.h"
#include "utils\SpriteSheet.h"
#include "utils\UserInput.h"

#include <iostream>

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
std::vector<glm::vec4> GenerateAnimation(SpriteSheet& spriteSheet, std::vector<unsigned int> spriteIndices, bool flipped);
void LoadMap(std::vector<Tile*>& tiles, MapLoader& loader, const std::string& mapName);
bool HasCollided(glm::vec4 rect1, glm::vec4 rect2);
void Collisions(std::vector<Entity*>& entities, std::vector<Tile*>& tiles);

// Globals.
float MOUSE_SCROLL = 0.0f;
float SCROLL_SENSITIVITY = 0.1f;

int main()
{
	// Setup.
	Renderer2D renderer;
	GLFWwindow* window = renderer.GetWindow();
	UserInput userInput(window);

	// Callbacks.
	glfwSetScrollCallback(window, ScrollCallback);

	// Load Textures
	float poohTexture = renderer.LoadTexture("res/sprites/PoohBearSheet.png");
	float coinTexture = renderer.LoadTexture("res/sprites/coin.png");
	float tilesTexture = renderer.LoadTexture("res/sprites/TileSet.png");

	// Setup containers.
	std::vector<AnimatedSprite*> sprites;
	std::vector<Tile*> tiles;
	std::vector<Entity*> entities;

	Player player;
	player.SetPosition(glm::vec2(0.0f, 256.0f));
	player.SetHitBox(8.0f, 0.0f, -16.0f, -6.0f);
	player.SetSpriteSheet(poohTexture);

	Enemy enemy;
	enemy.SetPosition(glm::vec2(300.0f, 256.0f));
	enemy.SetHitBox(8.0f, 0.0f, -16.0f, -6.0f);
	enemy.SetSpriteSheet(coinTexture);

	Coin coin;
	coin.SetSize(glm::vec2(16.0f));
	coin.SetPosition(glm::vec2(242.0f, 200.0f));
	coin.SetSpriteSheet(coinTexture);

	sprites.push_back(&player);
	sprites.push_back(&coin);
	sprites.push_back(&enemy);
	entities.push_back(&player);
	entities.push_back(&enemy);

	// Generate animations.
	{
		SpriteSheet poohSheet(256, 256, 32, 32);
		SpriteSheet coinSheet(208, 16, 16, 16);

		std::vector<unsigned int> coinSpinIndices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		std::vector<unsigned int> playerWalkIndices = { 48, 49, 50, 51 };
		std::vector<unsigned int> playerIdleIndices = { 56, 57, 58, 59, 60, 61, 62, 63 };

		std::vector<glm::vec4> coinSpin(GenerateAnimation(coinSheet, coinSpinIndices, false));
		std::vector<glm::vec4> playerWalkLeft(GenerateAnimation(poohSheet, playerWalkIndices, true));
		std::vector<glm::vec4> playerWalkRight(GenerateAnimation(poohSheet, playerWalkIndices, false));
		std::vector<glm::vec4> playerIdle(GenerateAnimation(poohSheet, playerIdleIndices, false));

		player.AddAnimation(IDLE, 10.0f, playerIdle);
		player.AddAnimation(WALK_RIGHT, 8.0f, playerWalkRight);
		player.AddAnimation(WALK_LEFT, 8.0f, playerWalkLeft);
		enemy.AddAnimation(IDLE, 32.0f, coinSpin);
		coin.AddAnimation(IDLE, 16.0f, coinSpin);
	}

	// Init tiles.
	Tile initTile(GrassMiddle, glm::vec2(0.0f));
	initTile.Init(tilesTexture);

	// Load map.
	MapLoader mapLoader;
	mapLoader.LoadMap("Level 1", "res/maps/map1.txt");
	LoadMap(tiles, mapLoader, "Level 1");

	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// Delta-time stuff.
		float currentTime = glfwGetTime();
		float timeStep = currentTime - lastTime;
		lastTime = currentTime;
		std::cout << "TPF: " << timeStep * 1000 << " ms\n";
		std::cout << "FPS: " << 1 / timeStep << "\n";
		
		userInput.UpdateInputs();

		if (userInput.IsPressed(Key_ESC)) glfwSetWindowShouldClose(window, true);

		// Update.
		enemy.Update(player.GetPosition(), timeStep);
		player.Update(userInput.GetWASD(), timeStep);
		coin.Update(timeStep);
		Collisions(entities, tiles);
		
		// Camera stuff.
		glm::vec2 camPos(player.GetPosition()[0] + (player.GetSize()[0] * 0.5), player.GetPosition()[1] + (player.GetSize()[1] * 0.5));
		renderer.SetCameraPosition(camPos);
		renderer.ZoomCamera(MOUSE_SCROLL);
		MOUSE_SCROLL *= 0.9f;

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

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	MOUSE_SCROLL = ((float) yOffset) * SCROLL_SENSITIVITY;
}
std::vector<glm::vec4> GenerateAnimation(SpriteSheet& spriteSheet, std::vector<unsigned int> spriteIndices, bool flipped)
{
	std::vector<glm::vec4> animation;
	for (unsigned int spriteIndex : spriteIndices)
	{
		unsigned int col = spriteIndex % spriteSheet.GetTilesWide();
		unsigned int row = spriteIndex / spriteSheet.GetTilesHigh();
		animation.push_back(spriteSheet.GetTexCoords(col, row, 1, 1, flipped));
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
bool HasCollided(glm::vec4 rect1, glm::vec4 rect2)
{
	// vec4 -> x, y, width, height.
	if (rect1[0] < rect2[0] + rect2[2] &&
		rect1[0] + rect1[2] > rect2[0] &&
		rect1[1] < rect2[1] + rect2[3] &&
		rect1[1] + rect1[3] > rect2[1])
	{
		return true;
	}
	return false;
}
void Collisions(std::vector<Entity*>& entities, std::vector<Tile*>& tiles)
{
	// Check/Resolve collisions.
	for (Entity* entity : entities)
	{
		entity->Move(entity->m_nextMoveX, 0.0f);
		for (Tile* tile : tiles)
		{
			while (HasCollided(entity->GetHitBox(), tile->GetRect()))
			{
				entity->Move(-entity->m_nextMoveX * 0.2f, 0.0f);
			}
		}
		entity->Move(0.0f, entity->m_nextMoveY);
		for (Tile* tile : tiles)
		{
			if (HasCollided(entity->GetHitBox(), tile->GetRect()))
			{
				entity->m_yVelocity = 0.0f;
				if (entity->m_nextMoveY < 0.0f) entity->Land();
			}
			while (HasCollided(entity->GetHitBox(), tile->GetRect()))
			{
				entity->Move(0.0f, -entity->m_nextMoveY * 0.2f);
			}
		}
	}
}