#include "camera.h"
#include "Player.h"


// Set up a default camera
Camera::Camera()
{
	m_position = CVector3f(10.0f, 2.0f, 30.0f);
	m_viewPoint = CVector3f(0.0f, 2.0f, 5.0f);
	m_upVector = CVector3f(0.0f, 1.0f, 0.0f); 
	m_speed = 5.0f;
	m_totalVerticalRotation = 0.0f;
	m_totalHorizontalRotation = 0.0f;

	eCurrentCameraState = FOLLOWING_GAME_OBJECT;

}

Camera::~Camera() {}

// Set a specific camera
void Camera::Set(const CVector3f &position, const CVector3f &view, const CVector3f &up_vector, const float &speed)
{
	m_position = position;
	m_viewPoint = view;
	m_upVector = up_vector;
	m_speed = speed;

	Look();
}



// Update the camera -- note this typically involves updating the camera position (m_vPosition), viewpoint (m_vViewPoint), and stafe vector (m_vStrafeVector)
void Camera::Update(double dt)
{

	//mDirection = GetViewPoint() - GetPosition();
	

	switch (eCurrentCameraState) {

	case FOLLOWING_GAME_OBJECT:

		//Movement(dt);
		if (mAttatchedGameObject != NULL)
			FollowGameObject();

		mTempPosition = m_position;
		mTempView = m_viewPoint;

		break;

	case FREE_MODE:

		Movement(dt);
		if (GetFocus())
			SetViewByMouse();

		break;

	case TRANSITION_FROM_GAMEOBJECT:
			TransitionCameraFromGameObject(dt);
		break;

	case TRANSITION_TO_GAMEOBJECT:
			TransitionCameraToGameObject(dt);
		break;

	case TOP_DOWN_MODE:
		TopDownView(dt);
		break;

	default:
		break;
		SwitchCameraPos(dt);
		//m_position.y = mYPos;


	}
}

void Camera::TopDownView(float dt)
{
	//glPushMatrix();
	//	glRotatef(-45, )

	LookDown();
	Movement(dt);
	SetViewByMouse();
	m_position.y = MAX_LOOKDOWN_HEIGHT;
	//ADD LOOK DOWN INFO

}


//RENDERING FOR DEBUG CAMERA PATHS
void Camera::Render()
{

}


void Camera::FollowGameObject()
{
	
	float mYPos = m_position.y;
	m_position.y = mYPos;

	if (GetFocus())
		SetViewByMouse();


}

void Camera::SwitchStates(eCameraStates state)
{
	switch (state) {

	case FOLLOWING_GAME_OBJECT:
		//if (eCurrentCameraState != TRANSITION_TO_GAMEOBJECT || eCurrentCameraState != FREE_MODE)
		//	return;

		eCurrentCameraState = FOLLOWING_GAME_OBJECT;
		break;

	case FREE_MODE:
		eCurrentCameraState = FREE_MODE;
		break;

	case TRANSITION_FROM_GAMEOBJECT:
		if (eCurrentCameraState != FOLLOWING_GAME_OBJECT)
			return;

		eCurrentCameraState = TRANSITION_FROM_GAMEOBJECT;
		break;

	case TRANSITION_TO_GAMEOBJECT:
		//if (eCurrentCameraState != FREE_MODE || eCurrentCameraState != TOP_DOWN_MODE)
		//	return;

		eCurrentCameraState = TRANSITION_TO_GAMEOBJECT;
		break;

	case TOP_DOWN_MODE:
		eCurrentCameraState = TOP_DOWN_MODE;
		break;

	default:
		break;


	}

}

void Camera::LookDown()
{
	//Lights on lights offff ---> keep the lights on pls.

	GLfloat ambientLight1[] = { 1, 1, 1, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat positionLight1[] = { 0,30, 1, 0 };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT4, GL_POSITION, positionLight1);

	glEnable(GL_LIGHT4);

	m_position = CVector3f(GetPosition().x, MAX_LOOKDOWN_HEIGHT, GetPosition().z);
	m_viewPoint = CVector3f(GetPosition().x, 0.0f, GetPosition().z);
	Set(m_position, CVector3f(GetPosition().x, -10.0f, GetPosition().z), CVector3f(0, 0, 1), 25.0f);
}

void Camera::TransitionCameraFromGameObject(float dt)
{

	//if following game object we want it to stop and follow the new path.
	if (FollowingGameObject)
		FollowingGameObject = false;

	if (dist >= 1)
		dist = 0;

	float speed = dt * 0.45f;

	mAttatchedGameObject->SetActive(false);

		//Reset if its already been run
		if (dist < 1) {
			vPoint = ReturnPointOnCurve(g_vStartPoint, g_vControlPoint1, g_vControlPoint2, g_vEndPoint, dist);
			CVector3f newPosition = CVector3f(m_position.x + vPoint.x,   vPoint.y, m_position.z + vPoint.z);
			Set(newPosition, m_viewPoint, CVector3f(0, 1, 0), 15.0f);
			dist += speed;
		}

		if (dist >= 1) {
			cameraMovement = false;
			SwitchStates(TOP_DOWN_MODE);
		}

		//LOOK DOWN CODE TO FREEVIEW


		//FollowingGameObject = true;

}

void Camera::TransitionCameraToGameObject(float dt)
{

	//if (!cameraMovement)
	//	return;

	////if following game object we want it to stop and follow the new path.
	//if (FollowingGameObject)
	//	FollowingGameObject = false;

	if (dist >= 1)
		dist = 0;

	float speed = dt * 0.45f;

	//Reset if its already been ru

	if (dist < 1) 
	{
		vPoint = ReturnPointOnCurve(CVector3f(m_position.x, MAX_LOOKDOWN_HEIGHT, m_position.z), g_vControlPoint1, g_vControlPoint2, mTempPosition - g_vStartPoint, dist);
		CVector3f newPosition = CVector3f(vPoint.x,  vPoint.y, vPoint.z);
		Set(newPosition, mTempView, CVector3f(0, 1, 0), 15.0f);
		dist += speed;
	}

	if (dist >= 1) 
	{
		Set(mTempPosition, mTempView, CVector3f(0, 1, 0), 25.0f);
		SwitchStates(FOLLOWING_GAME_OBJECT);
		mAttatchedGameObject->SetActive(true);
	}

	//LOOK DOWN CODE TO FREEVIEW


	//FollowingGameObject = true;



}




void Camera::Follow(CGameObject& gameObj, float camBackDist, float camLookDist, float camHeight)
{
	mAttatchedGameObject = &gameObj;
	mCameraBackDistance = camBackDist;
	mCameraLookDistance = camLookDist;
	mCameraHeight = camHeight;

	FollowingGameObject = true;
	CanFollowGameObject = true;
}

// Get the camera rotation using the mouse
void Camera::SetViewByMouse()
{
	int middle_x = GameWindow::SCREEN_WIDTH >> 1;
	int middle_y = GameWindow::SCREEN_HEIGHT >> 1;


	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	SetCursorPos(middle_x, middle_y);

	float dx = (float)(middle_x - mouse.x) / 50.0f;
	float dy = (float)(middle_y - mouse.y) / 50.0f;

	m_totalHorizontalRotation += dx;
	m_totalVerticalRotation += dy;


	// Clip the rotation rotation about the strafe vector to [-MAX_STRAFE_ROTATION, MAX_STRAFE_ROTATION]
	if (m_totalVerticalRotation > MAX_VERTICAL_ROTATION) {
		m_totalVerticalRotation = MAX_VERTICAL_ROTATION;
	}
	else if (m_totalVerticalRotation < -MAX_VERTICAL_ROTATION) {
		m_totalVerticalRotation = -MAX_VERTICAL_ROTATION;
	}
	else {
		RotateView(dy, m_strafeVector);
	}

	//if (m_totalHorizontalRotation > MAX_HORIZONTAL_ROTATION) {
	//	m_totalHorizontalRotation = MAX_HORIZONTAL_ROTATION;
	//}
	//else if (m_totalHorizontalRotation < -MAX_HORIZONTAL_ROTATION) {
	//	m_totalHorizontalRotation = -MAX_HORIZONTAL_ROTATION;
	//}
	//else {
	//	RotateView(dx, CVector3f(0, 1, 0));
	//}
	RotateView(dx, CVector3f(0, 1, 0));
}

// Rotate the camera viewpoint around a vector
void Camera::RotateView(float angle, CVector3f &vector)
{
	CVector3f view = m_viewPoint - m_position;
	CVector3f rotatedView = view.RotateAboutAxis(angle, vector);
	m_viewPoint = m_position + rotatedView;
}
// Move the camera left/right
void Camera::Strafe(float speed)
{
	if (eCurrentCameraState == FREE_MODE) {
		m_position.x += m_strafeVector.x * speed;
		m_position.z += m_strafeVector.z * speed;

		m_viewPoint.x += m_strafeVector.x * speed;
		m_viewPoint.z += m_strafeVector.z * speed;

	}

	if (eCurrentCameraState == TOP_DOWN_MODE)
	{
		m_position.x += m_strafeVector.x * speed;
		m_position.y += m_strafeVector.y * speed;

		m_viewPoint.x += m_strafeVector.x * speed;
		m_viewPoint.y += m_strafeVector.y * speed;

	}
}

void Camera::SmallCameraShake(bool shakeOn)
{
	if (!shakeOn)
	{
		if (randomX != 0) {
			
			//m_position = CVector3f(m_position.x + (m_position.x - mTempShakeView.x), mTempPosition.y, m_position.z + (m_position.z - mTempShakeView.z));
			mTempShakeView = CVector3f(0.0f, m_position.y, 0.0f);
			randomX = 0;
			randomY = 0;
			
		}
	}
	else {

		if (mTempShakeView == CVector3f(0.0f, m_position.y, 0.0f))
			mTempShakeView = m_position;

		randomX = rand() % 100 / 60;
		randomY = rand() % 100 / 60;
		
		int negate = (randomX > 0.5f) ? -1 : 1;
		//m_position = CVector3f(m_position.x + randomX * negate, m_position.y + randomY * negate, m_position.z + randomX * negate);
		m_viewPoint.RotateAboutAxis(randomX * negate, CVector3f(0, 1, 0));
		m_viewPoint.RotateAboutAxis(randomY * negate, CVector3f(1, 0, 0));

		Set(m_position, m_viewPoint, CVector3f(0, 1, 0), 25);

	}
}

void Camera::MediumCameraShake(bool shakeOn)
{
	if (!shakeOn)
	{
		if (randomX != 0) {

			m_position = CVector3f(m_position.x + (m_position.x - mTempShakeView.x), mTempPosition.y, m_position.z + (m_position.z - mTempShakeView.z));
			mTempShakeView = CVector3f(0, 0, 0);
			randomX = 0;
			randomY = 0;

		}
	}
	else {

		if (mTempShakeView == CVector3f(0, 0, 0))
			mTempShakeView = m_position;

		randomX = rand() % 50 / 100;
		randomY = rand() % 50 / 100;
	}
}
void Camera::LargeCameraShake(bool shakeOn)
{
	if (!shakeOn)
	{
		randomX = 0;
		randomY = 0;
		m_viewPoint = mTempShakeView;
		mTempShakeView = CVector3f(0, 0, 0);
	}

	if(mTempShakeView == CVector3f(0,0,0))
		mTempShakeView = m_viewPoint;
	
	randomX = rand() % 50 / 5;
	randomY = rand() % 50 / 5;
}
// Move the camera forward/backward
void Camera::Advance(float speed)
{
	CVector3f view = m_viewPoint - m_position;
	view.Normalise();

	m_position += view * speed;
	m_viewPoint += view * speed;

}

// Move the camera up / down
void Camera::MoveUp(float speed)
{
	m_position += m_upVector * speed;
	m_viewPoint += m_upVector * speed;
}

void Camera::UpdateRail(double dt)
{
	m_position.z += m_speed * dt;
	m_viewPoint.z += m_position.z;
	MovementRail(dt);
	m_position.ClipX(-5, 5);
	m_position.ClipY(-5, 15);
	m_viewPoint.ClipX(-5, 5);
	m_viewPoint.ClipY(-5, 15);
}


// Call gluLookAt to put a viewing matrix on the modelview matrix stack
void Camera::Look()
{
	gluLookAt(
		m_position.x, m_position.y, m_position.z,
		m_viewPoint.x, m_viewPoint.y, m_viewPoint.z,
		m_upVector.x, m_upVector.y, m_upVector.z);

	m_strafeVector = (m_viewPoint - m_position) % m_upVector;
	m_strafeVector.Normalise();

}

void Camera::MovementRail(double dt)
{
	float speed = (float)dt * m_speed;

	if ((GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)) {
		MoveUp(speed);	// Forward
	}

	if ((GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)) {
		MoveUp(-speed);	// Backward
	}

	if ((GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)) {
		Strafe(-speed); // Left
	}

	if ((GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)) {
		Strafe(speed); // Right
	}
}

// Move the camera based on arrow keys or WSAD keys
void Camera::Movement(double dt)
{	
	float speed = (float) dt * m_speed;

	if(eCurrentCameraState == TOP_DOWN_MODE)
	{ 
		if ((GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)) {
			MoveUp(speed);	// Forward
		}

		if ((GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)) {
			MoveUp(-speed);	// Backward
		}

		if ((GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)) {
			Strafe(-speed); // Left
		}

		if ((GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)) {
			Strafe(speed); // Right
		}
	} 
	else
	{

		if ((GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)) {
			Advance(speed);	// Forward
		}

		if ((GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)) {
			Advance(-speed);	// Backward
		}

		if ((GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)) {
			Strafe(-speed); // Left
		}

		if ((GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)) {
			Strafe(speed); // Right
		}
	}
}

CVector3f Camera::GetPosition() const
{
	return m_position;
}

CVector3f Camera::GetViewPoint() const
{
	return m_viewPoint;
}

CVector3f Camera::GetUpVector() const
{
	return m_upVector;
}

CVector3f Camera::GetStrafeVector() const
{
	return m_strafeVector;
}


void Camera::SwitchCameraPos(float dt)
{
	
	if (dist >= 1)
		dist = 0;

	float speed = dt * 0.05f;
	
	CVector3f v = ReturnPointOnCurve(CVector3f(m_position.x, MAX_LOOKDOWN_HEIGHT, m_position.z), g_vControlPoint1, g_vControlPoint2, mAttatchedGameObject->GetPosition() - g_vStartPoint, dist);
	glVertex3f(v.x, v.y, v.z);

	dist += speed;

	//m_position = vPoint;
		
}

CVector3f Camera::ReturnPointOnCurve(CVector3f p1, CVector3f p2, CVector3f p3, CVector3f p4, float t)
{
	float MAX_STEPS = 25;

	CVector3f startPoint =   p1;
	CVector3f firstCurve =   p2;
	CVector3f secondCurve =  p3;
	CVector3f endPoint =  p4;

	//Code clean up
	float var1, var2, var3;

		var1 = 1 - t;
		var2 = var1 * var1 * var1;
		var3 = t * t * t;

	//Equation
		CVector3f returnPoint;
		returnPoint.x = (startPoint.x * var2) + (firstCurve.x * var3 * var1 * var1) + (secondCurve.x * var3 * var3 * var1) + (endPoint.x * var3);
		returnPoint.y = (startPoint.y * var2) + (firstCurve.y * var3 * var1 * var1) + (secondCurve.y * var3 * var3 * var1) + (endPoint.y * var3);
		returnPoint.z = (startPoint.z * var2) + (firstCurve.z * var3 * var1 * var1) + (secondCurve.z * var3 * var3 * var1) + (endPoint.z * var3);

		return returnPoint;
}