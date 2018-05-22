#pragma once
#include <windows.h>									

#include ".\include\gl.h"								
#include "Vector3f.h"
#include "Texture.h"

class CShockwave
{
public:
	CShockwave();
	~CShockwave();
	bool Initialise();
	void Activate(float radius, CVector3f position);
	void Render();
	void Update(float dt);
	bool IsActive();
	
private:
	CTexture m_texture;
	bool m_isActive;
	float m_radius;
	float m_life;
	CVector3f m_position;

};