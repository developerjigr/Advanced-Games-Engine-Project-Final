#include "Terrain.h"
#include "Texture.h"
#include <vector>


CTerrain::CTerrain()
{}

CTerrain::~CTerrain()
{}

bool CTerrain::Initialise()
{
	// Load the texture
	CTexture GrassTexture, RockTexture, RoadTexture;
	RockTexture.Load("Resources\\Textures\\Terrain.bmp", GL_REPEAT);
	m_RockTextureID = RockTexture.m_textureID;

	GrassTexture.Load("Resources\\Textures\\Terrain2.bmp", GL_REPEAT);
	m_GrassTextureID = GrassTexture.m_textureID;

	RoadTexture.Load("Resources\\Textures\\road_floor.jpg", GL_REPEAT);
	m_RoadTextureID = RoadTexture.m_textureID;

	return true;
}

void CTerrain::Render()
{
	//Size of the rendered floor.
	float TERRAIN_SIZE = 500;
	
	GLfloat x, y, z, length, width;
	length = TERRAIN_SIZE / 2.0f;
	width = TERRAIN_SIZE / 2.0f;
	
	// Centre terrain around 0, 0, 0;
	x = -width  / 2; //-125
	y = 0;
	z = -length / 2; //-125

	//// Bind the texture	
	GLfloat TILE_SIZE = 10;
	GLint X_GRID_TILE_AMOUNT = 5;
	GLint Z_GRID_TILE_AMOUNT = 5;
	GLfloat X_TILE_SIZE = width / 2 / X_GRID_TILE_AMOUNT;
	GLfloat Z_TILE_SIZE = length / 2 / Z_GRID_TILE_AMOUNT;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_RockTextureID);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLfloat fLargest = 10;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	// Draw a first quad
	
	glEnable(GL_LIGHTING);
	glClearColor(1, 1, 1, 1);
	//glDisable(GL_COLOR_MATERIAL);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f);				glNormal3f(0, 1, 0);		glVertex3f(x, y, z); // (-125, 0, -125)
		glTexCoord2f(0.0f, TILE_SIZE); 			glNormal3f(0, 1, 0);		glVertex3f(x, y, z + length); // (-125, 0, 125)
		glTexCoord2f(TILE_SIZE, TILE_SIZE);		glNormal3f(0, 1, 0);		glVertex3f(x + width, y, z + length); // (125, 0, 125)
		glTexCoord2f(TILE_SIZE, 0.0f);			glNormal3f(0, 1, 0);		glVertex3f(x + width, y,			z); // (125, 0, -125)
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_GrassTextureID);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_REPEAT);

	fLargest = 10;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	// Draw a first quad

	glEnable(GL_LIGHTING);
	glClearColor(1, 1, 1, 1);
	//glDisable(GL_COLOR_MATERIAL);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);				glNormal3f(0, 1, 0);		glVertex3f(x, y+0.0075, z); // (-125, 0, -125)
	glTexCoord2f(0.0f, TILE_SIZE); 			glNormal3f(0, 1, 0);		glVertex3f(x, y+0.0075, z + length); // (-125, 0, 125)
	glTexCoord2f(TILE_SIZE, TILE_SIZE);		glNormal3f(0, 1, 0);		glVertex3f(x + width, y+0.0075, z + length); // (125, 0, 125)
	glTexCoord2f(TILE_SIZE, 0.0f);			glNormal3f(0, 1, 0);		glVertex3f(x + width, y+0.0075, z); // (125, 0, -125)
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	///DRAW THE ROAD FLOOR
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_RoadTextureID);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_REPEAT);


	for (int j = z; j < (length / 2) - Z_TILE_SIZE; j += Z_GRID_TILE_AMOUNT) 
	{
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(0, y+0.01, j); // (-125, 0, -125)
					glTexCoord2f(0.0f, TILE_SIZE); glVertex3f(0, y+0.01, j + Z_TILE_SIZE); // (-125, 0, 125)
					glTexCoord2f(TILE_SIZE, TILE_SIZE); glVertex3f(0 + X_TILE_SIZE, y+ 0.01, j + Z_TILE_SIZE); // (125, 0, 125)
					glTexCoord2f(TILE_SIZE, 0.0f); glVertex3f(0 + X_TILE_SIZE, y + 0.01, j); // (125, 0, -125)
				glEnd();
	}


	glEnable(GL_NORMALIZE);
	
}
