#include "Entity.h"

Entity::Entity() : AnimatedSprite()
{
	m_hitBox = glm::vec4(0.0f);
}
void Entity::Update(float timeStep)
{
	AnimatedSprite::Update(timeStep);
	m_right = false;
	m_left = false;
	m_nextMoveX = 0.0f;
	m_nextMoveY = 0.0f;
}
void Entity::Land()
{
	m_jumping = false;
}
glm::vec4 Entity::GetHitBox()
{
	return glm::vec4(
		GetPosition()[0] + m_hitBox[0], 
		GetPosition()[1] + m_hitBox[1], 
		GetSize()[0] + m_hitBox[2],
		GetSize()[1] + m_hitBox[3]
		);
}
void Entity::SetHitBox(float xModify, float yModify, float widthModify, float heightModify)
{
	m_hitBox[0] += xModify;
	m_hitBox[1] += yModify;
	m_hitBox[2] += widthModify;
	m_hitBox[3] += heightModify;
}