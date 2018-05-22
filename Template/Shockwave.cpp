#include "Shockwave.h"


CShockwave::CShockwave()
{
	m_isActive = false;
	m_radius = 5.0f;
}

CShockwave::~CShockwave()
{}

bool CShockwave::IsActive()
{
	return m_isActive;
}

void CShockwave::Update(float dt)
{
	if (m_isActive == false)
		return;

	// Life decreasing on each iteration
	m_life -= 0.5f * dt;
	if (m_life < 0) {
		m_isActive = false;
		return;
	}

	float f = pow(m_life, 0.25f); // Nonlinear mapping -- makes it expand faster at first
	m_radius += 15.0f*f*dt;


}

bool CShockwave::Initialise()
{
	// Load the texture
	return m_texture.Load("Resources\\Textures\\LightingRing.png", false); 
}

void CShockwave::Activate(float radius, CVector3f position)
{
	m_position = position;
	m_life = 1.0f;
	m_radius = radius;
	m_isActive = true;

}

void CShockwave::Render()
{

	if (m_isActive == false)
		return;
	
	glDisable(GL_CULL_FACE);
		

	CVector3f xAxis = CVector3f(1, 0, 0);
	CVector3f zAxis = CVector3f(0, 0, 1);

	// Render a texture mapped quad -- find four points
	CVector3f p1 = m_position - m_radius*xAxis - m_radius*zAxis;
	CVector3f p2 = m_position - m_radius*xAxis + m_radius*zAxis;
	CVector3f p3 = m_position + m_radius*xAxis + m_radius*zAxis;
	CVector3f p4 = m_position + m_radius*xAxis - m_radius*zAxis;
	
	// Bind the texture	
	m_texture.Bind();

	// Assume oriented in xz plane
	glNormal3f(0, 1, 0);

	glPushMatrix();
	glEnable(GL_BLEND);									
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	// Set transparency based on life
	glColor4f(1, 1, 1, m_life);

	// Draw a quad
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(p3.x, p3.y, p3.z);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(p4.x, p4.y, p4.z);
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	glPopMatrix();

}
