#include "WatchTower.h"
#include <math.h>


//Default Constructer / Deconstruct
CWatchTower::CWatchTower() {}
CWatchTower::~CWatchTower() {}


void CWatchTower::Initialise()
{
	//mPosition = CVector3f(5.0f, 5.0f, 10.0f);

	mTexture.Load("Resources\\Textures\\wood.jpg", GL_REPEAT);
	mTextureID = mTexture.m_textureID;
}

void CWatchTower::Initialise(CVector3f pos, float scale)
{
	//SCALE UNUSED
	mPosition = pos;
	mTexture.Load("Resources\\Textures\\wood.jpg", GL_REPEAT);
	mTextureID = mTexture.m_textureID;

	mPhysics.Initialise(mPosition, CVector3f(0, 0, 0), 100, 0.0f, 5*scale, 2.5*scale, 2.5*scale);
	M_SCALE = scale;
	mGameObjectID = 10;
}

//STATIC OBJECT 
void CWatchTower::Update(float dt)
{
	mPhysics.Update(mPosition, mDirection, dt);
	return;
}

void CWatchTower::Render()
{
	float xPos = ((_WIDTH) / 4) * 3; // 7.5
	float zPos = ((_WIDTH) / 4) * 3;
	float yPos = (_PILLAR_HEIGHT);

	glPushMatrix();

	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glScalef(M_SCALE, M_SCALE, M_SCALE);

	//+-
	glPushMatrix();
	glTranslatef(xPos, 0.0f, -zPos);
	glScalef(1, 2, 1);
	DrawPillar();
	glPopMatrix();

	//++
	glPushMatrix();
	glTranslatef(xPos, 0.0f, zPos);
	glScalef(1, 2, 1);
	DrawPillar();
	glPopMatrix();

	//-+
	glPushMatrix();
	glTranslatef(-xPos, 0.0f, zPos);
	glScalef(1, 2, 1);
	DrawPillar();
	glPopMatrix();

	//--
	glPushMatrix();
	glTranslatef(-xPos, 0.0f, -zPos);
	glScalef(1, 2, 1);
	DrawPillar();
	glPopMatrix();


	//Bottom Pillar is double height
	yPos *= 2;

	//BOTTOM BASE
	glPushMatrix();
	glTranslatef(0.0f, yPos, 0.0f);
	//glRotatef(180, 1, 0, 0);
	DrawHorizontalBase();
	glPopMatrix();

	//SUB PILLARS

	//+-
	glPushMatrix();
	glTranslatef(xPos, yPos + _BASE_HEIGHT, -zPos);
	DrawPillar();
	glPopMatrix();

	//++
	glPushMatrix();
	glTranslatef(xPos, yPos + _BASE_HEIGHT, zPos);
	DrawPillar();
	glPopMatrix();

	//-+
	glPushMatrix();
	glTranslatef(-xPos, yPos + _BASE_HEIGHT, zPos);
	DrawPillar();
	glPopMatrix();

	//--
	glPushMatrix();
	glTranslatef(-xPos, yPos + _BASE_HEIGHT, -zPos);
	DrawPillar();
	glPopMatrix();

	//TOP ROOF BASE
	glPushMatrix();
	glTranslatef(0.0f, yPos + _BASE_HEIGHT + _PILLAR_HEIGHT, 0.0f);
	//glRotatef(180, 1, 0, 0);
	DrawHorizontalBase();
	glPopMatrix();
	glPopMatrix();

	//mPhysics.DebugRayPoints();

}

void CWatchTower::DrawHorizontalBase()
{	//ROOF SQUARE
	GLfloat x = _WIDTH; //1.25f
	GLfloat y = _BASE_HEIGHT;
	float z = x;

	GLfloat v0[3] = { -x, 0.0f, -x }, //bottom
			v1[3] = { -x, 0.0f, +x }, // top left
			v2[3] = { +x, 0.0f, +x },// top right
			v3[3] = { +x, 0.0f, -x }, // bottomright

			v4[3] = { -x, +y, -x },
			v5[3] = { -x, +y, +x },
			v6[3] = { +x, +y, +x },
			v7[3] = { +x, +y, -x };

	int rows = 10;
	int cols = 10;

	vector<GLfloat[3]> vectorArray = vector<GLfloat[3]>();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glBegin(GL_QUADS);
	glShadeModel(GL_SMOOTH);
	//Bottom Side

	CVector3f norm = CalcNormal(v2, v3, v0);

	//0, -1, 0
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v1);

	//Back Side

	norm = CalcNormal(v5, v1, v0);
	//expeced norm: -x
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v5);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v4);

	//Right back side

	norm = CalcNormal(v6, v2, v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v6);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v5);

	//Front side
	norm = CalcNormal(v7, v3, v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v6);

	norm = CalcNormal(v4, v0, v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v4);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v7);

	//Top Side
	norm = CalcNormal(v5, v4, v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v4);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v5);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v6);
	glEnd();
		
	glEnable(GL_LIGHTING);
}

CVector3f CWatchTower::CalcNormal(GLfloat v0[], GLfloat v1[], GLfloat v2[])
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

void CWatchTower::DrawPillar()
{	//PILLAR
	GLfloat x = _WIDTH / 32; //2.5f
	GLfloat y = _PILLAR_HEIGHT;

	GLfloat v0[3] = { -x, 0.0f, -x },
		v1[3] = { -x, 0.0f, +x },
		v2[3] = { +x, 0.0f, +x },
		v3[3] = { +x, 0.0f, -x },

		v4[3] = { -x, +y, -x },
		v5[3] = { -x, +y, +x },
		v6[3] = { +x, +y, +x },
		v7[3] = { +x, +y, -x };
	
	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	/* -> Binding Texture */ glBindTexture(GL_TEXTURE_2D, mTextureID);
	
	//glPushMatrix();
	glBegin(GL_QUADS);
	CVector3f norm = CalcNormal(v2, v3, v0);

	//0, -1, 0
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v1);

	//Back Side

	norm = CalcNormal(v5, v1, v0);
	//expeced norm: -x
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v5);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v4);

	//Right back side

	norm = CalcNormal(v6, v2, v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v1);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v6);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v5);

	//Front side
	norm = CalcNormal(v7, v3, v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v2);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v6);

	norm = CalcNormal(v4, v0, v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v3);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v0);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v4);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v7);

	//Top Side
	norm = CalcNormal(v5, v4, v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 0); glVertex3fv(v7);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 0); glVertex3fv(v4);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(1, 1); glVertex3fv(v5);
	glNormal3f(norm.x, norm.y, norm.z); glTexCoord2f(0, 1); glVertex3fv(v6);
	glEnd();

//	glPopMatrix();

	glEnable(GL_LIGHTING);
	
}