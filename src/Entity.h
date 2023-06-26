#pragma once

#include "AnimatedSprite.h"

class Entity : public AnimatedSprite
{
public:
	Entity();
	void Update(float timeStep);
	void Land();

public:
	const float m_gravity = -0.1f;
	float m_yVelocity = 0.0f;
	float m_speed = 100.0f;

	float m_nextMoveX = 0.0f;
	float m_nextMoveY = 0.0f;

	bool m_right = false;
	bool m_left = false;
	bool m_jumping = false;
	bool m_falling = false;
};
