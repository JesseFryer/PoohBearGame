#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
{
	m_position = glm::vec2(0.0f);
	m_size = glm::vec2(32.0f);
}

void AnimatedSprite::Draw(Renderer2D& renderer)
{
	renderer.AddQuad(m_position, m_size, m_animations[m_currentAnimation][m_animationIndex], m_spriteSheetID);
}
void AnimatedSprite::Update(float timeStep)
{
	m_animationIncrement += m_animationSpeeds[m_currentAnimation] * timeStep;
	if (m_animationIncrement > m_animations[m_currentAnimation].size() - 1)	m_animationIncrement = 0.0f;
	m_animationIndex = (unsigned int) m_animationIncrement;
}

glm::vec2 AnimatedSprite::GetPosition()
{
	return m_position;
}
glm::vec2 AnimatedSprite::GetSize()
{
	return m_size;
}
glm::vec4 AnimatedSprite::GetCurrentTexture()
{
	return m_animations[m_currentAnimation][0];
}
float AnimatedSprite::GetTexID()
{
	return m_spriteSheetID;
}

void AnimatedSprite::Move(float dx, float dy)
{
	m_position.x += dx;
	m_position.y += dy;
}
void AnimatedSprite::SetPosition(glm::vec2 position)
{
	m_position = position;
}
void AnimatedSprite::SetSize(glm::vec2 size)
{
	m_size = size;
}
void AnimatedSprite::AddAnimation(Animation name, float speed, std::vector<glm::vec4> animationTexCoords)
{
	m_animations[name] = animationTexCoords;
	m_animationSpeeds[name] = speed;
}
void AnimatedSprite::SetSpriteSheet(float id)
{
	m_spriteSheetID = id;
}