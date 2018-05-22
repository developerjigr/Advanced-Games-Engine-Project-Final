#include "InputManager.h"


CInputManager* CInputManager::_instance = 0;

CInputManager* CInputManager::Instance() {
	if (_instance == 0) {
		_instance = new CInputManager;
	}
	return _instance;
}


void CInputManager::SetMouseScreenToGame(HWND& hwnd)
{
	GLdouble mMatrix[16], pMatrix[16], x, y, z;
	GLint vp[4];
	float d;
	POINT p2d;
	// Get the position of mouse in window coordinates
	GetCursorPos(&p2d);
	ScreenToClient(hwnd, &p2d); // hwnd comes from GameWindow::GetHwnd()
								// Get the modelview, projection, and viewport matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
	glGetIntegerv(GL_VIEWPORT, vp);
	// Read from the depth buffer and unproject to get 3d point at x, y, z
	glReadPixels((int)p2d.x, vp[3] - (int)p2d.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &d);
	gluUnProject((double)p2d.x, vp[3] - (double)p2d.y, d, mMatrix, pMatrix, vp, &x, &y, &z);
}


