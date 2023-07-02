#pragma once

#include "Entity.h"
#include <array>

class Player : public Entity
{
public:
	Player();
	void Update(std::array<bool, 5> inputs, float timeStep);
	glm::vec4 GetHitBox();

private:
	float m_variableJump = 1.0f;
};
