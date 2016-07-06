/*
=================
cTarget.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cTarget.h"

void cTarget::render()
{


	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cTarget::update(float deltaTime)
{

	spriteRotation += 2.0f * deltaTime;
	if (spriteRotation > 360)
	{
		spriteRotation -= 360.0f;
	}

	spritePos2D += spriteTranslation * deltaTime;

	setBoundingRect(&boundingRect);
}
gameState cTarget::update(float deltaTime, gameState aGameState)
{
	gameState tGameState = aGameState;
	spriteRotation += 2.0f * deltaTime;
	if (spriteRotation > 360)
	{
		spriteRotation -= 360.0f;
	}

	spritePos2D += spriteTranslation * deltaTime;

	setBoundingRect(&boundingRect);

	//Fail state
	if (spritePos2D.x < 200)
	{
		tGameState = ENDFAIL;
	}
	return tGameState;
}
/*
=================================================================
  Sets the velocity for the Target
=================================================================
*/
void cTarget::setTargetVelocity(glm::vec2 TargetVel)
{
	targetVelocity = TargetVel;
}
/*
=================================================================
  Gets the Target velocity
=================================================================
*/
glm::vec2 cTarget::getTargetVelocity()
{
	return targetVelocity;
}
/*
==========================================================================
Use this method to show the collision box.
==========================================================================
*/
void cTarget::renderCollisionBox()
{
	glPushMatrix();

	glTranslatef(boundingRect.left, boundingRect.top, 0.0f);
	//glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	//glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glColor3f(255.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	//glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	//glVertex2f((textureWidth / 2), -(textureHeight / 2));
	//glVertex2f((textureWidth / 2), (textureHeight / 2));
	//glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glVertex2f(-((boundingRect.right - boundingRect.left) / 2), -((boundingRect.bottom - boundingRect.top)/2));
	glVertex2f(((boundingRect.right - boundingRect.left) / 2), -((boundingRect.bottom - boundingRect.top) / 2));
	glVertex2f(((boundingRect.right - boundingRect.left) / 2), ((boundingRect.bottom - boundingRect.top) / 2));
	glVertex2f(-((boundingRect.right - boundingRect.left) / 2), ((boundingRect.bottom - boundingRect.top) / 2));
	glEnd();

	glPopMatrix();
}

