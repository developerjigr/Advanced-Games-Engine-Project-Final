#pragma once

#define _USE_MATH_DEFINES								// Will allow certain constants (like M_PI) to be availabe from <math.h>

#include <windows.h>									// Header File For the Windows Library
#include <stdio.h>										// Header File For the Standard Input/Output Functions
#include <stdlib.h>										// Header File For the Standard Library
#include <math.h>										// Header File For the Math header file
#include ".\include\glew.h"								// Header File For the GLEW Library
#include ".\include\gl.h"								// Header File For the OpenGL32 Library
#include ".\include\glu.h"								// Header File For the GLu32 Library

// Includes for game objects
#include "Camera.h"
#include "Gamewindow.h"
#include "InputManager.h"
#include "HighResolutionTimer.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "OpenAssetImporterMesh.h"
#include "BouncingBall.h"
#include "Audio.h"
#include "Text.h"
#include "Lighting.h"
#include "Material.h"
#include "Md2Animation.h"
#include "SpecialFX.h"
#include "Pickup.h"
#include "Tetrahedron.h"
#include "Player.h"
#include "WatchTower.h"
#include "StreetLamp.h"
#include "Jeep.h"
#include "Bullet.h"

#include "Enemy.h"
#include "LightningBolt.h"

#include "GUI.h"

class Game {
public:

	const float  PI_F = 3.14159265358979f;

private:
	void Initialise();		// Initialises the games (load assets, etc.)
	void Update();			// Update the simulation at each time step
	void Render();			// Render the scene

	//void OrthoMode();
	//void PerspectiveMode();
	
	void GameLoop();		// The game loop


	void RenderRandomObjects();

	// Game objects
	Camera m_camera;								// The camera
	CHighResolutionTimer m_highResolutionTimer;		// A timer for measuring the time between frames //TODO
	CTerrain m_terrain;								// The terrain (planar)
	CSkybox m_skybox;								// A skybox surrounding the scene
	CBouncingBall m_ball;							// A bouncing ball, showing simple physics
	CAudio m_audio;									// Audio using FMOD
	CText m_text;									// Text for the head's up display
	CLighting m_lighting;							// Lighting 
	CMaterial m_material;							// Material
	COpenAssetImporterMesh m_cow;					// A 3D mesh model 
	COpenAssetImporterMesh m_tree;					// Another 3D mesh model
	COpenAssetImporterMesh mGun;
	//COpenAssetImporterMesh m_streetLamps[10];
	COpenAssetImporterMesh mNatureSet;
	CMD2Model m_yohko;								// An animated (MD2) model
	CSpecialFX m_fx;

	COpenAssetImporterMesh m_mansion;


	//GAME OBJECTS

	const int mLampCount = 5;
	vector<CStreetLamp> mStreetLamps;

	CPickup mPickup;								//Pick up object 
	CTetrahedron mTetrahedron;
	CWatchTower mWatchTower;

	CPlayer mPlayer;
	CPlayer* mPlayer_ptr;

	CBullet myBullets[25];
	
	CJeep mJeep;
	CJeep mJeep2;

	CBullet b;


	CLightningBolt mLightningBolt;


	double m_dt;									// A variable to measure the amount of time elasped between frames
	int m_animation;								// Current animation of MD2 model

	//Game HUD -> Used for general gui
	CGUI GameGUI;
	bool m_introScreen;								// A boolean flag indicating if the intro screen is on

	//Global list of objects which will be spawned and used for collision
	vector<CGameObject*> vGameObjectList;

	//Global "Pool" of bullets -> Reduce excess amount of bullets 
	CBullet mGlobalBullets[20];
	vector<CBullet*> vBulletList;

	//Enemy Spawn variables
	CHighResolutionTimer mEnemySpawnTimer;
	int mCurrentEnemyIndex = 0;
	int mEnemyCount = 5;
	CEnemy mEnemyList[5];
	void SpawnEnemy();

	//Pickup Spawn variables
	CHighResolutionTimer mPickupSpawnTimer;
	int mCurrentPickupIndex = 0;
	int mPickUpCount = 5;
	CPickup mPickupList[5];
	void SpawnPickUp();
	float mNextSpawnTime = 0;
	int MIN_SPAWN_TIME_LIMIT = 20;

public:

	~Game();
	static Game& GetInstance();
	WPARAM Execute();
	void SetHinstance(HINSTANCE hinstance);
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);

private:
	Game::Game();
	Game::Game(const Game&);
	void Game::operator=(const Game&);

	GameWindow m_GameWindow;
	HINSTANCE m_hinstance;

	char* strCameraViewPoint;

	void LoadAssets();

	int mPickupScore = 0;

	int mGameScore;
	int mSpawnedEnemyCount = 0;

	void CountEnemies();


};