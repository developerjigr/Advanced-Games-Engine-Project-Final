#include "Jeep.h"

void CJeep::Initialise(CVector3f pos, string meshName, float w, float h, float d)
{
	mPosition = pos;
	mWidth = w; mHeight = h, mDepth = d;
	mMesh = COpenAssetImporterMesh();
	mMesh.Load(meshName);
	mPhysics.Initialise(mPosition, CVector3f(0, 0, 0), 50.0f, 0.0f, w, h, d);
	mDirection = CVector3f(0, 0, 1);
	mSpeed = 15.5f;
	mGameObjectID = 2;
	mPhysics.SetSpeed(15.5f, mDirection);
}

void CJeep::Update(float dt)
{

	if (mPosition.z >= 100)
	{
		mPosition.z *= -0.5;
		mSpeed = 10 + rand() % 30;
		mPhysics.SetSpeed(mSpeed, mDirection);
	}

	mPosition = mPhysics.Update(mPosition, mDirection, dt);
}

void CJeep::Render()
{

		if (DebugModeOn) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 0.0f, 0.0f);
			glEnable(GL_TEXTURE_2D);
		}

		CVector3f directionVector = mDirection;
		directionVector.Normalise();

		glPushMatrix();
		glTranslatef(mPosition.x, mPosition.y, mPosition.z - 1.0f);

		glRotatef(rotation , 0, 1, 0);
		glScalef(2.5, 2.5, 2.5);
		mMesh.Render();
		glPopMatrix();

		glEnable(GL_LIGHTING);

		//mPhysics.DebugRayPoints();

}