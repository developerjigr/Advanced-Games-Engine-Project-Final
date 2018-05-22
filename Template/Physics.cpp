#include "Physics.h"
#include <iostream>
#include "GameObject.h"

CPhysics::CPhysics()
{
}

void CPhysics::Initialise(CVector3f position, CVector3f mInitialVelocity, float mass, float speed, float width, float height, float depth)
{
	mLastPosition = position;
	mVelocity = mInitialVelocity;
	mMass = mass;
	mSpeed = speed;

	mHeight = height;
	mWidth = width;
	mDepth = depth;

	mYCollisionPoints[0] = CVector3f(0.0f, height, 0.0f);
	mYCollisionPoints[1] = CVector3f(0.0f, -height, 0.0f);
	mXCollisionPoints[0] = CVector3f(width, 0.0f, 0.0f);
	mXCollisionPoints[1] = CVector3f(-width, 0.0f, 0.0f);
	mZCollisionPoints[0] = CVector3f(0.0f, 0.0f, depth);
	mZCollisionPoints[1] = CVector3f(0.0f, 0.0f, -depth);

}

CPhysics::~CPhysics() {}


CVector3f CPhysics::Update(CVector3f mPos, CVector3f dir, float dt)
{
	if (!mPhysicsActive)
		return mPos;

	mLastPosition = mPos;
	UpdateCollisionPoints(mPos);

	mVelocity = mSpeed * dir;

	if (mAcceleration.Length() > 0 && m_contactTime > 0.05) {
		mAcceleration = CVector3f(0, 0, 0);
		m_contactTime = 0;
	}

	m_contactTime += dt;
	

	mVelocity += (mGravity + mAcceleration) * dt;
	mPos += (mVelocity)* dt;

	if (xCollision)
	{
		mPos.x += mVelocity.x * -0.175f;
		mVelocity.x = 0.0f;
	}

	if (zCollision)
	{

		mPos.z += mVelocity.z * -0.075f;
		mVelocity.z = 0.0f;
	}

	//Currently Disabled, only floor intersection for Y axis being used
	//if (yCollision)
	//{

	//	mPos.y += mVelocity.y * -0.075f;
	//	mVelocity.y = 0.0f;
	//}

	bool collisionY = InteresectedPlane(floorPlane3, floorPlane2, floorPlane1, mYCollisionPoints);

	if (collisionY)
	{
		mVelocity.y = 0.0f;
		mPos.y = mLastPosition.y;
	}

	xCollision = false;
	zCollision = false;
	yCollision = false;

	//Update Ray Points to current position

	 //Applies gravity and current force
	//mSpeed = 0;
	return mPos;
}

//void CPhysics::MoveForward(CVector3f dir, float speed)
//{
//	mForce = 0.5f * dir;
//	mAcceleration = mForce / mMass;
//	mSpeed = speed;
//}

void CPhysics::AddForce(float x, CVector3f dir)
{
	mForce = x * dir;
	mAcceleration = mForce / mMass;
}


void CPhysics::SetSpeed(float s, CVector3f dir)
{
	mSpeed = s;
	mVelocity = mSpeed * dir;
}

//Updates the position of the collision points + calulates the new plane positions
void CPhysics::UpdateCollisionPoints(CVector3f mPos)
{

	mYCollisionPoints[0] = CVector3f(mPos.x, mPos.y + mHeight, mPos.z);	mYCollisionPoints[1] = CVector3f(mPos.x, mPos.y - mHeight, mPos.z);
	mXCollisionPoints[0] = CVector3f(mPos.x + mWidth, mPos.y, mPos.z);	mXCollisionPoints[1] = CVector3f(mPos.x - mWidth, mPos.y, mPos.z);
	mZCollisionPoints[0] = CVector3f(mPos.x, mPos.y, mPos.z + mDepth);	mZCollisionPoints[1] = CVector3f(mPos.x, mPos.y, mPos.z - mDepth);

	//-------------------------------- X PLANE POINTS --------------------------------

	pX00 = CVector3f(mXCollisionPoints[0].x, mXCollisionPoints[0].y + 1, mXCollisionPoints[0].z);
	pX01 = mXCollisionPoints[0];
	pX02 = CVector3f(mXCollisionPoints[0].x, mXCollisionPoints[0].y, mXCollisionPoints[0].z - 1);

	pX10 = CVector3f(mXCollisionPoints[1].x, mXCollisionPoints[1].y + 1, mXCollisionPoints[1].z);
	pX11 = mXCollisionPoints[1];
	pX12 = CVector3f(mXCollisionPoints[1].x, mXCollisionPoints[1].y, mXCollisionPoints[1].z + 1);

	//-------------------------------- Y PLANE POINTS --------------------------------


	pY00 = CVector3f(mYCollisionPoints[0].x + 1, mYCollisionPoints[0].y, mYCollisionPoints[0].z);
	pY01 = mYCollisionPoints[0];
	pY02 = CVector3f(mYCollisionPoints[0].x, mYCollisionPoints[0].y, mYCollisionPoints[0].z + 1);

	pY10 = CVector3f(mYCollisionPoints[1].x + 1, mYCollisionPoints[1].y, mYCollisionPoints[1].z);
	pY11 = mYCollisionPoints[1];
	pY12 = CVector3f(mYCollisionPoints[1].x, mYCollisionPoints[1].y, mYCollisionPoints[1].z - 1);

	//-------------------------------- Z PLANE POINTS --------------------------------

	pZ00 = CVector3f(mZCollisionPoints[0].x, mZCollisionPoints[0].y + 1, mZCollisionPoints[0].z);
	pZ01 = mZCollisionPoints[0];
	pZ02 = CVector3f(mZCollisionPoints[0].x - 1, mZCollisionPoints[0].y, mZCollisionPoints[0].z);

	pZ10 = CVector3f(mZCollisionPoints[1].x, mZCollisionPoints[1].y + 1, mZCollisionPoints[1].z);
	pZ11 = mZCollisionPoints[1];
	pZ12 = CVector3f(mZCollisionPoints[1].x + 1, mZCollisionPoints[1].y, mZCollisionPoints[1].z);
}


CVector3f CPhysics::myClosestPointOnPlane(CVector3f vA, CVector3f vB, CVector3f vP)
{

	CVector3f v1 = vP - vA; //Point and A line 
	CVector3f v2 = (vB - vA); //Line Vector direction
	v2.Normalise();

	float vABDistance = Distance(vA, vB);

	//If the dot 
	float v1ProjOnV2 = v1 * v2;

	if (v1ProjOnV2 <= 0)
		return vA;

	if (v1ProjOnV2 >= vABDistance)
		return vB;

	CVector3f vLine = v2 * v1ProjOnV2;

	CVector3f vClosestPoint = vLine + vA;

	return vClosestPoint;
}

void CPhysics::SetCollisionX(bool x)
{
	xCollision = x;
}

void CPhysics::SetCollisionY(bool x)
{
	yCollision = x;
}

void CPhysics::SetCollisionZ(bool x)
{
	zCollision = x;
}

bool CPhysics::CheckCollisionX(CVector3f collisionPoints[2])
{

	float distance = Distance(mLastPosition, collisionPoints[0]);
	if (distance <= 3.0f) {

		GLfloat p1[3] = { pX00.x, pX00.y, pX00.z };
		GLfloat p2[3] = { pX01.x, pX01.y, pX01.z };
		GLfloat p3[3] = { pX02.x, pX02.y, pX02.z };

		GLfloat p4[3] = { pX10.x, pX10.y, pX10.z };
		GLfloat p5[3] = { pX11.x, pX11.y, pX11.z };
		GLfloat p6[3] = { pX12.x, pX12.y, pX12.z };

		const bool& c = ((InteresectedPlane(p1, p2, p3, collisionPoints) || InteresectedPlane(p6, p5, p4, collisionPoints)));
		return c;
	}

	return false;
}

bool CPhysics::CheckCollisionY(CVector3f rayPoints[2])
{
	float distance = Distance(mLastPosition, rayPoints[0]);
	if (distance <= 3.0f) {
	GLfloat p1[3] = { pY00.x, pY00.y, pY00.z };
	GLfloat p2[3] = { pY01.x, pY01.y, pY01.z };
	GLfloat p3[3] = { pY02.x, pY02.y, pY02.z };

	GLfloat p4[3] = { pY10.x, pY10.y, pY10.z };
	GLfloat p5[3] = { pY11.x, pY11.y, pY11.z };
	GLfloat p6[3] = { pY12.x, pY12.y, pY12.z };

	const bool& c = ((InteresectedPlane(p1, p2, p3, rayPoints) || InteresectedPlane(p4, p5, p6, rayPoints)));
	return c;
	}
	return false;
}

bool CPhysics::CheckCollisionZ(CVector3f rayPoints[2])
{
	float distance = Distance(mLastPosition, rayPoints[0]);

	if (distance <= 3.0f) {
	GLfloat p1[3] = { pZ00.x, pZ00.y, pZ00.z };
	GLfloat p2[3] = { pZ01.x, pZ01.y, pZ01.z };
	GLfloat p3[3] = { pZ02.x, pZ02.y, pZ02.z };

	GLfloat p4[3] = { pZ10.x, pZ10.y, pZ10.z };
	GLfloat p5[3] = { pZ11.x, pZ11.y, pZ11.z };
	GLfloat p6[3] = { pZ12.x, pZ12.y, pZ12.z };

	const bool& c = ((InteresectedPlane(p1, p2, p3, rayPoints) || InteresectedPlane(p4, p5, p6, rayPoints)));
	return c;
	}
	return false;
}

CVector3f CPhysics::CalcNormal(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
	CVector3f vv0 = CVector3f(v0[0], v0[1], v0[2]);
	CVector3f vv1 = CVector3f(v1[0], v1[1], v1[2]);
	CVector3f vv2 = CVector3f(v2[0], v2[1], v2[2]);

	CVector3f dV1 = vv2 - vv1;
	CVector3f dV2 = vv0 - vv1;

	CVector3f norm = dV2 % dV1;
	norm.Normalise();
	return norm;
}



CVector3f CPhysics::CalcNormal(CVector3f v0, CVector3f v1, CVector3f v2)
{
	CVector3f vv0 = v0;
	CVector3f vv1 = v1;
	CVector3f vv2 = v2;

	CVector3f dV1 = vv2 - vv1;
	CVector3f dV2 = vv0 - vv1;

	CVector3f norm = dV2 % dV1;
	norm.Normalise();
	return norm;
}



float CPhysics::Distance(CVector3f v1, CVector3f v2)
{
	float distance = 0;

	float x = (v2.x - v1.x);
	float y = (v2.y - v1.y);
	float z = (v2.z - v1.z);

	distance = (pow(x, 2) + pow(y, 2) + pow(z, 2));

	return sqrt(distance); //Could remove
}

//From tutorials
float CPhysics::PlaneDistance(CVector3f point, CVector3f normal)
{

	//PlaneDistance eq = Ax + By + Cz + d = 0;
	float d = 0;
	d = -((point.x * normal.x) + (point.y * normal.y) + (point.z * normal.z));
	return d;
}


//From tutorials
//checks if two points are between a plane 
//One point distance be positive and the other will be negative if they are both interesected 
bool CPhysics::InteresectedPlane(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], CVector3f rayPoints[2])
{
	float originDistance, rayPointDistA, rayPointDistB;
	CVector3f planeNormal = CalcNormal(p1, p2, p3);


	//Find D
	originDistance = PlaneDistance(CVector3f(p1[0], p1[1], p1[2]), planeNormal);


	//Ax + By+ Cz + D = 0;
	rayPointDistA =
		(planeNormal.x * rayPoints[0].x) +
		(planeNormal.y * rayPoints[0].y) +
		(planeNormal.z * rayPoints[0].z) + originDistance;


	rayPointDistB =
		(planeNormal.x * rayPoints[1].x) +
		(planeNormal.y * rayPoints[1].y) +
		(planeNormal.z * rayPoints[1].z) + originDistance;


	float val = rayPointDistA * rayPointDistB;

	//if both are behind or ahead then the result of the multiplication will be positive
	if (val <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}


void CPhysics::DebugRayPoints()
{

	glColor3f(0.75f, 0.15f, 0.15f);

	glDisable(GL_LIGHTING);

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	glVertex3f(pX00.x, pX00.y, pX00.z);
	glVertex3f(pX01.x, pX01.y, pX01.z);
	glVertex3f(pX02.x, pX02.y, pX02.z);

	glVertex3f(pY00.x, pY00.y, pY00.z);
	glVertex3f(pY01.x, pY01.y, pY01.z);
	glVertex3f(pY02.x, pY02.y, pY02.z);
	//
	glVertex3f(pZ00.x, pZ00.y, pZ00.z);
	glVertex3f(pZ01.x, pZ01.y, pZ01.z);
	glVertex3f(pZ02.x, pZ02.y, pZ02.z);

	glEnd();

	glPopMatrix();

	if (collidedWithObject)
		glColor3f(1, 1, 1);

	glPushMatrix();
	glTranslatef(mYCollisionPoints[0].x, mYCollisionPoints[0].y, mYCollisionPoints[0].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mYCollisionPoints[1].x, mYCollisionPoints[1].y, mYCollisionPoints[1].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mXCollisionPoints[0].x, mXCollisionPoints[0].y, mXCollisionPoints[0].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mXCollisionPoints[1].x, mXCollisionPoints[1].y, mXCollisionPoints[1].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mZCollisionPoints[0].x, mZCollisionPoints[0].y, mZCollisionPoints[0].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mZCollisionPoints[1].x, mZCollisionPoints[1].y, mZCollisionPoints[1].z);
	glutSolidSphere(0.125f, 25, 25);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}



