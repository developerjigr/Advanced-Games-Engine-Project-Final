#pragma once
#include <windows.h>									// Header File For The Windows Library

#include ".\include\gl.h"											// Header File For The OpenGL32 Library
#include ".\include\glut.h"											// Header File For The OpenGL32 Library
#include "Vector3f.h"

class CBouncingBall
{
public:
	CBouncingBall();
	~CBouncingBall();
	bool Initialise(CVector3f position, CVector3f velocity, CVector3f acceleration, float coefficientOfRestitution, float radius);
	bool UpdatePhysics(float dt);
	bool CollisionDetection(float yPlane);
	void CollisionResponse();
	void Render();

private:
	CVector3f m_position;
	CVector3f m_velocity;
	CVector3f m_acceleration;
	float m_coefficientOfRestitution;
	float m_radius;
	bool m_isActive;

	
};