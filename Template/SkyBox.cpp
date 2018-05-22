#include "SkyBox.h"
#include "Texture.h"

CSkybox::CSkybox()
{}

CSkybox::~CSkybox()
{}

bool CSkybox::Create(const char *bmpDirectory, const float fSize, Camera& cam)
{
	// Load textures
	CTexture texture;	

	texture.Load("Resources\\Textures\\back.jpg", true);
	m_textureIDs[SKYBOX_BACK_ID] = texture.m_textureID;

	texture.Load("Resources\\Textures\\front.jpg", true);
	m_textureIDs[SKYBOX_FRONT_ID] = texture.m_textureID;

	texture.Load("Resources\\Textures\\bottom.jpg", true);
	m_textureIDs[SKYBOX_BOTTOM_ID] = texture.m_textureID;

	texture.Load("Resources\\Textures\\top.jpg", true);
	m_textureIDs[SKYBOX_TOP_ID] = texture.m_textureID;

	texture.Load("Resources\\Textures\\left.jpg", true);
	m_textureIDs[SKYBOX_LEFT_ID] = texture.m_textureID;

	texture.Load("Resources\\Textures\\right.jpg", true);
	m_textureIDs[SKYBOX_RIGHT_ID] = texture.m_textureID;

	mCamera = &cam;

	return true;
}



void CSkybox::Render()
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	CVector3f vPos = mCamera->GetPosition();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	DrawSkybox(vPos.x, vPos.y, vPos.z, 250, 500, 250);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void CSkybox::DrawSkybox(float x, float y, float z, float width, float height, float length)
{
	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	// Bind the BACK texture of the sky map to the BACK side of the cube
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_BACK_ID]);

	// This centers the sky box around (x, y, z)
	x = x - width / 2;
	y = (y - height / 2) + 5;
	z = z - length / 2;

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////BACK SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

	// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();

	// Bind the FRONT texture of the sky map to the FRONT side of the box
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_FRONT_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////FRONT SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_BOTTOM_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////BOTTOM SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

	// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();

	// Bind the TOP texture of the sky map to the TOP side of the box
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_TOP_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////TOP SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

	// Assign the texture coordinates and vertices for the TOP Side

	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);


	glEnd();

	// Bind the LEFT texture of the sky map to the LEFT side of the box
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_LEFT_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////LEFT SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

	// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();

	// Bind the RIGHT texture of the sky map to the RIGHT side of the box
	glBindTexture(GL_TEXTURE_2D, m_textureIDs[SKYBOX_RIGHT_ID]);

	glNormal3f(0.0f, 0.0f, 1.0f);///////////////////////////////////////////////////////////////////////////////////////////////RIGHT SIDE
								 // Start drawing the side as a QUAD
	glBegin(GL_QUADS);

	// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glDisable(GL_TEXTURE_2D);


	// Render the skybox with lighting off
}
