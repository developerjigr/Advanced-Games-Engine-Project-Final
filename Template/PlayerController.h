#pragma once
#include "Vector3f.h"

class CPlayerController
{
	
public:
	CPlayerController();
	~CPlayerController();


	void Initialize();
	void UpdatePhysics(float dt);

	void ApplyForces();
	void ApplyHorizontalForce(float force);
	void ApplyVerticalForce(float force);






private:

	//Y is always constant
	CVector3f mSpeed = CVector3f(15.0f, 0.0f, 15.0f);
	CVector3f mVelocity;
	CVector3f mAcceleration;

	CVector3f mForce;
	
	

};