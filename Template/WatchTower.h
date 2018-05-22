#pragma once
#include "GameObject.h"



class CWatchTower : public CGameObject
{

public:

	CWatchTower();
	~CWatchTower();

	//ABstract functions from GameObject
	void Initialise();
	void Initialise(CVector3f pos, float scale);
	void Update(float dt);
	void Render();

	void DrawHorizontalBase();
	void DrawPillar();
	
	CVector3f CalcNormal(GLfloat v1[], GLfloat v2[], GLfloat v3[]);


private:

	float MAX_HEALTH = 1000.0f;
	float mHealth = 100.0f;
	
	float _WIDTH = 3.0f;
	float _PILLAR_HEIGHT = 4.0f;
	float _BASE_HEIGHT = 0.75f;

	
	UINT mTextureID;
	CTexture mTexture;


	float M_SCALE = 0.05f;

/*
	GLfloat mCameraLookDistance = 5.5f;
	GLfloat mCameraBackDistance = -10.5f;
	GLfloat mCameraHeight = 7.5f;
*/
};