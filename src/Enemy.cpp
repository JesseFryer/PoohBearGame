#include "Enemy.h"

Enemy::Enemy()
{
}
void Enemy::Update(glm::vec2 playerPos, float timeStep)
{
	Entity::Update(timeStep);
	if (playerPos[0] > GetPosition()[0]) m_nextMoveX = m_speed * timeStep;
	else if (playerPos[0] < GetPosition()[0]) m_nextMoveX = -m_speed * timeStep;
	if (playerPos[1] > GetPosition()[1]) m_nextMoveY = m_speed * timeStep;
	else if (playerPos[1] < GetPosition()[1]) m_nextMoveY = -m_speed * timeStep;
}
glm::vec4 Enemy::GetHitBox()
{
	return glm::vec4(GetPosition()[0] + 32.0f, GetPosition()[1] + 10, 20.0f, 27.0f);
}
