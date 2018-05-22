#include "Material.h"	



void CMaterial::SetDefaultMaterial()
{
	GLfloat ambientMaterial[] = {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat diffuseMaterial[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specularMaterial[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);
}



void CMaterial::SetRedMaterial()
{
	GLfloat ambientMaterial[] = {0.2f, 0.0f, 0.0f, 1.0f};
	GLfloat diffuseMaterial[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 55.0f);

}