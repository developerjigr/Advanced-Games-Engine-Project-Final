#pragma once
#include "GameObject.h"


class CBullet : public CGameObject
{

public:

	CBullet() {}
	~CBullet() {}
	
	void Initialise() { mPosition = CVector3f(400, 400, 400); Render(); isActive = false; }

	void Initialise(CVector3f pos, CVector3f dir, int collisionFilter, float mass, float speed, float w, float h, float d);
	void Update(float dt);
	void Render();

	void SetActive(bool x) { isActive = x; }

	float mLifeTime = 1.0f;
	int mCollisionFilter = -1;

	bool CheckActive() { return isActive; }
	void DrawSpecialBullet();


	CAudio mCollisionAudio;

	CTexture mSpecialBulletTexture;
	UINT mSpecialBulletTextureID;

private:

	

};