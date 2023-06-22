#pragma once

#include "AnimatedSprite.h"

class Player : public AnimatedSprite
{
public:
	Player();
	void Update(bool inputs[5], float timeStep);

private:
	float m_speed = 100.0f;
};
