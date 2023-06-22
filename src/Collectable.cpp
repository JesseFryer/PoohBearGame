#include "Collectable.h"
#include <math.h>

Collectable::Collectable() : AnimatedSprite()
{
}

void Collectable::Update(float timeStep)
{
	AnimatedSprite::Update(timeStep);
	AnimatedSprite::Move(0.0f, 0.2f * sin(m_oscillationSpeed * m_oscillationTime));
	m_oscillationTime += timeStep;
}
