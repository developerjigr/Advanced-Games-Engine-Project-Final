#include "BouncingBall.h"

CBouncingBall::CBouncingBall()
{}

CBouncingBall::~CBouncingBall()
{}

bool CBouncingBall::Initialise(CVector3f position, CVector3f velocity, CVector3f acceleration, float coefficientOfResitution, float radius)
{
	m_position = position;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_coefficientOfRestitution = coefficientOfResitution;
	m_radius = radius;
	m_isActive = true;
	return true;

}

bool CBouncingBall::CollisionDetection(float yPlane)
{
	if (m_position.y - m_radius < yPlane && m_velocity.y < 0) {
		return true;
	}
	return false;
}

void CBouncingBall::CollisionResponse()
{
	float convergenceThreshold = 0.5f;
	if (m_velocity.Length() > convergenceThreshold)
		m_velocity = CVector3f(m_velocity.x, -m_velocity.y * m_coefficientOfRestitution, m_velocity.z);
	else {
		m_velocity = CVector3f(0, 0, 0);
		m_acceleration = CVector3f(0, 0, 0);
		m_position = CVector3f(m_position.x, m_radius, m_position.z);
		m_isActive = false;
	}
}

bool CBouncingBall::UpdatePhysics(float dt)
{
	if (m_isActive == false)
		return false;

	bool bounce = false;

	m_position += m_velocity * dt + (m_acceleration * dt * dt) * 0.5;
	m_velocity += m_acceleration * dt;
	
	float yPlane = 0.0f;
	if(CollisionDetection(yPlane)) {
		CollisionResponse();
		bounce = true;
	}

	return bounce;

}


void CBouncingBall::Render()
{

	glColor3f(1, 0, 0);
	glPushMatrix();
		glTranslatef(m_position.x, m_position.y, m_position.z);
		glutSolidSphere(m_radius, 15, 15);
	glPopMatrix();

}
