#include "Tetrahedron.h"
#include  "./include/glut.h"
#include  "./include/gl.h"
#include <windows.h>


CTetrahedron::CTetrahedron()
{

}

CTetrahedron::~CTetrahedron()
{

}

void CTetrahedron::ChangePosition(CVector3f newPos)
{
	mPosition.x = newPos.x;
	mPosition.y = newPos.y;
	mPosition.z = newPos.z;
}


void CTetrahedron::Initialise()
{
	mTexture.Load("Resources\\Textures\\rockImage.jpg", GL_REPEAT);
	mTextureID = mTexture.m_textureID;
}

void CTetrahedron::Update(float dt)
{
	mTheta += dt * 50.0f;
}

void CTetrahedron::Render()
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	GLfloat v0[3] = { 0, 10, 0 }; //Top mid point
	GLfloat v2[3] = { 0, 0, 10 }; //Left point
	GLfloat v1[3] = { -10, 0, 0 }; //Back left point
	GLfloat v3[3] = { 10, 0, 0 }; // mid backpoint
	GLfloat v4[3] = { 0, 0, -10 };

	//glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	glDisable(GL_LIGHTING);
	//glFrontFace(GL_CW);

	glPushMatrix();
	
	glColor4f(0.9f, 0.35f, 0.1f, 0.75f);
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glRotatef(180, 1, 0, 0);
	glRotatef(mTheta, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0);		glVertex3fv(v1);
	glTexCoord2f(0, 1);		glVertex3fv(v2);
	glTexCoord2f(0.5f, 1);	glVertex3fv(v0);

	glTexCoord2f(0, 0); 	glVertex3fv(v2);
	glTexCoord2f(0, 1);		glVertex3fv(v3);
	glTexCoord2f(0.5f, 1);	glVertex3fv(v0);

	glTexCoord2f(0.5f, 1);	glVertex3fv(v0);
	glTexCoord2f(0, 0);		glVertex3fv(v3);
	glTexCoord2f(0, 1);		glVertex3fv(v4);

	glTexCoord2f(0, 1);		glVertex3fv(v4);
	glTexCoord2f(0, 0);		glVertex3fv(v1);
	glTexCoord2f(0.5f, 1);	glVertex3fv(v0);

	glEnd();

	glPopMatrix();



}
