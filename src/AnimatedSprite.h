#pragma once

#include "renderer\renderer.h"
#include <unordered_map>

enum Animation
{
	IDLE,
	WALK_LEFT,
	WALK_RIGHT,
	RUN,
	JUMP
};

class AnimatedSprite
{
public:
	AnimatedSprite();

	void Draw(Renderer2D& renderer);
	void Update(float timeStep);

	glm::vec2 GetPosition();
	glm::vec2 GetSize();
	glm::vec4 GetCurrentTexture();
	float GetTexID();
	glm::vec4 GetRect();

	void Move(float dx, float dy);
	void SetPosition(glm::vec2 position);
	void SetSize(glm::vec2 size);
	void AddAnimation(Animation name, float speed, std::vector<glm::vec4> animationTexCoords);
	void SetSpriteSheet(float id);
	void SetCurrentAnimation(Animation animation);

private:
	glm::vec2 m_position;
	glm::vec2 m_size;

	unsigned int m_spriteSheetID = -1.0f;
	std::unordered_map< Animation, std::vector<glm::vec4> > m_animations;
	std::unordered_map< Animation, float > m_animationSpeeds;
	Animation m_currentAnimation = IDLE;
	unsigned int m_animationIndex = 0;
	float m_animationIncrement = 0.0f;
};
