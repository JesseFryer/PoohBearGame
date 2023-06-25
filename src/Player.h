#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	void Update(bool inputs[5], float timeStep);

private:
	float m_speed = 100.0f;

public:
	float m_lastMoveX = 0.0f;
	float m_lastMoveY = 0.0f;
	bool m_right = false;
	bool m_left = false;
	bool m_jumping = false;
	bool m_falling = false;
};
