#pragma once
#include "GameObject.h"
#include "Shockwave.h"


class CPickup : public CGameObject
{
public:
	//Constructor
	CPickup();
	~CPickup();

	void Initialise() 
	{ 
		mPosition = CVector3f(100, 100, 100); 
		isActive = true;
		Render();
		isActive = false;
	}


	void Initialise(CGameObject& player, CVector3f pos);
	void Update(float dt);
	void Render();

	int mPickupCountMultiplier = 0;

	CHighResolutionTimer mMultiplierTimer;
	CHighResolutionTimer mShockwaveTimer;
	int oldPlayerSpeed = 0;

	CShockwave mShockwave;
private:

	float mTheta;
	bool  mIsActive;
	CGameObject* mPlayer;





};