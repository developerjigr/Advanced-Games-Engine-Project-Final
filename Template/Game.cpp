/* 
OpenGL Template for IN3026
City University London, School of Informatics
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Christy Quinn, Sam Kellett, and others

 For educational use by School of Informatics, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, mesh loader, sipmle physics, texturing, audio

 Potential ways to modify the code:  Add new geometry types, change the terrain, load new meshes, set up lighting, 
 add in additional music / sound FX, modify camera control, put in a startup screen, collision detection, etc.
 
 Template version 3.0a 25/08/2014
 Dr. Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/

#include "Game.h"

// Constructor.  
Game::Game()  
{
	m_introScreen = true;
	m_dt = 0.0f;
	m_animation = 0;
}

// Destructor.  Technically we could delete all resources, but when the game ends the OS will free memory for us.
Game::~Game() 
{ 
	//delete mPlayer_ptr;
}

// Initialise the game by loading assets and setting up the scene.  Note, it's best to make OpenGL calls here since we know the rendering context is available.
void Game::Initialise() 
{

	//Screen overlay
	m_introScreen = true;
	m_text.Create(m_GameWindow.GetHdc(), "Arial", 18);
	glClearColor(1, 1, 1, 1);

	mPlayer.Initialise(m_camera, 1.25f, 2, 1.25f);
	mPlayer_ptr = &mPlayer;

	LoadAssets();
	
	GameGUI = CGUI(mPlayer, m_GameWindow.SCREEN_WIDTH, m_GameWindow.SCREEN_HEIGHT);

	m_camera.Follow(*mPlayer_ptr, 15.0f, 10.0f, 7.5f);
	m_skybox.Create("", 0.0f, m_camera);


	//Initalise all bullets
	for (int i = 0; i < 20; i++) {
		mGlobalBullets[i].Initialise();
		vBulletList.push_back(&mGlobalBullets[i]);
		vGameObjectList.push_back(&mGlobalBullets[i]);
	}

	//Initialise the enemies
	for (int i = 0; i < mEnemyCount; ++i)
	{
		mEnemyList[i].vBulletList = &vBulletList;
		vGameObjectList.push_back(&mEnemyList[i]);
		mEnemyList[i].SetActive(false);
	}

	//Initialise pickup items
	for (int i = 0; i < mPickUpCount; ++i)
	{
		mPickupList[i].Initialise();
		vGameObjectList.push_back(&mPickupList[i]);
		mPickupList[i].SetActive(false);
	}

	//Assign pointer to collision game objects to all other objects
	for (CGameObject* x : vGameObjectList)
	{
		x->vGameObjectList = &vGameObjectList;
	}


	mPlayer.vGameObjectList = &vGameObjectList;
	mPlayer.vBulletList = &vBulletList;

}


void Game::LoadAssets()
{
	m_terrain.Initialise();

	mJeep.Initialise(CVector3f(5.0f, 0.0f, -20.0f), "Resources\\Meshes\\jeep1.obj", 1.5f, 2.0f, 2.0f);
	vGameObjectList.push_back(&mJeep);

	m_audio.Initialise();
	m_audio.LoadEventSound("Resources\\Audio\\Bounce.wav");					// Royalty free sound from freesound.org
	m_audio.LoadMusicStream("Resources\\Audio\\GameThemeSong.mp3");	// Royalty free music from http://www.nosoapradio.us/

	m_tree.Load("Resources\\Meshes\\elm.3ds");
	m_cow.Load("Resources\\Meshes\\cow4.3ds");
	m_mansion.Load("Resources\\Meshes\\Mansion.obj");
	mNatureSet.Load("Resources\\Meshes\\BlenderNatureAsset.obj");

	for (int i = 0; i < 5; i+=1)
	{
		CStreetLamp l = CStreetLamp("B:\\Advanced Games\\models\\Exported Models\\Lighting\\street_lamp_double.obj", CVector3f((float)0.0f, 0.0f, (float)-15 + i * 20));
		mStreetLamps.push_back(l);
		mStreetLamps[i].index = i + 1;
		mStreetLamps[i].SetLampLight(i + 1);
	}

	mTetrahedron.Initialise();
	mWatchTower.Initialise(CVector3f(20, 0, 67), 2.0f);
	vGameObjectList.push_back(&mWatchTower);

	m_audio.PlayMusicStream();
}


//MIN_SPAWN_TIME_LIMIT IS THE MIN TIME UNTIL NEXT ENEMY CAN SPAWN

void Game::SpawnEnemy()
{
	if (mCurrentEnemyIndex % mEnemyCount == 0)
		mCurrentEnemyIndex = 0;
	
	mNextSpawnTime = rand() % 75 + MIN_SPAWN_TIME_LIMIT;
	CountEnemies();

	int mEnemyKillScore = 0;

	for (int i = 0; i < mEnemyCount; i++)
	{
		mEnemyKillScore += mEnemyList[i].mDeathCountMultiplier;
	}

	mGameScore += mEnemyKillScore;

	if (mEnemySpawnTimer.StartStopWatch(mNextSpawnTime))
	{
		if (!mEnemyList[mCurrentEnemyIndex].isActive)
		{
			int negator = (rand() % 100) > 60 ? -1 : 1;
			float x = rand() % 100 * negator;
			negator = (rand() % 100) > 60 ? -1 : 1;
			float z = rand() % 100 * negator;

			mTetrahedron.ChangePosition(CVector3f(x, 70.0f, z));
			mLightningBolt.Update();
			if (mCurrentEnemyIndex % 2 == 0) {
				mEnemyList[mCurrentEnemyIndex].Initialise(*mPlayer_ptr,
					"Resources\\Meshes\\enemy\\tris.md2", "Resources\\Meshes\\Yohko.bmp",
					"Resources\\Meshes\\enemy\\weapon.md2", "Resources\\Meshes\\YohkoWeapon.bmp",
					CVector3f(x, 0.0f, z), 1.0f, 1.5f, 1.0f);
			}
			else
			{
				mEnemyList[mCurrentEnemyIndex].Initialise(*mPlayer_ptr,
					"Resources\\Meshes\\enemy\\tris2.md2", "Resources\\Meshes\\Yohko.bmp",
					"Resources\\Meshes\\enemy\\weapon2.md2", "Resources\\Meshes\\YohkoWeapon.bmp",
					CVector3f(x, 0.0f, z), 1.2f, 1.5f, 1.2f);
			}

			mEnemyList[mCurrentEnemyIndex].isActive = true;
			mCurrentEnemyIndex += 1;
		}
		else
		{
			mCurrentEnemyIndex+=1;
		}
		
	}
	

}


void Game::SpawnPickUp()
{
	if (mCurrentPickupIndex % mPickUpCount == 0)
		mCurrentPickupIndex = 0;

	mNextSpawnTime = rand() % 30 + MIN_SPAWN_TIME_LIMIT;

	//count score
	mPickupScore = 0;

	for (int i = 0; i < mPickUpCount; i++)
	{
		mPickupScore += mPickupList[i].mPickupCountMultiplier;
	}

	mGameScore += mPickupScore;

	if (mPickupSpawnTimer.StartStopWatch(mNextSpawnTime))
	{
		if (!mPickupList[mCurrentPickupIndex].isActive)
		{
			int negator = (rand() % 100) > 60 ? -1 : 1;
			float x = rand() % 100 * negator;
			negator = (rand() % 100) > 60 ? -1 : 1;
			float z = rand() % 100 * negator;

			mPickupList[mCurrentPickupIndex].Initialise(*mPlayer_ptr, CVector3f(x, 1.2f, z));
			mPickupList[mCurrentPickupIndex].isActive = true;
			mCurrentPickupIndex += 1;
		}
		else
		{
			mCurrentPickupIndex += 1;
		}

	}


}

// Update game objects using a timer
void Game::Update() 
{

	//if (GameGUI.eCurrentInterface == CGUI::INTRO_SCREEN || GameGUI.eCurrentInterface == CGUI::PAUSE_SCREEN)
	//	return;

	if (GameGUI.eCurrentInterface == CGUI::INGAME_HUD)
	{

		SpawnEnemy();
		SpawnPickUp();

		//for (CGameObject* x : vGameObjectList)
		//{
		//	x->vGameObjectList = &vGameObjectList;
		//}

		//mPlayer.vGameObjectList = &vGameObjectList;
		//	mEnemy.vGameObjectList = vGameObjectList;
			// Update the camera
		m_camera.Update(m_dt);

		for (CGameObject* x : vGameObjectList)
		{
			x->Update(m_dt);
		}

		// Periodically update audio being played back by FMOD
		m_audio.Update();
		m_GameWindow.UpdateFrameRate(m_dt);

		//Update Models

		mPlayer.Update(m_dt);
		//mPlayer.UpdateCamera(m_camera);
		
		mTetrahedron.Update(m_dt);
	}

	HWND d = m_GameWindow.GetHwnd();

}


// Render the scene
void Game::Render() 
{
	// Clear the buffers and load the identity matrix into the modelview matrix
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Calling look will put the viewing matrix onto the modelview matrix stack
	m_camera.Look();

	if (m_introScreen == true) 
	{
		GameGUI.eCurrentInterface = CGUI::INTRO_SCREEN;
	} else 
	{
		if (GameGUI.eCurrentInterface == CGUI::INGAME_HUD)
		{
			m_skybox.Render();
			RenderRandomObjects();

			mPlayer.Render();

			for (CGameObject* x : vGameObjectList)
			{
				x->Render();
			}
			for (CBullet* x : vBulletList)
			{
				x->Render();
			}
		}

	}

	//Overhead display
	GameGUI.Render();
	SwapBuffers(m_GameWindow.GetHdc());		
}

void Game::CountEnemies()
{
	mSpawnedEnemyCount = 0;

	for (int i = 0; i < mEnemyCount; i++)
	{
		if (mEnemyList[i].isActive)
			mSpawnedEnemyCount += 1;
	}

}


void Game::RenderRandomObjects()
{

	// Get the camera position and view vector
	CVector3f vPos = m_camera.GetPosition();
	CVector3f vView = m_camera.GetViewPoint();
	CVector3f mCowPosition = CVector3f(0, 0, 30);
	CVector3f vCamCow = m_camera.GetPosition() - mCowPosition;
	CVector3f mJeepPosition = CVector3f(0, 0, 50);

	float angle = atan2f(vCamCow.x, vCamCow.z) * (180 / PI_F);

	
	glDisable(GL_LIGHTING);
		char strTextBuffer[50] = { 0 };			// We will store the string here for the window title
		sprintf_s(strTextBuffer, "Current Score %d", int(mGameScore));
		m_text.Render(strTextBuffer, 15, 25, 1, 0.5, 0);  // Draw some text -- useful for debugging and head's up displays
	glEnable(GL_LIGHTING);


	// -------------------------- Ambient light objects -----------------------
	m_camera.Render(); //NOTE:: GET RID OF (USED FOR DEBUGGING)

	// Set materials and lighting for the scene
	
	//-------------------------------------Light Based Objects----------------------------
	
	m_lighting.SetGlobalLighting();
	m_material.SetDefaultMaterial();
	m_terrain.Render();
	mTetrahedron.Render();

	for (int i = 0; i < mLampCount; i++)
	{
		glPushMatrix();
			mStreetLamps[i].Render();
		glPopMatrix();
	}


	// Render the cow
	glPushMatrix();
	glTranslatef(mCowPosition.x, mCowPosition.y, mCowPosition.z);
	glRotatef(angle, 0, 1, 0);
	m_cow.Render();
	glPopMatrix();


	//Render Mansion
	float m_s = 10;
	glPushMatrix();
	glTranslatef(25, 0, 25);
	glRotatef(-90, 0, 1, 0);

	glScalef(m_s, m_s, m_s);
	//m_tree.Render();
	m_mansion.Render();
	glPopMatrix();


	//Render trees
	
	for (int i = 0; i < 7; i++)
	{
		glPushMatrix();
		glTranslatef(cos(i) * -95, 0, sin(i) * 125);
		glScalef(m_s*0.75, m_s*0.75, m_s*0.75);
		mNatureSet.Render();
		glPopMatrix();
	}

}

void Game::GameLoop()
{
	// This code implements a variable timer
	m_highResolutionTimer.Start();

	if (!GameGUI.eCurrentInterface == CGUI::INTRO_SCREEN || !GameGUI.eCurrentInterface == CGUI::PAUSE_SCREEN)
		Update();


	Render();
	m_dt = m_highResolutionTimer.Elapsed();

}

WPARAM Game::Execute() 
{
	m_GameWindow.Init(m_hinstance);

	if(!m_GameWindow.GetHdc()) {
		return 1;
	}

	Initialise();

	m_highResolutionTimer.Start();

	double frame_duration = 1.0 / (double) 30.0;  // 30 FPS

	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		}else{
			GameLoop();
		} 
	}

	m_GameWindow.Deinit();

	return(msg.wParam);
}



LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	int mXPos;
	int mYPos;

	switch (message) {

	case WM_SIZE:
		if (!m_GameWindow.GetFullScreen()) {
			m_GameWindow.SizeOpenGLScreen(LOWORD(l_param), HIWORD(l_param));

			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_GameWindow.SetDimensions(dimensions);
		}
		break;

	case WM_LBUTTONDOWN:
		//SetCapture(window);
		if (mPlayer.isActive)
			mPlayer.Attack();
		break;

	case WM_LBUTTONUP:
	//	ReleaseCapture();
		break;

	case WM_CHAR:
		switch (w_param) {
		case '0': {
			// respond to user pressing 0 key
			m_fx.ActivateFog(0.01, 0.6, 0.6, 0.6);
		}
				  break;
		case '1': {
			m_introScreen = false;
		}
				  break;

		case '2': {
			mPlayer_ptr->Attack();
		}
				  break;
		case '3': {
			m_camera.SwitchStates(Camera::FREE_MODE);
			// respond to user pressing 2 key
		}
				  break;
		case '4': {
			GameGUI.IncrementIntroOption();
			//m_camera.SwitchStates(Camera::FOLLOWING_GAME_OBJECT);
			// respond to user pressing 2 key
		}
				  break;
		case '5': {
			glDisable(GL_LIGHT4);
			m_camera.SwitchStates(Camera::TRANSITION_TO_GAMEOBJECT);
			// respond to user pressing 2 key

		}
				  break;
		case '6': {
			m_camera.SwitchStates(Camera::TRANSITION_FROM_GAMEOBJECT);
			// respond to user pressing 2 key
		}
				  break;
		case 'w': {
			mPlayer.MoveForward();
		}
				  break;
		case 'd': {
		
		}
				  break;

		case 's': {
			mPlayer.MoveBackward();
		}
				  break;
		case 'p': {
			if (GameGUI.eCurrentInterface != GameGUI.PAUSE_SCREEN) {
				GameGUI.eCurrentInterface = GameGUI.PAUSE_SCREEN;
			}
			else
			{
				GameGUI.eCurrentInterface = GameGUI.INGAME_HUD;
			}
		}
				  break;
		}


		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(window, &ps);
			EndPaint(window, &ps);
			break;


		case WM_KEYDOWN:
			switch (w_param) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_SPACE:

				break;

			case VK_RETURN:
				if (GameGUI.eCurrentInterface == CGUI::INTRO_SCREEN)
				{
					if (GameGUI.aboutScreen)
					{
						GameGUI.aboutScreen = false;
					}
					else {
						switch (GameGUI.eIntroSelectedOption)
						{
							//Play Game
						case 0:
							GameGUI.aboutScreen = false;
							m_introScreen = false;
							GameGUI.eCurrentInterface = CGUI::INGAME_HUD;

							break;

							//About screen
						case 1:
							GameGUI.aboutScreen = true;
							break;

							//Exit Game
						case 2:
							PostQuitMessage(0);
							break;
						default:
							break;
						}
					}

				}

				break;

			case VK_SHIFT:
				mPlayer.Sprint();
				break;	

			case VK_UP:
				if (GameGUI.eCurrentInterface == CGUI::INTRO_SCREEN)
				{
					GameGUI.aboutScreen = false;
				GameGUI.IncrementIntroOption();
				}
				break;
	
			case VK_DOWN:
				if (GameGUI.eCurrentInterface == CGUI::INTRO_SCREEN)
				{
					GameGUI.aboutScreen = false;
					GameGUI.ReduceIntroOption();
				}
				else {

				}
				break;
		}

		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		default:
			result = DefWindowProc(window, message, w_param, l_param);
			break;
		}

		return result;
	
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hinstance = hinstance;
}


LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}




