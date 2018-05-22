#include "Lighting.h"	
#include <windows.h>



//Directional lighting which should not be changed and disabled.
//Objects can only be illuminated using 8 light sources at onces 
void CLighting::SetGlobalLighting()
{
	glEnable(GL_LIGHTING);
	
	GLfloat ambientLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat positionLight0[] = { 0.0f, 1.0f, 1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);

	glDisable(GL_LIGHT0);
	GLfloat glob_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glob_ambient);

	glEnable(GL_NORMALIZE);

}

void CLighting::InitialiseLighting()
{



}


void CLighting::FlashLight()
{

	glEnable(GL_LIGHTING);

	GLfloat ambientLight1[] = { 1, 1, 1, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat positionLight1[] = { 0,30, 1, 0 };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT4, GL_POSITION, positionLight1);

	glEnable(GL_LIGHT4);
	glEnable(GL_NORMALIZE);

	glDisable(GL_LIGHT4);

	//SetGlobalLighting();

}