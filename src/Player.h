#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	void Update(bool inputs[5], float timeStep);
};
