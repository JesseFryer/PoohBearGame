#include "Player.h"

Player::Player() : Entity()
{
	m_speed = 150.0f;
}
void Player::Update(std::array<bool, 5> inputs, float timeStep)
{
	Entity::Update(timeStep);

	float distance = m_speed * timeStep;

	if (inputs[0] && !inputs[2])
	{
		if (!m_jumping && m_yVelocity == 0)
		{
			m_jumping = true;
			m_yVelocity = 3.0f;
		}
		else m_variableJump = 2.0f;
	}

	if (inputs[1]) 
	{
		SetCurrentAnimation(WALK_LEFT);
		m_left = true;
		m_nextMoveX = -distance;
	}

	if (inputs[2])
	{
		m_yVelocity -= 0.5f;
	}

	if (inputs[3]) 
	{
		SetCurrentAnimation(WALK_RIGHT);
		m_right = true;
		m_nextMoveX = distance;
	}

	m_yVelocity += m_gravity / m_variableJump;
	m_variableJump = 1.0f;
	if (m_yVelocity < -4.5f) m_yVelocity = -4.5f;
	m_nextMoveY = m_yVelocity;
	if (!m_nextMoveX) SetCurrentAnimation(IDLE);
}
glm::vec4 Player::GetHitBox()
{
	return glm::vec4(GetPosition()[0] + 32.0f, GetPosition()[1] + 10, 20.0f, 27.0f);
}