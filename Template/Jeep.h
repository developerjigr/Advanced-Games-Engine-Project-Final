#pragma once
#include "GameObject.h"

class CJeep : public CGameObject
{
public:

	CJeep(){}
	~CJeep() {}

	void Initialise() {}
	void Initialise(CVector3f pos, string meshName, float w, float h, float d);
	void Update(float dt);
	void Render();

//mPhysics.Ini

private:
	int rotation = 180;


};