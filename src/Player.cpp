#include "Player.h"

Player::Player() : AnimatedSprite()
{
}
void Player::Update(bool inputs[5], float timeStep)
{
	AnimatedSprite::Update(timeStep);
	float distance = m_speed * timeStep;
	if (inputs[0]) Move(0.0f, distance);
	if (inputs[1]) Move(-distance, 0.0f);
	if (inputs[2]) Move(0.0f, -distance);
	if (inputs[3]) Move(distance, 0.0f);
}
