#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	void Update(glm::vec2 playerPos, float timeStep);
	glm::vec4 GetHitBox();
};
