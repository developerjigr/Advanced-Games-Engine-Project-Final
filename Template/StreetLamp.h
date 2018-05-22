#pragma once
#include "GameObject.h"

class CStreetLamp : CGameObject
{
public:

	CStreetLamp();
	CStreetLamp(string meshLocation, CVector3f pos);
	~CStreetLamp();

	void Initialise();
	void Update(float dt);
	void Render();

	void TurnOn();
	void TurnOff();

	void SetLampLight(int index);

	void DebugLightPosition();

	int index = 0;

private:

	const float _SCALE = 2.5f;
	CVector3f lightPosition = CVector3f(mPosition.x,  10.0f, mPosition.z);

	bool lightsTurnedOn = true;


};