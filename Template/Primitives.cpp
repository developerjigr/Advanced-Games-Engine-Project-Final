#include "Primitives.h"

#define PI 3.14159265

CPrimitives::CPrimitives() {}
CPrimitives::~CPrimitives() {}

void CPrimitives::DrawCircle(int EDGES, float scale, float r, float g, float b)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);


	glPushMatrix();

	glLineWidth(6.5f);
	//Rotate around Z-Axis
	glTranslatef(5, 10, 5);
	glRotatef(-90, 1, 0, 0);
	glScalef(scale, scale, scale);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= EDGES; i++)
	{
		float x = cos((2 * PI *i) / EDGES);
		float y = sin((2 * PI*i) / EDGES);

		glVertex3f(x, y, 0);
		
	}
	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CPrimitives::DrawCube(float x, float y, float d, float scale)
{

	GLfloat 
		v0[3] = { -x, 0.0f, -d },
		v1[3] = { -x, 0.0f, +d },
		v2[3] = { +x, 0.0f, +d },
		v3[3] = { +x, 0.0f, -d },

		v4[3] = { -x, +y, -d },
		v5[3] = { -x, +y, +d },
		v6[3] = { +x, +y, +d },
		v7[3] = { +x, +y, -d };

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glPushMatrix();

	glScalef(scale, scale, scale);

	glBegin(GL_QUADS);

	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);

	glVertex3fv(v0);
	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v1);

	glVertex3fv(v1);
	glVertex3fv(v5);
	glVertex3fv(v6);
	glVertex3fv(v2);

	glVertex3fv(v2);
	glVertex3fv(v6);
	glVertex3fv(v7);
	glVertex3fv(v3);

	glVertex3fv(v3);
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v0);

	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);
	glVertex3fv(v4);
	glEnd();


	glPopMatrix();


	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);



}