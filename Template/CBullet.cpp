#include "Bullet.h"

void CBullet::Initialise(CVector3f pos, CVector3f dir, int collisionFilter, float mass, float speed, float w, float h, float d)
{
	mPosition = pos;
	mWidth = w; mHeight = h, mDepth = d;
	mDirection = dir;
	CVector3f mInitVelocity = dir * speed;
	mSpeed = speed;
	mPhysics.Initialise(mPosition, mInitVelocity, mass, 0.0f, w, h, d);
	isActive = false;

	mGameObjectID = 3;
	mCollisionFilter = -1;

	mSpecialBulletTexture.Load(".\\Resources\\Textures\\rockImage.jpg", GL_CLAMP);
	mSpecialBulletTextureID = mSpecialBulletTexture.m_textureID;
}

void CBullet::Update(float dt)
{
	if (!isActive)
		return;

	if (mTimer.StartStopWatch(5) || CheckCollisions(mCollisionFilter) > -1 ) {
		DeactivateObject();
	}

	mPhysics.AddForce(200, mDirection);
	mPhysics.SetSpeed(mSpeed, mDirection);
	mPosition = mPhysics.Update(mPosition, mDirection, dt);

//	mCollisionAudio.Update();

		
}

void CBullet::Render()
{
		if (!isActive)
			return;

		glPushMatrix();

			glDisable(GL_LIGHTING);
			glTranslatef(mPosition.x, mPosition.y, mPosition.z);
			//glScalef(4.15f, 4.15f, 4.15f);

			DrawSpecialBullet();

			glDisable(GL_TEXTURE_2D);
			glColor4f(0.5f, 0.65f, 0.1f, 1);
			glutSolidSphere(0.15f, 25, 25);
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);

		glPopMatrix();
}

void CBullet::DrawSpecialBullet()
{

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mSpecialBulletTextureID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glRotatef(rand() % 360, 1 - rand() % 1, 1 - rand() % 1, 1 - rand() % 1);

	glScalef(0.15f, 0.15f, 0.15f);

	glBegin(GL_QUAD_STRIP);
	glColor4f(0.75, 0.2, 0.7, 0.9f);

	for (int i = 0; i < 360; i += 360/5) 
	{
		float textureCoord = i / (2 * M_PI);
		float angle = i * M_PI / 180; // degrees to radians
		glTexCoord2f(textureCoord, 0.0f);		glVertex3f(1.5f * cos(angle), 1.5f * sin(angle), 0.0);
		glTexCoord2f(textureCoord, 1.0f);		glVertex3f(1.5f* cos(angle), 1.5f* sin(angle), 1.5f);
	}

	glEnd();

	glColor4f(0.5f, 0.65f, 0.1f, 1);

	glPopMatrix();

	glDisable(GL_BLEND);
}
