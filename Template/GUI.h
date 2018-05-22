#pragma once
#include ".\include\glew.h"									// Header File For The Glaux Library
#include ".\include\gl.h"											// Header File For The OpenGL32 Library
#include ".\include\glu.h"									// Header File For The GLu32 Library

#include <math.h>
#include <windows.h>

#include "gamewindow.h"
#include "Vector3f.h"
#include "GameObject.h"
#include "Primitives.h"
#include "Texture.h"
#include "Player.h"
#include "Audio.h"

const int _INTRO_OPTION_COUNT = 3;

class CGUI
{

public:
	CGUI();
	CGUI(CPlayer& player, GLfloat width, GLfloat height);
	~CGUI();

	enum IntroScreenOptions {INTRO_PLAY_GAME, INTRO_ABOUT_GAME, INTRO_EXIT_GAME};
	IntroScreenOptions eIntroSelectedOption = INTRO_PLAY_GAME;

	UINT CGUI::GetIntroTextureID(int option)
	{
		switch (option)
		{
		case 0:
			return mIntroMenuTextureID_PLAY;
			break;
		case 1:
			return mIntroMenuTextureID_ABOUT;
		case 2:
			return mIntroMenuTextureID_EXIT;
			break;
		default:
			break;
		}

		return 0;

	}

	enum InterfaceOptions {INTRO_SCREEN, PAUSE_SCREEN, INGAME_HUD};
	InterfaceOptions eCurrentInterface;

	void Render();

	void DrawIntroscreen();
	void DrawPauseScreen();
	void DrawAboutScreen();
	void DrawIngameHUD();

	void OrthoMode();
	void PerspectiveMode();

	void DrawSelectedOptionHighlight(int option);
	//For debugging current translate origin on screen
	void DrawOrigin(float scale);

	void ChangeIntroOption(int x);
	void IncrementIntroOption();
	void ReduceIntroOption();

	CPrimitives mPrimatives;
	//void DecreaseOption
	
	int ammo = 10;


	CPlayer* mPlayer;


	bool aboutScreen = false;

private:

	GLfloat _SCREEN_WIDTH = 800;
	GLfloat _SCREEN_HEIGHT = 600;

	GLfloat _INTRO_OPTION_WIDTH = 100;
	GLfloat _INTRO_OPTION_HEIGHT = 50;
	
	int mSelectedIntroOption = 0;

	/*
	Stores options with odd index at top, even at bottom 0 in middle 
	ex:
		3
		1
		0   - First menu option
		2
		4
	*/

	CVector3f mIntroOptionCoords[_INTRO_OPTION_COUNT];


	CTexture mCrossHairTexture;
	UINT mCrossHairTextureID;

	CTexture mAmmoTexture;
	UINT mAmmoTextureID;

	CTexture mHealthTexture;
	UINT mHealthTextureID;

	CTexture mIngameHUDTexture;
	UINT mIngameHUDTextureID;

	CTexture mIntroMenuTexture_PLAY;
	UINT mIntroMenuTextureID_PLAY;

	CTexture mIntroMenuTexture_EXIT;
	UINT mIntroMenuTextureID_EXIT;

	CTexture mIntroMenuTexture_ABOUT;
	UINT mIntroMenuTextureID_ABOUT;

	CTexture mAboutPageTexture;
	UINT mAboutPageTextureID;


	CAudio mOptionChangeAudio;

	int healthPercentage;


};