/*
=================
cCatapult.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cCatapult.h"

void cCatapult::render()
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

void cCatapult::update(float deltaTime)
{
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		spritePos2D -= spriteTranslation;
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		spritePos2D += spriteTranslation;
	}
	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		// Add new rock sprite to the vector array
		theRocks.push_back(new cRock);
		int numRocks = theRocks.size() - 1;
		theRocks[numRocks]->setSpritePos(glm::vec2(spritePos2D.x , spritePos2D.y));
		theRocks[numRocks]->setSpriteTranslation(glm::vec2(300.0f, 0.0f));
		theRocks[numRocks]->setTexture(theGameTextures[2]->getTexture());
		theRocks[numRocks]->setTextureDimensions(theGameTextures[2]->getTWidth(), theGameTextures[2]->getTHeight());
		theRocks[numRocks]->setSpriteCentre();
		theRocks[numRocks]->setRockVelocity(glm::vec2(0.0f, 0.0f));
		theRocks[numRocks]->setSpriteRotation(getSpriteRotation());
		theRocks[numRocks]->setActive(true);
		theRocks[numRocks]->setMdlRadius();
		// play the firing sound
		m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cRock*>::iterator rockIterartor = theRocks.begin(); rockIterartor != theRocks.end(); ++rockIterartor)
	{
		(*rockIterartor)->update(deltaTime);
		for (vector<cTarget*>::iterator targetIterator = theTargets.begin(); targetIterator != theTargets.end(); ++targetIterator)
		{
			if ((*targetIterator)->collidedWith((*targetIterator)->getBoundingRect(), (*rockIterartor)->getBoundingRect()))
			{
				// if a collision set the rock and target to false
				(*targetIterator)->setActive(false);
				(*rockIterartor)->setActive(false);
			}
		}
	}

	vector<cRock*>::iterator rockIterartor = theRocks.begin();
	while (rockIterartor != theRocks.end())
	{
		if ((*rockIterartor)->isActive() == false)
		{
			rockIterartor = theRocks.erase(rockIterartor);
			// play the explosion sound.
			m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
			
		}
		else
		{
			//(*rockIterartor)->update(deltaTime);
			(*rockIterartor)->render();
			++rockIterartor;
		}
	}

	vector<cTarget*>::iterator targetIterartor = theTargets.begin();
	while (targetIterartor != theTargets.end())
	{
		// Win state
		if ((*targetIterartor)->isActive() == false)
		{
			targetIterartor = theTargets.erase(targetIterartor);
			if (theTargets.size() == 0)
			{
				bGameOver = true;
			}

		}
		else
		{
			//(*rockIterartor)->update(deltaTime);
			(*targetIterartor)->render();
			++targetIterartor;
		}
	}
}
/*
=================================================================
  Sets the velocity for the catapult
=================================================================
*/
void cCatapult::setCatapultVelocity(glm::vec2 catapultVel)
{
	catapultVelocity = catapultVel;
}
/*
=================================================================
  Gets the rocket catapult
=================================================================
*/
glm::vec2 cCatapult::getCatapultVelocity()
{
	return catapultVelocity;
}
