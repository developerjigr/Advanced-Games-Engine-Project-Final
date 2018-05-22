#pragma once
#include "Vector3f.h"
#include "Lighting.h"
#include "HighResolutionTimer.h"

class CLightningBolt
{

public:

	CLightningBolt() {}
	~CLightningBolt() {}

	CLighting mLighting;

	void Initialise(CVector3f pos)
	{
		mPosition = pos;
	}

	void Update();
	void Render();


	CHighResolutionTimer mLightingTimer;


private:


	CVector3f mPosition = CVector3f(0, 0, 0);

};