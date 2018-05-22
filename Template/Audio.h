#pragma once
#include <windows.h>									// Header File For The Windows Library
#include ".\include\gl.h"											// Header File For The OpenGL32 Library
#include ".\include\fmod.hpp"
#include ".\include\fmod_errors.h"									// Header File For The Glaux Library


class CAudio
{
public:
	CAudio();
	~CAudio();
	bool Initialise();
	bool LoadEventSound(char *filename);
	bool PlayEventSound();
	bool LoadMusicStream(char *filename);
	bool PlayMusicStream();
	void Update();

private:
		

	void FmodErrorCheck(FMOD_RESULT result);


	FMOD_RESULT result;
	FMOD::System* m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;

	
	FMOD::Sound *m_music;
	FMOD::Channel* m_musicChannel;

};
