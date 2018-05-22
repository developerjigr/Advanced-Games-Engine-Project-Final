#include "StreetLamp.h"


CStreetLamp::CStreetLamp() {}

CStreetLamp::CStreetLamp(string meshLoc, CVector3f pos)
{
	mMesh = COpenAssetImporterMesh();
	mMesh.Load(meshLoc);
	mPosition = pos;
	Initialise();
	//mMes = 
}

CStreetLamp::~CStreetLamp() {}

void CStreetLamp::Initialise() 
{
	Render();
}

void CStreetLamp::Update(float dt) 
{


}

void CStreetLamp::SetLampLight(int ind)
{


	//lightPosition = CVector3f(mPosition.x, 10.0f, mPosition.z);

	//glPushMatrix();

	//GLfloat ambientLight1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat positionLight1[] = { lightPosition.x, lightPosition.y, lightPosition.z, 1.0f };
	//GLfloat spotPos[] = { 0.0f, -1.0f, 0.0f };


	//switch (ind)
	//{
	//
	//case 1: 
	//	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	//	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	//	//glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	//	//glLightfv(GL_LIGHT1, GL_POSITION, positionLight1);
	//
	//	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	//	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5.0);
	//	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotPos);

	//	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.25);
	//	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);


	//	//glEnable(GL_LIGHT1);
	//	//glEnable(GL_NORMALIZE);

	//	break;
	//case 2:
	//	//glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight1);
	//	//glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight1);
	//	//glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight1);
	//	//glLightfv(GL_LIGHT2, GL_POSITION, positionLight1);
	//	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
	//	//glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 128.0);
	//	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotPos);

	//	//glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.01);
	//	//glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.01);
	//	//glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
	//	////glEnable(index);
	//	//glEnable(GL_LIGHT2);

	//break;
	////case 3:
	////	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight1);
	////	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight1);
	////	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight1);
	////	glLightfv(GL_LIGHT3, GL_POSITION, positionLight1);
	////	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.5);
	////	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
	////	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2);

	////	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	////	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 128.0);
	////	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotPos);
	////	
	////	glEnable(GL_LIGHT3);

	////	break;
	//case 4:
	//	//glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight1);
	//	//glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight1);
	//	//glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight1);
	//	//glLightfv(GL_LIGHT4, GL_POSITION, positionLight1);

	//	//glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 180.0);
	//	//glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 128);
	//	//glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spotPos);
	//	//glDisable(GL_LIGHT4);
	////	
	//	break;
	////case 5:
	////	glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight1);
	////	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight1);
	////	glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight1);
	////	glLightfv(GL_LIGHT5, GL_POSITION, positionLight1);

	////	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
	////	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 1.0);
	////	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spotPos);
	////	glEnable(GL_LIGHT5);
	////	glEnable(GL_NORMALIZE);
	////	break;
	////case 6:
	////	glLightfv(GL_LIGHT6, GL_AMBIENT, ambientLight1);
	////	glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuseLight1);
	////	glLightfv(GL_LIGHT6, GL_SPECULAR, specularLight1);
	////	glLightfv(GL_LIGHT6, GL_POSITION, positionLight1);

	////	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
	////	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 1.0);
	////	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spotPos);
	////	
	////	glEnable(GL_LIGHT6);

	//	break;

	//default:

	//	break;
	//}

	//glEnable(GL_NORMALIZE);
	//glPopMatrix();


}

void CStreetLamp::Render() 
{
	//TRO

//	glDisable(GL_TEXTURE_2D);

	lightPosition = CVector3f(mPosition.x, 10.0f, mPosition.z);
	//DebugLightPosition();

	glPushMatrix();
		
		SetLampLight(index);
		glTranslatef(mPosition.x, mPosition.y, mPosition.z);
		glScalef(_SCALE, _SCALE, _SCALE);
		mMesh.Render();
	glPopMatrix();

	//

//	glEnable(GL_TEXTURE_2D);
}

void CStreetLamp::DebugLightPosition()
{

	glPushMatrix();
		glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
		glutSolidSphere(0.5f, 25, 25);
	glPopMatrix();



	//glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);


}




