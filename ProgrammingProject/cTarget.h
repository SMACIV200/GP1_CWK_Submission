/*
=================
cAsteroid.h
- Header file for class definition - SPECIFICATION
- Header file for the Catapult class which is a child of cSprite class
=================
*/
#ifndef _CTARGET_H
#define _CTARGET_H
#include "cSprite.h"


class cTarget : public cSprite
{
private:
	glm::vec2 targetVelocity = glm::vec2(0.0f, 0.0f);

public:
	void render();		// Default render function
	void update(float deltaTime);		// Target update method
	void setTargetVelocity(glm::vec2 targetVel);   // Sets the velocity for the target
	glm::vec2 getTargetVelocity();				 // Gets the target velocity
	void renderCollisionBox();				// Use this function to show the collision box
};
#endif