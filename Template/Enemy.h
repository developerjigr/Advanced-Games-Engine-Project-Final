#pragma once
#include "GameObject.h"
#include "HighResolutionTimer.h"
#include "Bullet.h"
#include "Shockwave.h"


#include <math.h>
#include <random>

class CEnemy : public CGameObject
{


public:
	CEnemy() {}
	~CEnemy() {}

	void Initialise() {}
	void Initialise(CGameObject& player, char* body, char* bodySkin, char* wep, char* wepSkin, CVector3f mPos, float w, float h, float d);
	void Update(float dt);
	void Render();

	CGameObject* mPlayer;
	vector<CBullet*>* vBulletList;

	bool initialised = false;

	enum Animations 
	{
		ANIM_IDLE, ANIM_WALK, ANIM_ATTACK, ANIM_HIT1, ANIM_HIT2, ANIM_HIT3, ANIM_JUMP, ANIM_FLIP,
		ANIM_SALUTE, ANIM_TAUNT, ANIM_WAVE, ANIM_CRSTAND, ANIM_CRWALK, ANIM_CRATT, ANIM_CRPAIN, ANIM_CRDEATH, ANIM_DEATH1, ANIM_DEATH2, ANIM_DEATH3
	};

	Animations eCurrentAnimation = ANIM_IDLE;

	enum States {IDLE, WALK, ATTACK, HIT, DEATH };
	States eCurrentState = IDLE;


	void UpdateState(States s);

	void Attack();
	void Walk(float dt);
	void Idle();
	void Death();
	void Hit();

	float mElapsedTime = 0.0f;

	CHighResolutionTimer mTimer;
	CHighResolutionTimer mAttackTimer;
	CHighResolutionTimer mHitTimer;
	CHighResolutionTimer mStateSwitchTimer;


	int ATTACK_DISTANCE = 10;

	float randTimer = 5.0f;

	int mAmmoCount = 5;
	int currentAmmo = 0;

	int ammoID = -1;

	bool CanAttack = false;

	const float OFFSET_Y = 1.2f;

	bool CanTakeDamage = false;

	CTexture mBloodTexture;
	UINT mBloodTextureID;

	CShockwave shockwaveDeath;

	CAudio mRobotSpawn;

	int mDeathCountMultiplier = 0;
	CHighResolutionTimer mMultiplierTimer;

	
	

};

