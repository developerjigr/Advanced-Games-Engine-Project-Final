#include "Pickup.h"
#include ".\include\glut.h"

//Write method bodies in cpp class

CPickup::CPickup()
{}

CPickup::~CPickup()
{}

void CPickup::Initialise(CGameObject& player, CVector3f pos)
{
	mPlayer = &player;
	mPosition = pos;
	mTheta = 0.0f;
	isActive = false;

	mShockwave.Initialise();

	mPhysics.mPhysicsActive = false;
}


void CPickup::Update(float dt)
{

	if (!isActive)
		return;

	if (mMultiplierTimer.StartStopWatch(15))
	{
		mPlayer->mSpeed = oldPlayerSpeed;
		mPickupCountMultiplier = 0;
	}

	if (mPhysics.Distance(mPlayer->GetPosition(), mPosition) < 2)
	{
		oldPlayerSpeed = mPlayer->mSpeed;
		mPlayer->mSpeed + 5;
		mPickupCountMultiplier += 1;
		DeactivateObject();
	}

	if (mShockwaveTimer.StartStopWatch(2.5f))
	{
		mShockwave.Activate(2, mPosition);
	}

	if(mShockwave.IsActive())
		mShockwave.Update(dt);

	mTheta += dt * 50.0f;
	//Render();		
}

void CPickup::Render()
{
	if (isActive)
	{
		glColor3f(0.8, 0.35, 0.25);
		glPushMatrix();
		glTranslatef(mPosition.x, mPosition.y, mPosition.z);
		glRotatef(mTheta, 0, 1, 0);
		glScalef(1, 1, 1);

		glutSolidTorus(0.1f, 0.9f, 30, 30);
		glPopMatrix();

		glPushMatrix();
		if (mShockwave.IsActive())
			mShockwave.Render();
		glPopMatrix();
	} 


}


