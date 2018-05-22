#pragma once
#include ".\include\GLew.H"
#include ".\include\GL.H"
#include ".\include\GLU.H"
#include ".\include\glut.H"
#include "Vector3f.h"
#include "GameWindow.h"



class CInputManager
{

public:
	static CInputManager* Instance();
	
	void SetMouseScreenToGame(HWND& hwnd);
	CVector3f GetMouseToGamePos() { return mousePosInGame; }

protected:
	CInputManager() {}
		~CInputManager() {}

	private:
		static CInputManager* _instance;
		CVector3f mousePosInGame;

	


};