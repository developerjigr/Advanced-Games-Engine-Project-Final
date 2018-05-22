#pragma once
#include <windows.h>	
#include "Camera.h"// Header File For The Windows Library
#include ".\include\gl.h"								// Header File For The OpenGL32 Library


typedef enum {
	SKYBOX_BACK_ID,
	SKYBOX_FRONT_ID,
	SKYBOX_BOTTOM_ID,
	SKYBOX_TOP_ID,
	SKYBOX_LEFT_ID,
	SKYBOX_RIGHT_ID
} BoxSideTypes;

class CSkybox
{
public:
	CSkybox();
	~CSkybox();
	bool Create(const char *bmpDirectory, const float fSize, Camera& cam);

	void Render();

	void DrawSkybox(float x, float y, float z, float width, float height, float length);

private:
	
	unsigned int m_textureIDs[6];
	//Camera pointer 
	Camera* mCamera;
	

};