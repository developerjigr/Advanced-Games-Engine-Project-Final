#pragma once
#include ".\include.\glew.h"
#include ".\include.\gl.h"
#include ".\include.\glut.h"
#include "Vector3f.h"
#include <map>

class CPhysics
{

public:

	CPhysics();
	~CPhysics();

	void Initialise(CVector3f position, CVector3f mInitialVelocity, float mass, float mSpeed, float height, float width, float depth );

	CVector3f Update(CVector3f currentPos, CVector3f direction, float dt);
	void UpdateCollisionPoints(CVector3f position);

	void ApplyForces(CVector3f dir);
	void SetSpeed(float speed, CVector3f dir);

	CVector3f GetZCollisionPoint0() { return mZCollisionPoints[0]; }
	CVector3f GetZCollisionPoint1() { return mZCollisionPoints[1]; }

	CVector3f GetYCollisionPoint0() { return mYCollisionPoints[0]; }
	CVector3f GetYCollisionPoint1() { return mYCollisionPoints[1]; }

	CVector3f GetXCollisionPoint0() { return mXCollisionPoints[0]; }
	CVector3f GetXCollisionPoint1() { return mXCollisionPoints[1]; }

	bool CheckCollisionY(CVector3f collisionPoints[2]);
	bool CheckCollisionX(CVector3f collisionPoints[2]);
	bool CheckCollisionZ(CVector3f collisionPoints[2]);

	void SetCollisionX(bool x);
	void SetCollisionY(bool y);
	void SetCollisionZ(bool z);

	//void MoveForward(CVector3f dir, float speed);
	void AddForce(float x, CVector3f dir);

	
	//Helpers
	CVector3f myClosestPointOnPlane(CVector3f vA, CVector3f vB, CVector3f vPoint);
	float PlaneDistance(CVector3f point, CVector3f normal);
	bool InteresectedPlane(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], CVector3f rayCastLine[2]);
	CVector3f CalcNormal(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
	CVector3f CalcNormal(CVector3f v0, CVector3f v1, CVector3f v2);
	float Distance(CVector3f v1, CVector3f v2);

	void DebugRayPoints();

	bool mPhysicsActive = true;


private:
	
	float m_contactTime;

	//Gravity needs to be applied with instantaneous
	const CVector3f mGravity = CVector3f(0.0f, -9.81f, 0.0f);
	CVector3f mAcceleration = CVector3f(0, 0, 0);
	CVector3f mVelocity = CVector3f(0, 0, 0);
	CVector3f mForce = CVector3f(0, 0, 0);
	CVector3f mLastPosition = CVector3f(0, 0, 0);

	float mSpeed = 10.0f;	
	float mMass = 50.0f;

	float mWidth = 2.0f;
	float mHeight = 2.0f;
	float mDepth = 2.0f;

	bool xCollision = false;
	bool yCollision = false;
	bool zCollision = false;

	bool collidedWithObject = false;
	
	/*
	x (p3)
	.
	.
	x (p2).........x (p1)

	*/

	//Coords for Floor Plane 
	GLfloat floorPlane1[3] = { 0, 0, 1 };
	GLfloat floorPlane2[3] = { 0, 0, 0 };
	GLfloat floorPlane3[3] = { 1, 0, 0 };

	//XPlane 0 
	CVector3f pX00 = CVector3f(0, 1, 0);	CVector3f pX01 = CVector3f(1, 0, 0);	CVector3f pX02 = CVector3f(0, 0, 1);	
	//XPLane 1
	CVector3f pX10 = CVector3f(1, 0, 0);	CVector3f pX11 = CVector3f(1, 0, 0);	CVector3f pX12 = CVector3f(1, 0, 0);

	//YPlane 0 
	CVector3f pY00 = CVector3f(0, 1, 0);	CVector3f pY01 = CVector3f(1, 0, 0);	CVector3f pY02 = CVector3f(0, 0, 1);

	//YPLane 1
	CVector3f pY10 = CVector3f(1, 0, 0);	CVector3f pY11 = CVector3f(1, 0, 0);	CVector3f pY12 = CVector3f(1, 0, 0);
	
	//YPlane 0 
	CVector3f pZ00 = CVector3f(0, 1, 0);	CVector3f pZ01 = CVector3f(1, 0, 0);	CVector3f pZ02 = CVector3f(0, 0, 1);

	//YPLane 1
	CVector3f pZ10 = CVector3f(1, 0, 0);	CVector3f pZ11 = CVector3f(1, 0, 0);	CVector3f pZ12 = CVector3f(1, 0, 0);

	CVector3f mXCollisionPoints[2];
	CVector3f mYCollisionPoints[2];
	CVector3f mZCollisionPoints[2];





};

