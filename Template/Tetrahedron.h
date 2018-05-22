#include "Texture.h"
#include "Vector3f.h"
#pragma once
class CTetrahedron
{
public:
	CTetrahedron();
	~CTetrahedron();
	void Initialise();
	void Update(float dt);
	void Render();
	void ChangePosition(CVector3f newPos);


	GLfloat points;

	int mRandomNoisePoints = 4;


private:
	UINT mTextureID;
	CTexture mTexture;

	float mTheta = 0.0f;

	CVector3f mPosition;



};