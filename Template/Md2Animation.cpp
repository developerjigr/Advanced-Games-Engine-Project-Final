	//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		digiben@gametutorials.com			 //
//																		 //
//		$Program:		MD2 Animation									 //
//																		 //
//		$Description:	Demonstrates how to animate a Quake2 MD2 Model	 //
//																		 //
//***********************************************************************//


#include "Md2Animation.h"
#include "windows.h"
#include "Texture.h"
using namespace std;
 


///////////////////////////////// CLOAD MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This constructor initializes the md2 structures
/////
///////////////////////////////// CLOAD MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CLoadMD2::CLoadMD2()
{
	// Here we initialize our structures to 0
	memset(&m_Header, 0, sizeof(tMd2Header));

	// Set the pointers to null
	m_pSkins=NULL;
	m_pTexCoords=NULL;
	m_pTriangles=NULL;
	m_pFrames=NULL;
}


///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is called by the client to open the .Md2 file, read it, then clean up
/////
///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CLoadMD2::ImportMD2(t3DModel *pModel, char *strFileName, char *strTexture)
{
	char strMessage[255] = {0};

	// Open the MD2 file in binary
	fopen_s(&m_FilePointer, strFileName, "rb");

	// Make sure we have a valid file pointer (we found the file)
	if(!m_FilePointer) 
	{
		// Display an error message and don't load anything if no file was found
		sprintf_s(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	// Read the header data and store it in our m_Header member variable
	fread(&m_Header, 1, sizeof(tMd2Header), m_FilePointer);

	// Make sure the version is this crazy number '8' or else it's a bad egg
	if(m_Header.version != 8)
	{
		// Display an error message for bad file format, then stop loading
		sprintf_s(strMessage, "Invalid file format (Version not 8): %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// Read in the model and animation data
	ReadMD2Data();
	
	// Here we pass in our model structure to it can store the read Quake data
	// in our own model and object structure data
	ConvertDataStructures(pModel);

	// If there is a valid texture name passed in, we want to set the texture data
	if(strTexture)
	{
		// Create a local material info structure
		tMaterialInfo texture;

		// Copy the name of the file into our texture file name variable
		strcpy_s(texture.strFile, strTexture);

		// Since there is only one texture for a .Md2 file, the ID is always 0
		texture.texureId = 0;

		// The tile or scale for the UV's is 1 to 1 (but Quake saves off a 0-256 ratio)
		texture.uTile = texture.uTile = 1;

		// We only have 1 material for a model
		pModel->numOfMaterials = 1;

		// Add the local material info to our model's material list
		pModel->pMaterials.push_back(texture);
	}

	// Clean up after everything
	CleanUp();

	// Return a success
	return true;
}


///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in all of the model's data, except the animation frames
/////
///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ReadMD2Data()
{
	// Create a larger buffer for the frames of animation (not fully used yet)
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	// Here we allocate all of our memory from the header's information
	m_pSkins     = new tMd2Skin [m_Header.numSkins];
	m_pTexCoords = new tMd2TexCoord [m_Header.numTexCoords];
	m_pTriangles = new tMd2Face [m_Header.numTriangles];
	m_pFrames    = new tMd2Frame [m_Header.numFrames];

	// Next, we start reading in the data by seeking to our skin names offset
	fseek(m_FilePointer, m_Header.offsetSkins, SEEK_SET);
	
	// Depending on the skin count, we read in each skin for this model
	fread(m_pSkins, sizeof(tMd2Skin), m_Header.numSkins, m_FilePointer);
	
	// Move the file pointer to the position in the file for texture coordinates
	fseek(m_FilePointer, m_Header.offsetTexCoords, SEEK_SET);
	
	// Read in all the texture coordinates in one fell swoop
	fread(m_pTexCoords, sizeof(tMd2TexCoord), m_Header.numTexCoords, m_FilePointer);

	// Move the file pointer to the triangles/face data offset
	fseek(m_FilePointer, m_Header.offsetTriangles, SEEK_SET);
	
	// Read in the face data for each triangle (vertex and texCoord indices)
	fread(m_pTriangles, sizeof(tMd2Face), m_Header.numTriangles, m_FilePointer);
			
	// Move the file pointer to the vertices (frames)
	fseek(m_FilePointer, m_Header.offsetFrames, SEEK_SET);


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	// In the last tutorial we just read in one frame of animation here.  Now we are going
	// to extract every key frame from the .MD2 model.  These key frames will be used to
	// interpolate between each other to form the somewhat smooth animation.
	// The only thing different is that we are putting a for loop in front, then changing
	// the '0' for the m_pFrames[0] in the last tutorial, to a 'i'.  The loop will
	// continue until 'i' has reached the number of key frames for this model.

	for (int i=0; i < m_Header.numFrames; i++)
	{
		// Assign our alias frame to our buffer memory
		tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

		// Allocate the memory for the first frame of animation's vertices
		m_pFrames[i].pVertices = new tMd2Triangle [m_Header.numVertices];

		// Read in the first frame of animation
		fread(pFrame, 1, m_Header.frameSize, m_FilePointer);

		// Copy the name of the animation to our frames array
		strcpy_s(m_pFrames[i].strName, pFrame->name);
			
		// Store off a vertex array pointer to cut down large lines of code
		tMd2Triangle *pVertices = m_pFrames[i].pVertices;

		// Go through all of the number of vertices and assign the scale and translations.
		// Store the vertices in our current frame's vertex list array, while swapping Y and Z.
		// Notice we also negate the Z axis as well to make the swap correctly.
		for (int j=0; j < m_Header.numVertices; j++)
		{
			pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
			pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
			pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
		}
	}
}


///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function fills in the animation list for each animation by name and frame
/////
///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ParseAnimations(t3DModel *pModel)
{
	tAnimationInfo animation;
	string strLastName = "";

	// This function felt like a hack when I wrote it.  You aren't really given
	// any good information about the animations, other than the fact that each
	// key frame has a name assigned to it with a frame number for that animation.
	// For instance, the first animation is the "stand" animation.  The first frame
	// would have the name of: "stand01" or perhaps "stand1".  The 40th frame is
	// usually the last frame for the standing animation, so it would look like:
	// "stand40".  After this frame, the next animation is the running animation.
	// The next frame is labeled something like "run01".  You now know that the
	// standing animation is from frame 1 to frame 40 of the total frames.  The
	// start of the run animation is 41.  We will know how long the run animation
	// goes when we run into another animation name besides "run..".  That is how
	// I went about finding out the animation information.  I just grab each frame
	// name and check if it's the same animation name as the last name we found.
	// If it is, I just ignore it and continue to the next frame.  Once I find that
	// it's not, I then have the last frame saved off from the index 'i', and then
	// I create a new animation to add to the list, then start from the beginning.
	// It wasn't until later that I found on www.planetquake.com that there is a
	// standard frame count for each animation and they are all the same.  I figure
	// this way makes it modular so you don't have to stick to those standards anyway.

	// Go through all of the frames of animation and parse each animation
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		// Assign the name of this frame of animation to a string object
		string strName  = m_pFrames[i].strName;
		int frameNum = 0;
		
		// Go through and extract the frame numbers and erase them from the name
		for(unsigned int j = 0; j < strName.length(); j++)
		{
			// If the current index is a number and it's one of the last 2 characters of the name
			if( isdigit(strName[j]) && j >= strName.length() - 2)
			{
				// Use a C function to convert the character to a integer.
				// Notice we use the address to pass in the current character and on
				frameNum = atoi(&strName[j]);

				// Erase the frame number from the name so we extract the animation name
				strName.erase(j, strName.length() - j);
				break;
			}
		}

		// Check if this animation name is not the same as the last frame,
		// or if we are on the last frame of animation for this model
		if(strName.compare(strLastName) != 0 || i == pModel->numOfObjects - 1)
		{
			// If this animation frame is NOT the first frame
			if(strLastName.compare("") != 0)
			{
				// Copy the last animation name into our new animation's name
				strcpy_s(animation.strName, strLastName.c_str());

				// Set the last frame of this animation to i
				animation.endFrame = i;

				// Add the animation to our list and reset the animation object for next time
				pModel->pAnimations.push_back(animation);
				memset(&animation, 0, sizeof(tAnimationInfo));

				// Increase the number of animations for this model
				pModel->numOfAnimations++;
			}

			// Set the starting frame number to the current frame number we just found,
			// minus 1 (since 0 is the first frame) and add 'i'.
			animation.startFrame = frameNum - 1 + i;
		}

		// Store the current animation name in the strLastName string to check it latter
		strLastName = strName;
	}
}


///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function converts the .md2 structures to our own model and object structures
/////
///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ConvertDataStructures(t3DModel *pModel)
{
	int j = 0, i = 0;


	// Like the previous function, not a lot was changed her from the last tutorial.
	// Since we are dealing with multiple key frames, we need to pretty much do the 
	// same things for each frame.  This entails adding a for loop to go through
	// every frame.  We use index 'i' into the m_pFrames array to set each key frame.
	// This is one tricky thing in the middle of this for loop though, since we only
	// need to save the face and texture information once, this is only calculated
	// once and stored in the first frame of animation.  We can then reference the 
	// polygon information for every other frame from the first frame.  That way
	// we don't copy the same information for every key frame, which would make a huge
	// memory footprint.

	// Initialize the model structure first before assigning data to it
	memset(pModel, 0, sizeof(t3DModel));
	
	// Set the number of objects to our number of frames since pObjects will
	// hold the key frames
	pModel->numOfObjects = m_Header.numFrames;

	// Create our animation list and store it in our model
	ParseAnimations(pModel);

	// Go through every key frame and store it's vertices info in our pObject list.
	for (i=0; i < pModel->numOfObjects; i++)
	{
		// Create a local object to store the first frame of animation's data
		t3DObject currentFrame = {0};

		// Assign the vertex, texture coord and face count to our new structure
		currentFrame.numOfVerts   = m_Header.numVertices;
		currentFrame.numTexVertex = m_Header.numTexCoords;
		currentFrame.numOfFaces   = m_Header.numTriangles;

		// Allocate memory for the vertices, texture coordinates and face data.
		currentFrame.pVerts    = new CVector3 [currentFrame.numOfVerts];

		// Go through all of the vertices and assign them over to our structure
		for (j=0; j < currentFrame.numOfVerts; j++)
		{
			currentFrame.pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
			currentFrame.pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
			currentFrame.pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
		}

		// We can now free the old vertices stored in this frame of animation
		// since we have them stored in the pObject list of our model
		delete m_pFrames[i].pVertices;
	
		// Now comes the tricky part, since we only need to store the UV coordinates
		// and face indices once, we only do this on the first frame.  If the
		// current frame is past the first frame, we just add the current frame
		// as it is to our list, then continue to the next frame.  If we are
		// on the first frame, we still need to store the UV and face information.
		// This will only go in the first frame, and will be referenced from the
		// remaining frames when we animate the model.

		// Check if we are past the first key frame
		if(i > 0)
		{
			// Here we add the current object (or frame) to our list object list
			pModel->pObject.push_back(currentFrame);
			continue;	// Go on to the next key frame
		}
	
		// We will only get here ONE because we just need this information
		// calculated for the first key frame.
			
		// Allocate memory for our UV coordinates and face information
		currentFrame.pTexVerts = new CVector2 [currentFrame.numTexVertex];
		currentFrame.pFaces    = new tFace [currentFrame.numOfFaces];

		// Go through all of the uv coordinates and assign them over to our structure.
		// The UV coordinates are not normal uv coordinates, they have a pixel ratio of
		// 0 to 256.  We want it to be a 0 to 1 ratio, so we divide the u value by the
		// skin width and the v value by the skin height.  This gives us our 0 to 1 ratio.
		// For some reason also, the v coodinate is flipped upside down.  We just subtract
		// the v coordinate from 1 to remedy this problem.
		for(j=0; j < currentFrame.numTexVertex; j++)
		{
			currentFrame.pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
			currentFrame.pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
		}

		// Go through all of the face data and assign it over to OUR structure
		for(j=0; j < currentFrame.numOfFaces; j++)
		{
			// Assign the vertex indices to our face data
			currentFrame.pFaces[j].vertIndex[0] = m_pTriangles[j].vertexIndices[0];
			currentFrame.pFaces[j].vertIndex[1] = m_pTriangles[j].vertexIndices[1];
			currentFrame.pFaces[j].vertIndex[2] = m_pTriangles[j].vertexIndices[2];

			// Assign the texture coord indices to our face data
			currentFrame.pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
			currentFrame.pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
			currentFrame.pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
		}

		// Here we add the current object (or frame) to our list object list
		pModel->pObject.push_back(currentFrame);
	}
}

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function cleans up our allocated memory and closes the file
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::CleanUp()
{
	// This just just the regular cleanup or our md2 model class.  We can free
	// all of this data because we already have it stored in our own structures.

	fclose(m_FilePointer);						// Close the current file pointer

	if(m_pSkins)	 delete [] m_pSkins;		// Free the skins data
	if(m_pTexCoords) delete m_pTexCoords;		// Free the texture coord data
	if(m_pTriangles) delete m_pTriangles;		// Free the triangle face data
	if(m_pFrames)	 delete m_pFrames;			// Free the frames of animation
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// I bet this is a lot simpler than you imagined eh?  That is the beauty of the
// Quake2 model format, it's so easy to read in.  No crazy chucks to syphon through.
// The only challenge is to think of a way to efficiently parse the animations out.
// That is, if you don't use constants for the standards set on www.planetquake.com.
// You can find that info at:
//
// http://www.planetquake.com/polycount/resources/quake2/q2frameslist.shtml
// 
// All that we really changed from the last tutorial was adding a loop to
// read in the frames, instead of just storing first frame.  The ParseAnimations()
// function was also added to step through each of the frames and store off the
// animation names, animation count, and first and last frame for each animation.
//
// You might take note that in the last tutorial we calculated the face normals of our
// model, but on second thought, I decided to not do it in our final MD2 tutorial because
// the model's texture maps are created for their own lighting and just make it darker
// than usual.  Quake2 and many other games don't use dynamic lighting because it incredibly
// slow, unless it's done on a graphics card with a GPU.
//
// If you go to Main.cpp you will see how to actually interpolate between these
// key frames.  Make sure you understand that these are NOT all the frames of
// animation, but key frames to be blended together through interpolation.  More
// on these topics will be discussed in Main.cpp.
//
// I would once again like to thank Daniel E. Schoenblum <dansch@hops.cs.jhu.edu> 
// for help with explaining the file format.
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake2 .Md2 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written conset
// from ID Software.
//
// Quake and Quake2 are trademarks of ID Software.
// All trademarks used are properties of their respective owners. 
//
//

CMD2Model::CMD2Model()
{
	m_elapsedTime = 0.0f;
	m_lastTime = 0.0f;
}

CMD2Model::~CMD2Model()
{
}

CMD2Model::CMD2Model(const CMD2Model &x)
{
	// Copy all the stuff from x
	m_Texture[0] = x.m_Texture[0];
	m_Texture[1] = x.m_Texture[1];
	m_bHasWeapon = x.m_bHasWeapon;
	m_bLastFrameRendered = x.m_bLastFrameRendered;
	m_elapsedTime = x.m_elapsedTime;
	m_lastTime = x.m_lastTime;

	g1_3DModel.numOfObjects = x.g1_3DModel.numOfObjects;
	g1_3DModel.numOfMaterials = x.g1_3DModel.numOfMaterials;
	g1_3DModel.numOfAnimations = x.g1_3DModel.numOfAnimations;
	g1_3DModel.currentAnim = x.g1_3DModel.currentAnim;
	g1_3DModel.currentFrame = x.g1_3DModel.currentFrame;

	g1_3DModel.pAnimations = x.g1_3DModel.pAnimations;
	g1_3DModel.pMaterials = x.g1_3DModel.pMaterials;
	
	for (unsigned int i = 0; i < x.g1_3DModel.pObject.size(); i++) {
		t3DObject pObject = x.g1_3DModel.pObject[i];
		t3DObject copy;
		copy.numOfVerts = pObject.numOfVerts;
		copy.numOfFaces = pObject.numOfFaces;
		copy.numTexVertex = pObject.numTexVertex;
		copy.materialID = pObject.materialID;
		copy.bHasTexture = pObject.bHasTexture;
		strcpy_s(copy.strName, pObject.strName);
		copy.pVerts = new CVector3 [copy.numOfVerts];
		memcpy(copy.pVerts, pObject.pVerts, sizeof(CVector3) * copy.numOfVerts);
		copy.pNormals = NULL;
		if (pObject.pNormals != NULL) {
			copy.pNormals = new CVector3 [copy.numOfVerts];
			memcpy(copy.pNormals, pObject.pNormals, sizeof(CVector3) * copy.numOfVerts);
		}
		copy.pTexVerts = NULL;
		if (pObject.pTexVerts != NULL) {
			copy.pTexVerts = new CVector2 [copy.numTexVertex];
			memcpy(copy.pTexVerts, pObject.pTexVerts, sizeof(CVector2) * copy.numTexVertex);
		}
		copy.pFaces = NULL;
		if (pObject.pFaces != NULL) {
			copy.pFaces = new tFace [copy.numOfFaces];
			memcpy(copy.pFaces, pObject.pFaces, sizeof(tFace) * copy.numOfFaces);
		}
		g1_3DModel.pObject.push_back(pObject);
	}

	g2_3DModel.numOfObjects = x.g2_3DModel.numOfObjects;
	g2_3DModel.numOfMaterials = x.g2_3DModel.numOfMaterials;
	g2_3DModel.numOfAnimations = x.g2_3DModel.numOfAnimations;
	g2_3DModel.currentAnim = x.g2_3DModel.currentAnim;
	g2_3DModel.currentFrame = x.g2_3DModel.currentFrame;

	g2_3DModel.pAnimations = x.g2_3DModel.pAnimations;
	g2_3DModel.pMaterials = x.g2_3DModel.pMaterials;
	
	for (unsigned int i = 0; i < x.g2_3DModel.pObject.size(); i++) {
		t3DObject pObject = x.g2_3DModel.pObject[i];
		t3DObject copy;
		copy.numOfVerts = pObject.numOfVerts;
		copy.numOfFaces = pObject.numOfFaces;
		copy.numTexVertex = pObject.numTexVertex;
		copy.materialID = pObject.materialID;
		copy.bHasTexture = pObject.bHasTexture;
		strcpy_s(copy.strName, pObject.strName);
		copy.pVerts = new CVector3 [copy.numOfVerts];
		memcpy(copy.pVerts, pObject.pVerts, sizeof(CVector3) * copy.numOfVerts);
		copy.pNormals = NULL;
		if (pObject.pNormals != NULL) {
			copy.pNormals = new CVector3 [copy.numOfVerts];
			memcpy(copy.pNormals, pObject.pNormals, sizeof(CVector3) * copy.numOfVerts);
		}
		copy.pTexVerts = NULL;
		if (pObject.pTexVerts != NULL) {
			copy.pTexVerts = new CVector2 [copy.numTexVertex];
			memcpy(copy.pTexVerts, pObject.pTexVerts, sizeof(CVector2) * copy.numTexVertex);
		}
		copy.pFaces = NULL;
		if (pObject.pFaces != NULL) {
			copy.pFaces = new tFace [copy.numOfFaces];
			memcpy(copy.pFaces, pObject.pFaces, sizeof(tFace) * copy.numOfFaces);
		}
		g2_3DModel.pObject.push_back(pObject);
	}


}

float CMD2Model::ReturnCurrentTime(t3DModel *pModel, int nextFrame)
{
	//static float elapsedTime   = 0.0f;
	//static float lastTime	  = 0.0f;

	// This function is very similar to finding the frames per second.
	// Instead of checking when we reach a second, we check if we reach
	// 1 second / our animation speed. (1000 ms / kAnimationSpeed).
	// That's how we know when we need to switch to the next key frame.
	// In the process, we get the t value for how we are at to going to the
	// next animation key frame.  We use time to do the interpolation, that way
	// it runs the same speed on any persons computer, regardless of their specs.
	// It might look chopier on a junky computer, but the key frames still be
	// changing the same time as the other persons, it will just be not as smooth
	// of a transition between each frame.  The more frames per second we get, the
	// smoother the animation will be.

	// Get the current time in milliseconds
	float time = (float) GetTickCount();

	// Find the time that has elapsed since the last time that was stored
	m_elapsedTime = time - m_lastTime;

	// To find the current t we divide the elapsed time by the ratio of 1 second / our anim speed.
	// Since we aren't using 1 second as our t = 1, we need to divide the speed by 1000
	// milliseconds to get our new ratio, which is a 5th of a second.
	float t = m_elapsedTime / (1000.0f / kAnimationSpeed);
	
	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if (m_elapsedTime >= (1000.0f / kAnimationSpeed) )
	{
		// Set our current frame to the next key frame (which could be the start of the anim)
		pModel->currentFrame = nextFrame;

		// Set our last time to the current time just like we would when getting our FPS.
		m_lastTime = time;
	}

	// Return the time t so we can plug this into our interpolation.
	return t;
}


void CMD2Model::Draw(t3DModel *pModel, int currentFrame, int nextFrame, float t)
{
	
	// Get the current key frame we are on
	t3DObject *pFrame =		 &pModel->pObject[currentFrame];

	// Get the next key frame we are interpolating too
	t3DObject *pNextFrame =  &pModel->pObject[nextFrame];

	// Get the first key frame so we have an address to the texture and face information
	t3DObject *pFirstFrame = &pModel->pObject[0];

	// Start rendering lines or triangles, depending on our current rendering mode (Lft Mouse Btn)
	glBegin(GL_TRIANGLES);

		// Go through all of the faces (polygons) of the current frame and draw them
		for(int j = 0; j < pFrame->numOfFaces; j++)
		{
			// Go through each corner of the triangle and draw it.
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];

				// Get the index for each texture coordinate for this face
				int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
						
				// Make sure there was a UVW map applied to the object.  Notice that
				// we use the first frame to check if we have texture coordinates because
				// none of the other frames hold this information, just the first by design.
				if(pFirstFrame->pTexVerts) 
				{
					// Pass in the texture coordinate for this vertex
					glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
								 pFirstFrame->pTexVerts[ texIndex ].y);
				}

				// Now we get to the interpolation part! (*Bites his nails*)
				// Below, we first store the vertex we are working on for the current
				// frame and the frame we are interpolating too.  Next, we use the
				// linear interpolation equation to smoothly transition from one
				// key frame to the next.
				
				// Store the current and next frame's vertex
				CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
				CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];

				// By using the equation: p(t) = p0 + t(p1 - p0), with a time t
				// passed in, we create a new vertex that is closer to the next key frame.
				glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), // Find the interpolated X
						   vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
						   vPoint1.z + t * (vPoint2.z - vPoint1.z));// Find the interpolated Z
			}
		}
	// Stop rendering the triangles
	glEnd();	

}


///////////////////////////////// ANIMATE MD2 MODEL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This draws and animates the .md2 model by interpoloated key frame animation
/////
///////////////////////////////// ANIMATE MD2 MODEL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CMD2Model::AnimateMD2Model(t3DModel *pModel, t3DModel *pModel2, bool renderWeapon)
{
	// Now comes the juice of our tutorial.  Fear not, this is actually very intuitive
	// if you drool over it for a while (stay away from the keyboard though...).
	// What's going on here is, we are getting our current animation that we are
	// on, finding the current frame of that animation that we are on, then interpolating
	// between that frame and the next frame.  To make a smooth constant animation when
	// we get to the end frame, we interpolate between the last frame of the animation 
	// and the first frame of the animation.  That way, if we are doing the running 
	// animation let's say, when the last frame of the running animation is hit, we don't
	// have a huge jerk when going back to the first frame of that animation.  Remember,
	// because we have the texture and face information stored in the first frame of our
	// animation, we need to reference back to this frame every time when drawing the
	// model.  The only thing the other frames store is the vertices, but no information
	// about them.
	
	m_bLastFrameRendered = false;

	// Make sure we have valid objects just in case. (size() is in the vector class)
	if(pModel->pObject.size() <= 0) return;

	// Here we grab the current animation that we are on from our model's animation list
	tAnimationInfo *pAnim = &(pModel->pAnimations[pModel->currentAnim]);

	// This gives us the current frame we are on.  We mod the current frame plus
	// 1 by the current animations end frame to make sure the next frame is valid.
	// If the next frame is past our end frame, then we go back to zero.  We check this next.
	int nextFrame = (pModel->currentFrame + 1) % pAnim->endFrame;

	// If the next frame is zero, that means that we need to start the animation over.
	// To do this, we set nextFrame to the starting frame of this animation.
	if(nextFrame == 0) 
		nextFrame =  pAnim->startFrame;

	// Shouldn't be here but for some buggy MD2s, this helps
	if (nextFrame < 0)
		nextFrame = 0;
	if (pModel->currentFrame < 0)
		pModel->currentFrame = 0;
		

	// Next, we want to get the current time that we are interpolating by.  Remember,
	// if t = 0 then we are at the beginning of the animation, where if t = 1 we are at the end.
	// Anyhing from 0 to 1 can be thought of as a percentage from 0 to 100 percent complete.
	float t = ReturnCurrentTime(pModel, nextFrame);

	glBindTexture(GL_TEXTURE_2D,m_Texture[0]);
	Draw(pModel, pModel->currentFrame, nextFrame, t);
	
	if (m_bHasWeapon && renderWeapon && pModel->currentAnim < pModel2->numOfAnimations) {
		glBindTexture(GL_TEXTURE_2D,m_Texture[1]);
		Draw(pModel2, pModel->currentFrame, nextFrame, t);
	}
	if (nextFrame ==  pAnim->startFrame)
		m_bLastFrameRendered = true;

	return;
}

int CMD2Model::GetAnimation()
{
	return g1_3DModel.currentAnim;
}

void CMD2Model::Render(bool renderWeapon)
{

	
	glDisable(GL_CULL_FACE);
	AnimateMD2Model(&g1_3DModel, &g2_3DModel, renderWeapon);
	glEnable(GL_CULL_FACE);


	
}

void CMD2Model::SetAnimation(int i)
{
	// Increase the current animation and mod it by the max animations
	// and set the current frame to be the starting frame of the new animation
	g1_3DModel.currentAnim = i % (g1_3DModel.numOfAnimations);
	g1_3DModel.currentFrame = g1_3DModel.pAnimations[g1_3DModel.currentAnim].startFrame;

	
	if (m_bHasWeapon) {
		g2_3DModel.currentAnim = i % (g2_3DModel.numOfAnimations);
		g2_3DModel.currentFrame = g2_3DModel.pAnimations[g2_3DModel.currentAnim].startFrame;
	}
	
}

bool CMD2Model::Load(char *md2fileCharacter, char *imgFileCharacter, char *md2fileWeapon, char *imgFileWeapon)
{
	CLoadMD2 g1_LoadMd2;										
	CLoadMD2 g2_LoadMd2;										

	// Load the Quake2 model with it's file name and texture name
	g1_LoadMd2.ImportMD2(&g1_3DModel, md2fileCharacter, imgFileCharacter);		

	CTexture tex1;
	tex1.Load(imgFileCharacter, 1);
	m_Texture[0] = tex1.m_textureID;

	if (strlen(md2fileWeapon) == 0) {
		m_bHasWeapon = false;
		return true;
	}

	// Load the Quake2 model with it's file name and texture name
	g2_LoadMd2.ImportMD2(&g2_3DModel, md2fileWeapon, imgFileWeapon);		

	CTexture tex2;
	tex2.Load(imgFileWeapon, 1);
	m_Texture[1] = tex2.m_textureID;

	m_bHasWeapon = true;

	return true;
}

bool CMD2Model::IsLastFrameRendered()
{
	return m_bLastFrameRendered;
}

void CMD2Model::SetLastFrameRendered(bool flag)
{
	m_bLastFrameRendered = flag;
}