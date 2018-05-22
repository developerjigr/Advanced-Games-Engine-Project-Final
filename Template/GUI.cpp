#include "GUI.h"

//NOTE:: MAKE DRAWING OPTIONS LESS AWKWARD AND COMPLICATED IF TIME
CGUI::CGUI() {}

CGUI::CGUI(CPlayer &player, GLfloat x, GLfloat y) 
{
	_SCREEN_WIDTH = x;
	_SCREEN_HEIGHT = y;

	_INTRO_OPTION_WIDTH = 200;
	_INTRO_OPTION_HEIGHT = _SCREEN_HEIGHT / _INTRO_OPTION_COUNT / 4;

	eCurrentInterface = INTRO_SCREEN;
	
	mCrossHairTexture.Load("Resources\\Textures\\crosshair.png", GL_REPEAT);
	mCrossHairTextureID = mCrossHairTexture.m_textureID;

	mAmmoTexture.Load("Resources\\Textures\\hud_ammo.png", GL_REPEAT);
	mAmmoTextureID = mAmmoTexture.m_textureID;

	mHealthTexture.Load("Resources\\Textures\\hud_health.png", GL_REPEAT);
	mHealthTextureID = mHealthTexture.m_textureID;

	mIntroMenuTexture_PLAY.Load("Resources\\Textures\\play_button.png", GL_REPEAT);
	mIntroMenuTextureID_PLAY = mIntroMenuTexture_PLAY.m_textureID;

	mIntroMenuTexture_EXIT.Load("Resources\\Textures\\exit_button.png", GL_REPEAT);
	mIntroMenuTextureID_EXIT = mIntroMenuTexture_EXIT.m_textureID;

	mIntroMenuTexture_ABOUT.Load("Resources\\Textures\\about_button.png", GL_REPEAT);
	mIntroMenuTextureID_ABOUT = mIntroMenuTexture_ABOUT.m_textureID;

	mIngameHUDTexture.Load("Resources\\Textures\\ingamehud.png", GL_REPEAT);
	mIngameHUDTextureID = mIngameHUDTexture.m_textureID;

	mAboutPageTexture.Load("Resources\\Textures\\about_page.png", GL_REPEAT);
	mAboutPageTextureID = mAboutPageTexture.m_textureID;
	
	mPlayer = &player;

	mOptionChangeAudio.Initialise();
	mOptionChangeAudio.LoadEventSound("Resources\\Audio\\Bounce.wav");


}
CGUI::~CGUI() {}

void CGUI::Render()
{
	glDisable(GL_LIGHTING);	
	glPushMatrix();
	OrthoMode();

	glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
	glDisable(GL_DEPTH_TEST);  // disable depth-testing
	glDisable(GL_BACK);

	switch(eCurrentInterface)
	{
		case INTRO_SCREEN:
			DrawIntroscreen();
			break;

		case PAUSE_SCREEN:
			DrawPauseScreen();
			break;

		case INGAME_HUD:
			DrawIngameHUD();
			break;

		default:
			DrawIngameHUD();
			break;
	}
	
	glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
	glEnable(GL_DEPTH_TEST);  // disable depth-testing
	glEnable(GL_BACK);

	glPopMatrix();

	PerspectiveMode();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void CGUI::DrawIntroscreen()
{
	//NOTE TO SELF:: ODD NUMBER OF MENU OPTIONS FOR NOW 
	int negator = -1;

	glClear(GL_COLOR);
	glColor4f(1, 1, 1, 1);

	//glTranslatef(0, 300, 0);
	//Checking for origin
	//DrawOrigin(0.25f);

	if (aboutScreen)
	{
		DrawAboutScreen();
	}
	else
	{
		glPushMatrix();


		//EVEN BARS CHANGE TO (IF WE WANT NO MIDDLE OPTION) -> _INTRO_OPTION_COUNT * _INTRO_OPTION_HEIGHT;
		//ODD (IF WE WANT NO SPACE BETWEEN THE OPTIONS) _INTRO_OPTION_COUNT * (_INTRO_OPTION_HEIGHT / 2) * 2
		float PADDING = _SCREEN_HEIGHT / _INTRO_OPTION_COUNT / 2;

		int n = 0;

		//GLfloat v0[2] { _INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT };		//TOP RIGHT CORNER
		//GLfloat v1[2]{ -_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT };		//TOP LEFT CORNER
		//GLfloat v2[2]{ -_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT };		//BOTTOM LEFT CORNER
		//GLfloat v3[2]{ _INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT };		//BOTTOM RIGHT CORNER

		//Counter for padding 
		int count = 0, count2 = 0;

		for (int i = 0; i < _SCREEN_HEIGHT / 2; i += _SCREEN_HEIGHT / _INTRO_OPTION_COUNT)
		{


			UINT texID = GetIntroTextureID(count2);

			if (i == 0)
			{
				//FIRST CENTER OPTION
				mIntroOptionCoords[count2] = CVector3f(0.0f, (float)i, 0.0f);

				if (texID != 0)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texID);
					glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glClearColor(1, 1, 1, 1);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
				}

				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glTexCoord2f(1, 1);		glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 1); 	glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 0); 	glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glTexCoord2f(1, 0); 	glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glEnd();

				//OUTLINES

				glLineWidth(0.5f);
				//glColor3f(0, 0, 0);
				glBegin(GL_LINES);

				//TOP LEFT TO RIGHT
				glVertex2f(_INTRO_OPTION_WIDTH, i + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, i + _INTRO_OPTION_HEIGHT);

				//LEFT TOP TO BOTTOM
				glVertex2f(-_INTRO_OPTION_WIDTH, i + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, i - _INTRO_OPTION_HEIGHT);

				//RIGHT TOP TO BOTTOM
				glVertex2f(_INTRO_OPTION_WIDTH, i + _INTRO_OPTION_HEIGHT);
				glVertex2f(_INTRO_OPTION_WIDTH, i - _INTRO_OPTION_HEIGHT);

				//BOTTOM RIGHT TO LEFT
				glVertex2f(-_INTRO_OPTION_WIDTH, i - _INTRO_OPTION_HEIGHT);
				glVertex2f(_INTRO_OPTION_WIDTH, i - _INTRO_OPTION_HEIGHT);

				glEnd();

				count2 += 1;
				//END OUTLINES 
			}
			else {

				//Positive bars
				n = i - PADDING * count;

				mIntroOptionCoords[count2] = CVector3f(0.0f, (float)n, 0.0f);

				texID = GetIntroTextureID(count2);

				if (texID != 0)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texID);
					glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glClearColor(1, 1, 1, 1);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
				}

				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glTexCoord2f(1, 1);		glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 1); 	glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 0); 	glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glTexCoord2f(1, 0); 	glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glEnd();

				//OUTLINES

				glLineWidth(0.5f);
				//glColor3f(0, 0, 0);

				glBegin(GL_LINES);
				//TOP LEFT TO RIGHT
				glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);

				//LEFT TOP TO BOTTOM
				glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);

				//RIGHT TOP TO BOTTOM
				glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);

				//BOTTOM RIGHT TO LEFT
				glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glEnd();

				//END OUTLINES 

				//Negative bars
				n *= negator;

				mIntroOptionCoords[count2 + 1] = CVector3f(0.0f, (float)n, 0.0f);

				texID = GetIntroTextureID(count2 + 1);

				if (texID != 0)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texID);
					glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glClearColor(1, 1, 1, 1);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
				}

				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glTexCoord2f(1, 1);		glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 1); 	glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glTexCoord2f(0, 0); 	glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glTexCoord2f(1, 0); 	glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glEnd();

				//OUTLINES


				glLineWidth(0.5f);
				//glColor3f(0, 0, 0);

				glBegin(GL_LINES);
				glVertex2f(_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, n + _INTRO_OPTION_HEIGHT);
				glVertex2f(-_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glVertex2f(_INTRO_OPTION_WIDTH, n - _INTRO_OPTION_HEIGHT);
				glEnd();

				//END OUTLINES 
				count2 += 2;
			}
			count += 1;

		}

		DrawSelectedOptionHighlight(mSelectedIntroOption);

		glPopMatrix();

	}


}

void CGUI::DrawSelectedOptionHighlight(int option)
{


	glPushMatrix();

	glDisable(GL_TEXTURE_2D);
	//OUTLINES
	glColor3f(0.75, 0.5, 0.075);
	glLineWidth(5.5f);

	glBegin(GL_LINES);
	//TOP LEFT TO RIGHT
	glVertex2f(_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y + _INTRO_OPTION_HEIGHT);
	glVertex2f(-_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y + _INTRO_OPTION_HEIGHT);

	//LEFT TOP TO BOTTOM
	glVertex2f(-_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y + _INTRO_OPTION_HEIGHT);
	glVertex2f(-_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y - _INTRO_OPTION_HEIGHT);

	//RIGHT TOP TO BOTTOM
	glVertex2f(_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y + _INTRO_OPTION_HEIGHT);
	glVertex2f(_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y - _INTRO_OPTION_HEIGHT);

	//BOTTOM RIGHT TO LEFT
	glVertex2f(-_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y - _INTRO_OPTION_HEIGHT);
	glVertex2f(_INTRO_OPTION_WIDTH, mIntroOptionCoords[option].y - _INTRO_OPTION_HEIGHT);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	//END OUTLINES 

}


void CGUI::ChangeIntroOption(int x)
{
	if (x % _INTRO_OPTION_COUNT == 0)
	{
		mSelectedIntroOption = 0;
	}

	eIntroSelectedOption = (IntroScreenOptions) mSelectedIntroOption;
}


void CGUI::IncrementIntroOption()
{
	mOptionChangeAudio.PlayEventSound();

	//Top values
	if (mSelectedIntroOption % 2 != 0)
	{
		//If its the last odd option (top most item)
		if (mSelectedIntroOption == (_INTRO_OPTION_COUNT - 2)) {
			mSelectedIntroOption = _INTRO_OPTION_COUNT - 1;
		}
		else
		{
			mSelectedIntroOption += 2;
		}
	}
	//bottom values
	else
	{
		if (mSelectedIntroOption == 0)
			mSelectedIntroOption += 1;
		else
			mSelectedIntroOption -= 2;
	}

	eIntroSelectedOption = (IntroScreenOptions) mSelectedIntroOption;
}

void CGUI::ReduceIntroOption()
{

	mOptionChangeAudio.PlayEventSound();


	//IF AT THE BOTTOM OF MENU LIST
	//Bottom values
	if (mSelectedIntroOption % 2 == 0)
	{
		//if its last even (last item)
		if (mSelectedIntroOption == (_INTRO_OPTION_COUNT - 1)) {
			mSelectedIntroOption = _INTRO_OPTION_COUNT - 2;
		}
		else 
		{
			mSelectedIntroOption += 2;
		}
	}
	//Top values
	else
	{
		if (mSelectedIntroOption == 1)
			mSelectedIntroOption -= 1;
		else
			mSelectedIntroOption -= 2;
	}

	eIntroSelectedOption = (IntroScreenOptions) mSelectedIntroOption;
}

void CGUI::DrawPauseScreen()
{
	float size = 250;
	//Crosshair

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mIngameHUDTextureID);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1, 1, 1, 1);

	glColor4f(1, 1, 1, 1);

	glPushMatrix();

	glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
	glDisable(GL_DEPTH_TEST);  // disable depth-testing
	glDisable(GL_BACK);


	glBegin(GL_QUADS);           // 
	glTexCoord2f(1, 1);     glVertex2f(size, size);
	glTexCoord2f(0, 1);     glVertex2f(-size, size);
	glTexCoord2f(0, 0);   glVertex2f(-size, -size);
	glTexCoord2f(1, 0);     glVertex2f(size, -size);
	glEnd();

	glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
	glEnable(GL_DEPTH_TEST);  // disable depth-testing
	glEnable(GL_BACK);

	glPopMatrix();
}

void CGUI::DrawAboutScreen()
{
	float size = 250;
	//Crosshair

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mAboutPageTextureID);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1, 1, 1, 1);

	glColor4f(1, 1, 1, 1);

	glPushMatrix();
	glScalef(1.25, 1.25, 1.25);
	glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
	glDisable(GL_DEPTH_TEST);  // disable depth-testing
	glDisable(GL_BACK);


	glBegin(GL_QUADS);           // 
	glTexCoord2f(1, 1);     glVertex2f(size, size);
	glTexCoord2f(0, 1);     glVertex2f(-size, size);
	glTexCoord2f(0, 0);   glVertex2f(-size, -size);
	glTexCoord2f(1, 0);     glVertex2f(size, -size);
	glEnd();

	glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
	glEnable(GL_DEPTH_TEST);  // disable depth-testing
	glEnable(GL_BACK);

	glPopMatrix();
}


void CGUI::DrawIngameHUD()
{

	float size = 75;
	//Crosshair
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mCrossHairTextureID);

	glPushMatrix();

	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	glBegin(GL_QUADS);           // 
	glTexCoord2f(1, 1);     glVertex2f(size, size);
	glTexCoord2f(0, 1);     glVertex2f(-size, size);
	glTexCoord2f(0, 0);   glVertex2f(-size, -size);
	glTexCoord2f(1, 0);     glVertex2f(size, -size);
	glEnd();
	glPopMatrix();


	

	healthPercentage = (mPlayer->GetHealth());
	healthPercentage = 10- floorf(healthPercentage * 100) / 1000;


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mHealthTextureID);

	glPushMatrix();
	glDisable(GL_BLEND);

	glTranslatef(0, 270, 0);
	size = 65.0f;

	glBegin(GL_QUADS);           // 
			
	glTexCoord2f(healthPercentage, 0);     glVertex2f(size * 3, -size / 3);// Front
	glTexCoord2f(healthPercentage, 1);     glVertex2f(size*3, size/3);
	glTexCoord2f(0, 1);     glVertex2f(-size * 3, size / 3);
	glTexCoord2f(0, 0);   glVertex2f(-size * 3, -size/3);

	glEnd();

	glEnable(GL_BLEND);
	glPopMatrix();
	//
}


void CGUI::PerspectiveMode()										// Set Up A Perspective View
{
	// This function doesn't really go to perspective mode, since we aren't
	// using gluPerspective(), but if you look in OrthoMode(), we push on
	// a matrix once we change to our projection matrix.  All we need to do is
	// pop that matrix off of our stack and it will return to our original perspective
	// matrix.  So, before we pop the current matrix, we need to tell OpenGL which
	// matrix we want to use by changing our current matrix mode to perspective.
	// Then we can go back to our model view matrix and render normally.

	// Enter into our projection matrix mode
	glMatrixMode(GL_PROJECTION);

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();

	// Go back to our model view matrix like normal
	glMatrixMode(GL_MODELVIEW);

	// We should be in the normal 3D perspective mode now
}


void CGUI::OrthoMode()
{
	//SWITCH TO PROJECTION MATRIX
	glMatrixMode(GL_PROJECTION);
	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();
	// Reset the current matrix to our identity matrix
	glLoadIdentity();
	//Pass in our 2D ortho screen coordinates.like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho(0, _SCREEN_WIDTH, 0, _SCREEN_HEIGHT, 0, 1);
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);
	// Initialize the current model view matrix with the identity matrix
	//Origin in centre screen
	glLoadIdentity();

	glTranslatef(_SCREEN_WIDTH / 2, _SCREEN_HEIGHT / 2, 0);

}


//DEBUG CENTER SCREEN
void CGUI::DrawOrigin(float scale)
{
	float size = 100.0f * scale;
	glBegin(GL_TRIANGLES);           // 
									 // Front
	glColor3f(1.0f, 0.0f, 0.0f);     glVertex2f(0.0f, size);
	glColor3f(0.0f, 100.0f, 0.0f);   glVertex2f(-size, -size);
	glColor3f(0.0f, 0.0f, 1.0f);     glVertex2f(size, -size);

	glEnd();

}




////glEnable(GL_TEXTURE_2D);
//glDisable(GL_TEXTURE_2D);
//glDisable(GL_LIGHTING);

//glColor3f(0.91, 0.2, 0.3);
//glPushMatrix();
//OrthoMode();
//glDisable(GL_DEPTH_TEST);

//glTranslatef(m_GameWindow.SCREEN_WIDTH / 2, m_GameWindow.SCREEN_WIDTH / 2, 0);

//glBegin(GL_TRIANGLES);

//glVertex2f(0, 50);
//glColor3f(0.91, 0.2, 0.3);
//glVertex2f(100, 50);
//glColor3f(0.91, 0.2, 0.3);
//glVertex2f(0, 100);
//glEnd();

//glEnable(GL_DEPTH_TEST);
//glEnable(GL_LIGHTING);
////
//glPopMatrix();


//PerspectiveMode();

//glEnable(GL_TEXTURE_2D);