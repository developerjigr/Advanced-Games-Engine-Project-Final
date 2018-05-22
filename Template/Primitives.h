#pragma once
#include "Texture.h"
#include  "./include/glut.h"
#include  "./include/gl.h"
#include "Vector3f.h"
#include <windows.h>
#include <math.h>

#define _USE_MATHS_DEFINES

class CPrimitives
{

public:

	CPrimitives();
	~CPrimitives();
	
	void DrawCube(float width, float height, float depth, float scale);
	void DrawCircle(int edges, float scale, float red, float green, float blue);
	void DrawStar(float scale, CVector3f rotation);

private:


};