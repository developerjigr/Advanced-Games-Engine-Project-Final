#pragma once

#include ".\include\glut.h"	
#include "Vector3f.h"

class CLighting
{
public:

	void SetGlobalLighting(); //Create Lighting Effects
	void InitialiseLighting();

	void FlashLight();

	//std::vector<COpenAssetImporterMesh*> pStreetLamp;
	// Can create other lighting methods here

private:

	//const GLfloat off[] = { 0.0f, 0.0f, 0.0f, 0.0f };



};

