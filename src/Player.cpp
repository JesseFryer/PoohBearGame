#include "Player.h"

Player::Player() : Entity()
{
}
void Player::Update(bool inputs[5], float timeStep)
{
	Entity::Update(timeStep);

	float distance = m_speed * timeStep;
	if (inputs[0] && !m_jumping)
	{
		m_jumping = true;
		m_yVelocity = 4.0f;
	}
	if (inputs[1]) {
		m_left = true;
		m_nextMoveX = -distance;
	}
	if (inputs[3]) {
		m_right = true;
		m_nextMoveX = distance;
	}
	m_yVelocity += m_gravity;
	if (m_yVelocity < -3.5f) m_yVelocity = -3.5f;
	m_nextMoveY = m_yVelocity;
}
