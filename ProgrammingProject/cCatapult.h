/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Catapult class which is a child of cSprite class
=================
*/
#ifndef _CCATAPULT_H
#define _CCATAPULT_H
#include "cSprite.h"
#include "catapultGame.h"

class cCatapult : public cSprite
{
private:
	glm::vec2 catapultVelocity = glm::vec2(0.0f, 0.0f);

public:
	void render();		// Default render function
	void update(float deltaTime);		// Catapult update method
	void setCatapultVelocity(glm::vec2 catapultVel);   // Sets the velocity for the catapult
	glm::vec2 getCatapultVelocity();				 // Gets the catapult velocity

};
#endif