#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include ".\include\glew.h"									// Header File For The Glaux Library
#include ".\include\gl.h"											// Header File For The OpenGL32 Library
#include ".\include\glu.h"									// Header File For The GLu32 Library

#include <math.h>

#include "gamewindow.h"
#include "Vector3f.h"
#include "GameObject.h"

#include "HighResolutionTimer.h"

#define MAX_VERTICAL_ROTATION 85 
#define MAX_HORIZONTAL_ROTATION 45

class Camera {
public:
	Camera();
	~Camera();

	enum eCameraStates { FOLLOWING_GAME_OBJECT, TOP_DOWN_MODE, FREE_MODE, TRANSITION_TO_GAMEOBJECT, TRANSITION_FROM_GAMEOBJECT };
	eCameraStates eCurrentCameraState = FOLLOWING_GAME_OBJECT;
	
	void Set(const CVector3f &position, const CVector3f &view, const CVector3f &up_vector, const float &speed);
	
	CVector3f GetPosition() const;		// Return the camera position (in world coordinates)
	CVector3f GetViewPoint() const;		// Return the point where the camera is looking (in world coordinates)
	CVector3f GetUpVector() const;		// Return the camera up vector (in world coordinates)
	CVector3f GetStrafeVector() const;	// Return the camera strafe vector (in world coordinates)

	void Strafe(float speed);	// Strafing moves the camera left / right
	void Advance(float speed);	// Advance moves the camera forward / backward
	void MoveUp(float speed);	// MoveUp moves the camera up / down

	void Update(double dt);
	void Look();				// Calls gluLookAt() to put the viewing matrix on the modelview matrix stack
	
	void RotateView(float angle, CVector3f &point);
	void SetViewByMouse();
	void Movement(double dt);

	void UpdateRail(double dt);
	void MovementRail(double dt);

	void FollowGameObject();
	void Follow(CGameObject& gameObj, float mCameraBackDistance, float mCameraLookDistance, float mCameraHeight);

	void LookDown();
	void LookForward();
	void LookBehind();

	void SwitchStates(eCameraStates state);

	void TransitionCameraFromGameObject(float dt);
	void TransitionCameraToGameObject(float dt);

	void TopDownView(float dt);

	//values to be added to current position of followed game object
	//which will increment and translate to a curve
	CVector3f CurveTranslations[4] = { CVector3f(0, 0, 0), CVector3f(-1.0f, 4.5f, -1.0f), CVector3f(-1.5f, 7.5f, -1.5f), CVector3f(-2.5f, 8.5f, -2.5f) };

	CVector3f ReturnPointOnCurve(CVector3f p1, CVector3f p2, CVector3f p3, CVector3f p4, float t);
	void SwitchCameraPos(float dt);

	CVector3f mReturnPoint;

	bool cameraMovement = false;

	void Render();

	CGameObject* mAttatchedGameObject;
	bool FollowingGameObject, CanFollowGameObject = false;

	void SmallCameraShake(bool on);
	void MediumCameraShake(bool on);
	void LargeCameraShake(bool on);

	CVector3f mTempShakeView;

	float randomX;
	float randomY;


	void UpdatePosition(CVector3f pos)
	{
		//dir.Normalise();
		m_position = pos;
		//m_viewPoint += CVector3f(pos.x, 0.0f, pos.z);
		
		//
	}

	void UpdateDir(CVector3f dir)
	{
		//dir.Normalise();
		m_viewPoint += dir;
		//Set(m_position, m_viewPoint, CVector3f(0, 1, 0), 25);
	}

	CVector3f m_viewPointUpdate;
	void Turn(float angle)
	{
		RotateView(angle, CVector3f(0, 1, 0));
	}

	const CVector3f forwardVector = CVector3f(0.0f, 0.0f, 1.0f);

	CHighResolutionTimer mTimer;

	
private:
	CVector3f m_position;
	CVector3f m_viewPoint;
	CVector3f m_upVector;
	CVector3f m_strafeVector;

	float m_speed;
	float m_totalVerticalRotation;
	FLOAT m_totalHorizontalRotation;

	CVector3f backPosition, frontPosition, cameraBackPosition, cameraViewPoint;
	float mCameraBackDistance, mCameraHeight, mCameraLookDistance;

	float MAX_FOLLOW_HEIGHT = 10.0f;
	float MAX_LOOKDOWN_HEIGHT = 60.0f;
	float PATH_DIST =  5.0f;

	CVector3f g_vStartPoint = { -0.5f,  0.0f,  0.0f };				// This is the starting point of the curve
	CVector3f g_vControlPoint1 = { -0.0f,  MAX_LOOKDOWN_HEIGHT, 0.0f };				// This is the first control point of the curve
	CVector3f g_vControlPoint2 = { +0.0f, MAX_LOOKDOWN_HEIGHT, 0.0f };				// This is the second control point of the curve
	CVector3f g_vEndPoint = { 0.01f,  MAX_LOOKDOWN_HEIGHT,  0.0f };				// This is the end point of the curve

	CVector3f vPoint = { 0.0f, 0.0f, 0.0f };

	float dist = 0;

	CVector3f mDirection = CVector3f(0, 0, 0);
	
	float d = 0;
	float projection = 0;

	CVector3f vLook;
	CVector3f vCentrePathPointA;
	CVector3f vCentrePathPointB;
	CVector3f closestPointOnLine;
	CVector3f vP;

	CVector3f perpPointToViewA;
	CVector3f perpPointToViewB;

	CVector3f playerFacingDirection;
	
	CVector3f mTempPosition = CVector3f(0, 0, 0);
	CVector3f mTempView = CVector3f(0, 0, 0);

	
};

#endif