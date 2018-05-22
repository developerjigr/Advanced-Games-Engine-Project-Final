#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Bullet.h"


class CPlayer : public CGameObject
{

public:

	CPlayer();
	~CPlayer();

	//ABstract functions from GameObject
	void Initialise() {}
	void Initialise(Camera& camera, float w, float h, float d);
	void Update(float dt);
	void Render();

	void HandleMovement();
	void Attack();
	void Death();
	void Hit();

	void DebugGunPos();
	void LightOn();
	
	void MoveForward();
	void MoveBackward();
	void Sprint();

	void MoveLeft();
	void MoveRight();

	vector<CBullet*>* vBulletList;


	int mAmmoCount = 10;
	int currentAmmo = 0;
	int ammoID = -1;

	bool CanTakeDamage = true;
	bool isSprinting = false;
	bool isAttacking = false;

	bool IsFacingBackwards = false;

	void PerspectiveMode();
	void OrthoMode();

	CTexture mGunfireTexture;
	UINT mGunfireTextureID;

	CTexture mBloodSplatterTexture;
	UINT mBloodSplatterID;

	CTexture mDeathTexture;
	UINT mDeathTextureID;

	float alphaValue = 0;

	CAudio mGunShotAudio;




private:

	Camera* mCamera;
	float M_SCALE = 1.0f;

	GLfloat mCameraLookDistance = 5.5f;
	GLfloat mCameraBackDistance = -10.5f;
	GLfloat mCameraHeight = 7.5f;

	//Distant Point of where gun is aiming
	CVector3f lookPoint = CVector3f(0.0f, 5.0f, 10.0f);

	//TORCH VALUES
	GLfloat ambientLight1[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	bool movingForward = false;
	bool movingBackward = false;


	CHighResolutionTimer mHitTimer;



	
};