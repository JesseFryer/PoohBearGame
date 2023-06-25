#include "Player.h"

Player::Player() : Entity()
{
}
void Player::Update(bool inputs[5], float timeStep)
{
	Entity::Update(timeStep);
	float distance = m_speed * timeStep;
	if (inputs[0])
	{
		m_jumping = true;
		m_nextMoveY += 10 * distance;
	}
	if (inputs[1]) {
		m_left = true;
		m_nextMoveX = -distance;
	}
	if (inputs[3]) {
		m_right = true;
		m_nextMoveX = distance;
	}
	m_nextMoveY -= 1.5f;
}
