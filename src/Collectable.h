#pragma once

#include "AnimatedSprite.h"

class Collectable : public AnimatedSprite
{
public:
	Collectable();
	void Update(float timeStep);

private:
	float m_oscillationTime = 0.0f;
	float m_oscillationSpeed = 5.0f;
};
