/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Rock class which is a child of cSprite class
=================
*/
#ifndef _CROCK_H
#define _CROCK_H
#include "cSprite.h"


class cRock : public cSprite
{
private:
	glm::vec2 rockVelocity = glm::vec2(0.0f, 0.0f);

public:
	//cRock(vector<cTexture*> tileTextList)

	void render();		// Default render function
	void update(float deltaTime);		// Rock update method
	void setRockVelocity(glm::vec2 bulletVel);   // Sets the velocity for the Rock
	glm::vec2 getRockVelocity();				 // Gets the Rock velocity
	void renderCollisionBox();				// Use this function to show the collision box
};
#endif