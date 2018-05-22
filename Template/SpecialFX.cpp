#include "SpecialFX.h"
#include <windows.h>									// Header File For The Windows Library
#include ".\include\gl.h"								// Header File For The OpenGL32 Library


CSpecialFX::CSpecialFX()
{}

CSpecialFX::~CSpecialFX()
{}

void CSpecialFX::ActivateFog(float density, float r, float g, float b)
{
	GLfloat fogColor[4] = {r, g, b, 1.0};

	glEnable (GL_FOG);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, density);
	glHint (GL_FOG_HINT, GL_NICEST);
}

void CSpecialFX::DeactivateFog()
{
	glDisable (GL_FOG);

}
