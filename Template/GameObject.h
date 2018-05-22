#pragma once
#include <windows.h>
#define _USE_MATH_DEFINES
#include ".\include\GLew.H"
#include ".\include\GL.H"
#include ".\include\GLU.H"
#include ".\include\glut.H"
#include "Vector3f.h"
#include "Md2Animation.h"
#include <math.h>
#include "Texture.h"
#include "OpenAssetImporterMesh.h"
#include "Physics.h"
#include "Audio.h"
#include "HighResolutionTimer.h"

class CGameObject
{

public:
	
	const float  PI_F = 3.14159265358979f;

	virtual void Initialise() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	CVector3f GetPosition() { return mPosition; }
	CVector3f GetDirection() { return mDirection; }
	CVector3f GetRotation() { return mRotation; }

	void SetPosition(CVector3f newPos) { mPosition = newPos; }
	void SetDirection(CVector3f newDir) { mDirection = newDir; }
	void SetLookAngleX(float angle) { mLookAngleX = angle; }
	void SetLookAngleY(float angle) { mLookAngleY = angle; }

	bool DebugModeOn = false;

	void SetActive(bool active) {
		isActive = active;
	}

	CPhysics mPhysics;

	//0 -> OpenAssetImporter
	//1-> MD2 model


	void Turn(float angle)
	{
		mDirection = mDirection.RotateAboutAxis(angle, CVector3f(0, 1, 0));
	}

	void SetHealth(float h) { mHealth = h; }
	float GetHealth() { return mHealth; }
	void AddHealth(float health);
	
	void SetScore(float h) { mHealth = h; }
	float GetScore() { return mHealth; }
	void AddScore(float s) { mScore += s; }


	int CheckCollisions(int filter)
	{

		int collidedID = -1;
		CVector3f CollisionPointsX[2] = { mPhysics.GetXCollisionPoint0(), mPhysics.GetXCollisionPoint1() };
		CVector3f CollisionPointsY[2] = { mPhysics.GetYCollisionPoint0(), mPhysics.GetYCollisionPoint1() };
		CVector3f CollisionPointsZ[2] = { mPhysics.GetZCollisionPoint0(), mPhysics.GetZCollisionPoint1() };

		for (auto it = vGameObjectList->begin(); it != vGameObjectList->end(); ++it)
		{
			CGameObject* collisionObject = *it;

			if (!collisionObject->isActive || collisionObject->GetGameObjectID() == filter || collisionObject->GetGameObjectID() == mGameObjectID)
				continue;

			bool collisionX = collisionObject->mPhysics.CheckCollisionX(CollisionPointsX) || collisionObject->mPhysics.CheckCollisionX(CollisionPointsZ);
			bool collisionZ = collisionObject->mPhysics.CheckCollisionZ(CollisionPointsZ) || collisionObject->mPhysics.CheckCollisionZ(CollisionPointsX);
			bool collisionY = collisionObject->mPhysics.CheckCollisionY(CollisionPointsY);

			if (collisionX || collisionZ) {
				collidedID = collisionObject->mGameObjectID;
				mPhysics.SetCollisionX(collisionX);
				mPhysics.SetCollisionZ(collisionZ);
				return collidedID;
		}

		}
		return collidedID;
	}

	void DeactivateObject()
	{
		mPosition *= 1000;
		mPhysics.Update(mPosition, mDirection, 0.0001f);
		isActive = false;
	}

	bool isActive = true;

	int GetGameObjectID() { return mGameObjectID; }
	int mGameObjectID;

	CHighResolutionTimer mTimer;

	vector<CGameObject*>* vGameObjectList;



	/*
	bool CheckCollisions()
	{
		bool collided = false;
		mCollisionAudio.LoadEventSound(".\\Resources\\Audio\\Kick.wav");
		CVector3f CollisionPointsX[2] = { mPhysics.GetXCollisionPoint0(), mPhysics.GetXCollisionPoint1() };
		CVector3f CollisionPointsY[2] = { mPhysics.GetYCollisionPoint0(), mPhysics.GetYCollisionPoint1() };
		CVector3f CollisionPointsZ[2] = { mPhysics.GetZCollisionPoint0(), mPhysics.GetZCollisionPoint1() };

		for (auto it = vGameObjectList.begin(); it != vGameObjectList.end();)
		{
			CGameObject* collisionObject = *it;

			bool collisionX = collisionObject->mPhysics.CheckCollisionX(CollisionPointsX) || collisionObject->mPhysics.CheckCollisionX(CollisionPointsZ);
			bool collisionZ = collisionObject->mPhysics.CheckCollisionZ(CollisionPointsZ) || collisionObject->mPhysics.CheckCollisionX(CollisionPointsX);
			bool collisionY = collisionObject->mPhysics.CheckCollisionY(CollisionPointsY);

			if (collisionX || collisionZ) {
				mCollisionAudio.PlayEventSound();
			//	collided = true;
			}
			mPhysics.SetCollisionX(collisionX);
			mPhysics.SetCollisionZ(collisionZ);
			//mPhysics.SetCollisionY(collisionY);
			it++;
		}

		return collided;
	}

	*/
	float mHealth = 100;
	int mScore = 0;

	CVector3f mPosition;
	CVector3f mRotation;
	CVector3f mDirection;

	float mSpeed = 0.0f;
	float mHeight = 0.5f;
	float mWidth = 0.5f;
	float mDepth = 0.5f;

	CAudio mCollisionAudio;

	float mLookAngleX = 0.0f;
	float mLookAngleY = 0.0f;
	UINT mTextureID;

	CMD2Model mModel;
	COpenAssetImporterMesh mMesh;

	CVector3f movingDirection{};
	CVector3f defaultOrientation = CVector3f(0, 1, 0);





private:

	const float MIN_LIVES = 0;
	const float MAX_HEALTH = 100;


};