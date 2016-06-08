/*Programa teste para MPI FMOD E XML */

#ifndef __sound_h
#define __sound_h



#ifdef LINUX
    #include <wincompat.h>
	#include <fmod.h>
#endif

// Includes do Fmod em Windows
#ifdef WIN32
    #include <windows.h>
    #include <conio.h>
	#include <fmod.h>
	#include <fmod_errors.h>
#endif

#include "../communication.h"
#include "../viewer.h"
#include "../config/xmlsax.h"
#include "../www.h"

#define MAX_SOUND_SAMPLES 64

using namespace std;


class Sound {
	
	public:
	
	Sound(xml *temp_lll);		// Constructor
	~Sound();					// Destructor

	WWW *www;					// to open WWW files
	
	void setSound(char* name);	// function that sets the main sound
	int songFileUse;

	#ifdef LINUX
		FSOUND_SAMPLE *samp[MAX_SOUND_SAMPLES];
	#endif	
	
	int channel[MAX_SOUND_SAMPLES];
	int sampleNumber;
	float posSample[MAX_SOUND_SAMPLES][3];
	float velSample[MAX_SOUND_SAMPLES][3];
	int volume[MAX_SOUND_SAMPLES];

	float mute[MAX_SOUND_SAMPLES];
	
	void loadSample(char* sampleName,double pos[3],double vel[3], float mute, int volume);
	
	PosViewer* posView;			// holds the user position
	
	xml* lll;					// the configuration file
	
	void playMusic();

	private:

	char songFile[256];			// string that holds the file name of the song file
	
};



#endif
