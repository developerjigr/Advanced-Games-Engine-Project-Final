#include "Player.h"

using namespace std;

CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{

}


void CPlayer::Initialise(Camera& cam, float width, float height, float depth)
{

	mGameObjectID = 0;

	mPosition = CVector3f(10.0f, 2.0f, 30.0f);
	mDirection = CVector3f(1, 0, 0);
	mSpeed = 0.5f;

	mCamera = &cam;

	mMesh = COpenAssetImporterMesh();
	mMesh.Load(".\\Resources\\Meshes\\mp5_exported.obj");

	mCollisionAudio.Initialise();
	mCollisionAudio.LoadEventSound(".\\Resources\\Audio\\Kick.wav");

	mGunShotAudio.Initialise();
	mGunShotAudio.LoadEventSound(".\\Resources\\Audio\\gunshot.wav");

	mGunfireTexture.Load(".\\Resources\\Textures\\gunshot.png", GL_CLAMP);
	mGunfireTextureID = mGunfireTexture.m_textureID;

	mBloodSplatterTexture.Load(".\\Resources\\Textures\\bloodsplat.png", GL_CLAMP);
	mBloodSplatterID = mBloodSplatterTexture.m_textureID;

	mDeathTexture.Load(".\\Resources\\Textures\\death.png", GL_CLAMP);
	mDeathTextureID = mDeathTexture.m_textureID;

	mPhysics.Initialise(mPosition, CVector3f(0, 0, 0), 50.0f, 0.0f, width, height, depth);

	ammoID = rand() % 1000;

}


void CPlayer::Update(float dt)
{
	if (!isActive)
		return;

	LightOn();
	CVector3f newPos;

	if (CheckCollisions(ammoID) > -1) {
		Hit();
	}
	else
	{
		if (mHitTimer.StartStopWatch(0.05f))
		{
			CanTakeDamage = true;
		}
	}

	newPos = mPhysics.Update(mCamera->GetPosition(), mDirection, dt);
	mCamera->UpdatePosition(newPos);
	mCamera->UpdateDir(mDirection);	
	mCamera->SmallCameraShake(movingForward);
	HandleMovement();
	mDirection.Normalise();

	mCollisionAudio.Update();

	//mCamera->UpdateDir(mDirection - newPos);
}

void CPlayer::Hit()
{
	if (CanTakeDamage)
	{
		mHealth -= 5;
		mCollisionAudio.PlayEventSound();
		CanTakeDamage = false;
	}
	else
	{


	}
}

void CPlayer::MoveForward()
{
	if (!movingForward)
	{
		movingBackward= false;
		movingForward = true;
		mPhysics.SetSpeed(7.5f, mDirection);
	}
	else
	{
		movingForward = false;
		mPhysics.SetSpeed(0.0f, mDirection);
	}
}


void CPlayer::MoveLeft()
{
	mPhysics.SetSpeed(-7.5f, CVector3f(-1, 0, -1));
}

void CPlayer::MoveRight()
{
	mPhysics.SetSpeed(-7.5f, CVector3f(1, 0, 1));
}

void CPlayer::MoveBackward()
{
	if (!movingBackward)
	{
		movingForward = false;
		movingBackward = true;
		mPhysics.SetSpeed(-7.5f, mDirection);
	}
	else
	{
		movingBackward = false;
		mPhysics.SetSpeed(0.0f, mDirection);
	}
}

void CPlayer::Sprint()
{
	int speed = isSprinting ? 7.5f : 14.5f;
		if (movingForward)
		{
			isSprinting = !isSprinting;
			mPhysics.SetSpeed(speed, mDirection);
		}
		else if (movingBackward)
		{
			isSprinting = !isSprinting;
			mPhysics.SetSpeed(-speed, mDirection);
		}

}


void CPlayer::Render()
{

	if (mHealth <= 0)
	{

			glPushMatrix();
			OrthoMode();

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mDeathTextureID);

			glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
			glDisable(GL_DEPTH_TEST);  // disable depth-testing
			glDisable(GL_BACK);

			float size = 175.0f;
			float xOffSet = IsFacingBackwards ? 0 : 0;
			float yOffSet = IsFacingBackwards ? 0 : 0;

			glBegin(GL_QUADS);           // 
										 // Front
			glTexCoord2f(1, 1);     glVertex2f(size + xOffSet, size + yOffSet);
			glTexCoord2f(0, 1);     glVertex2f(-size + xOffSet, size + yOffSet);
			glTexCoord2f(0, 0);   glVertex2f(-size + xOffSet, -size + yOffSet);
			glTexCoord2f(1, 0);     glVertex2f(size + xOffSet, -size + yOffSet);

			glEnd();

			glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
			glEnable(GL_DEPTH_TEST);  // disable depth-testing
			glEnable(GL_BACK);
			glDisable(GL_BLEND);

			glPopMatrix();

			PerspectiveMode();
			isActive = false;
			return;
	}

	if (isAttacking)
	{

		glPushMatrix();
		OrthoMode();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mGunfireTextureID);

		glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
		glDisable(GL_DEPTH_TEST);  // disable depth-testing
		glDisable(GL_BACK);

		float size = 75.0f;
		float xOffSet = IsFacingBackwards ? 50.0 : -50;
		float yOffSet = IsFacingBackwards ? -50.0 : -50;

		glBegin(GL_QUADS);           // 
									 // Front
		glTexCoord2f(1, 1);     glVertex2f(size + xOffSet, size + yOffSet);
		glTexCoord2f(0, 1);     glVertex2f(-size + xOffSet, size + yOffSet);
		glTexCoord2f(0, 0);   glVertex2f(-size + xOffSet, -size + yOffSet);
		glTexCoord2f(1, 0);     glVertex2f(size + xOffSet, -size + yOffSet);

		glEnd();

		glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
		glEnable(GL_DEPTH_TEST);  // disable depth-testing
		glEnable(GL_BACK);
		glDisable(GL_BLEND);

		glPopMatrix();

		PerspectiveMode();

		isAttacking = false;

	}

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y-3, mPosition.z);
	glRotatef(mLookAngleY, 1, 0, 0);
	glRotatef(mLookAngleX, 0, 1, 0);
	glScalef(M_SCALE, M_SCALE, M_SCALE);
	mMesh.Render();

	glPopMatrix();

	if (!CanTakeDamage)
	{
	
		glPushMatrix();
		OrthoMode();

		//alphaValue = (mHealth / 100);
		//glColor4f(1.0, 1.0, 1.0, (float)alphaValue);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mBloodSplatterID);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glColor4f(1.0, 1.0, 1.0, 0.5f);//Replace this alpha for transparency

		glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
		glDisable(GL_DEPTH_TEST);  // disable depth-testing
		glDisable(GL_BACK);

		float width = 400;
		float height = 300;

		glBegin(GL_QUADS);           // 
									 // Front
		glTexCoord2f(1, 1);     glVertex2f(width, height);
		glTexCoord2f(0, 1);     glVertex2f(-width, height);
		glTexCoord2f(0, 0);   glVertex2f(-width, -height);
		glTexCoord2f(1, 0);     glVertex2f(width, -height);

		glEnd();

		glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
		glEnable(GL_DEPTH_TEST);  // disable depth-testing
		glEnable(GL_BACK);
		glDisable(GL_BLEND);
	
		glPopMatrix();

		PerspectiveMode();

	}


//	glEnable(GL_LIGHTING);

}

void CPlayer::Attack()
{

	CanTakeDamage = true;
	isAttacking = true;

	mGunShotAudio.PlayEventSound();

	if (currentAmmo % (mAmmoCount) == 0)
		currentAmmo = 0;

	for (auto it = vBulletList->begin(); it != vBulletList->end(); ++it)
	{
		CBullet* c = *it;

		if (!c->isActive)
		{
			c->Initialise(mPosition, mDirection, mGameObjectID, 25, 50.0f, 0.125f, 0.125f, 0.25f);
			c->mGameObjectID = ammoID;
			c->SetActive(true);
			currentAmmo++;
			break;
		}
		else
		{
			continue;
		}

		//mPhysics.SetCollisionY(collisionY);
		//it++;
	}

//mBullets[currentAmmo].Initialise(mPosition, mDirection, mGameObjectID, 25, 50.0f, 0.125f, 0.125f, 0.25f);
//mBullets[currentAmmo].mGameObjectID = ammoID;
//mBullets[currentAmmo].SetActive(true);

	
	
}



//Controls the position of the gun
void CPlayer::HandleMovement()
{
	CVector3f vPos = mCamera->GetPosition();
	CVector3f vView = mCamera->GetViewPoint(); 
	CVector3f vStrafe = mCamera->GetStrafeVector();

	CVector3f vDirection = vView - vPos;
	vDirection.Normalise();

	float checkBackwards = mCamera->forwardVector * vDirection;

	float theta = 90.0f - (180.0f / M_PI) * acos(vDirection.y);
	float phi = (180.0f / M_PI) * atan2(vDirection.x, vDirection.z);
	SetDirection(vDirection);

	if (checkBackwards <= 0)
	{
		IsFacingBackwards = true;
		vDirection *= 3.75f;
		CVector3f p = CVector3f(vPos.x + vDirection.x, vPos.y - 0.5f, vPos.z + vDirection.z) + (0.75f * vStrafe);
		SetPosition(p);
		SetLookAngleY(theta);
	}
	else
	{
		IsFacingBackwards = false;
		vDirection *= -3.75f;
		CVector3f p = CVector3f(vPos.x - vDirection.x, vPos.y - 0.5f, vPos.z - vDirection.z) - (0.75f * vStrafe);
		SetPosition(p);
		SetLookAngleY(-theta);
	}

	SetLookAngleX(180 + phi);
}


void CPlayer::LightOn()
{

	glPushMatrix();
	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	GLfloat positionLight1[] = { mPosition.x, mPosition.y, mPosition.z, 1.0f };
	GLfloat ambientLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spotDir[] = { mDirection.x, mDirection.y, mDirection.z };


	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, positionLight1);


	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.01);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.07);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);

	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	//glDisable(GL_LIGHTING);
	glPopMatrix();

}

void CPlayer::PerspectiveMode()										// Set Up A Perspective View
{
	// This function doesn't really go to perspective mode, since we aren't
	// using gluPerspective(), but if you look in OrthoMode(), we push on
	// a matrix once we change to our projection matrix.  All we need to do is
	// pop that matrix off of our stack and it will return to our original perspective
	// matrix.  So, before we pop the current matrix, we need to tell OpenGL which
	// matrix we want to use by changing our current matrix mode to perspective.
	// Then we can go back to our model view matrix and render normally.

	// Enter into our projection matrix mode
	glMatrixMode(GL_PROJECTION);

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();

	// Go back to our model view matrix like normal
	glMatrixMode(GL_MODELVIEW);

	// We should be in the normal 3D perspective mode now
}


void CPlayer::OrthoMode()
{
	//SWITCH TO PROJECTION MATRIX
	glMatrixMode(GL_PROJECTION);
	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();
	// Reset the current matrix to our identity matrix
	glLoadIdentity();
	//Pass in our 2D ortho screen coordinates.like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho(0, 800, 0, 600, 0, 1);
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);
	// Initialize the current model view matrix with the identity matrix
	//Origin in centre screen
	glLoadIdentity();

	glTranslatef(800 / 2, 600 / 2, 0);
}