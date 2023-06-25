#include "Entity.h"

Entity::Entity() : AnimatedSprite()
{
}

void Entity::Update(float timeStep)
{
	AnimatedSprite::Update(timeStep);
	m_right = false;
	m_left = false;
	m_jumping = false;
	m_falling = false;
	m_nextMoveX = 0.0f;
	m_nextMoveY = 0.0f;
}