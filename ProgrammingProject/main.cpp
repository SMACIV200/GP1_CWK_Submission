#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "catapultGame.h"
#include "cButton.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1280;
    const int windowHeight = 768;
    const int windowBPP = 16;

	void spawnTargets(int numberToSpawn, int wndWidth);

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\Target80x80.png", "Images\\Rock1.png", "Images\\Rock264x47.png", "Images\\Catapult.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/CatapultLaunch.wav", "Audio/explosion2.wav" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/Grass1280x768.png", "Images/RacingStartBkg.png", "Images/RacingEndBkg.png" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitButton.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playButton.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton saveButton;
	saveButton.attachInputMgr(theInputMgr);
	saveButton.setTexture(btnTextureList[4]->getTexture());
	saveButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	string outputMsg;
	string strMsg[] = { "Break The Targets!","Don't Let Any Through!", "Catapult", "Thanks for playing!","See you again soon!" };

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	int spawnNumber = 4;
	spawnTargets(spawnNumber, windowWidth);

	// Create vector array of textures		

	cTexture catapultTxt;
	catapultTxt.createTexture("Images\\Catapult256x140.png");

	cCatapult catapultSprite;
	catapultSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	catapultSprite.setSpritePos(glm::vec2(130.0f, 380.0f));
	catapultSprite.setTexture(catapultTxt.getTexture());
	catapultSprite.setTextureDimensions(catapultTxt.getTWidth(), catapultTxt.getTHeight());
	catapultSprite.setSpriteCentre();
	catapultSprite.setCatapultVelocity(glm::vec2(0.0f, 10.0f));
	catapultSprite.setSpriteTranslation(glm::vec2(0.0f, 10.0f));
	catapultSprite.setSpriteRotation(0.0f);

	// Attach sound manager to catapult sprite
	catapultSprite.attachSoundMgr(theSoundMgr);

	float spawnTimer = 0;

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState)
		{
		case MENU:
			spriteStartBkgd.render();

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(elapsedTime);


			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[1];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;
		case PLAYING:
			spriteBkgd.render();

			catapultSprite.update(elapsedTime);
			{
				vector<cTarget*>::iterator targetIterator = theTargets.begin();
				while (targetIterator != theTargets.end())
				{
					if ((*targetIterator)->isActive() == false)
					{
						targetIterator = theTargets.erase(targetIterator);
					}
					else
					{
						(*targetIterator)->update(elapsedTime);
						(*targetIterator)->render();
						++targetIterator;
					}
				}
			}
			catapultSprite.render();
		

			// render button and reset clicked to false

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

			if (bGameOver == true)
			{
				theGameState = END;
			}
			
			spawnTimer += elapsedTime;
			if (spawnTimer > 2)
			{
				spawnTargets(spawnNumber, windowWidth);
				spawnTimer = 0;
			}

			outputMsg = to_string(spawnTimer);
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(900, 15, 0.0f));

			break;
		case END:
			spriteEndBkgd.render();

			playButton.setClicked(false);
			exitButton.setClicked(false);

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(elapsedTime);

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[4];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			if (theGameState == PLAYING)
			{
				bGameOver = false;
				spawnTargets(spawnNumber, windowWidth);
				theRocks.clear();
			}
			break;
		}


		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}

/*
==========================================================================
Spawns required number of targets.
==========================================================================
*/
void spawnTargets(int numberToSpawn, int wndWidth)
{
	int startPos = theTargets.size();
	int endPos = startPos + numberToSpawn;
	int targNum = 0;

	for (int targ = startPos; targ < endPos; targ++)
	{
		theTargets.push_back(new cTarget);
		theTargets[targ]->setSpritePos(glm::vec2(wndWidth, 200 + (150 * targNum)));
		theTargets[targ]->setSpriteTranslation(glm::vec2(-(rand() % 5 + 1) * 15, 0));
		//int randTarget = rand() % 4;
		theTargets[targ]->setTexture(theGameTextures[0]->getTexture());
		theTargets[targ]->setTextureDimensions(theGameTextures[0]->getTWidth(), theGameTextures[0]->getTHeight());
		theTargets[targ]->setSpriteCentre();
		theTargets[targ]->setTargetVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theTargets[targ]->setActive(true);
		theTargets[targ]->setMdlRadius();
		targNum++;
	}

}