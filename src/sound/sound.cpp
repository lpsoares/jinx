// Sound

// Jinx Includes
#include "sound.h"
#include "../engine.h"
#include "../general.h"
#include "../commands.h"

// Includes do MPI
#include <mpi.h>

//C & C++ Includes
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

// Includes for sound library
#ifdef LINUX
	#include <fmod_errors.h>
#else

#endif


#ifdef LINUX

Sound::Sound(xml* temp_lll) {

	this->lll = temp_lll;			// the configuration class
	
	this->posView = new PosViewer();// The position class
	if(this->posView==NULL) {
		cerr << "Problems in Sound" << endl;
	}
	
	www = new WWW();
	
	this->songFileUse=0;			// resete that a file sound choose to play or not
	
	this->sampleNumber=0;			// the counter for the sample musics, each music has one index

	for(int f=0; f<MAX_SOUND_SAMPLES; f++) {
		samp[f]=NULL;				// reseting samp sound file
		channel[f]=-1;				// reseting the channel
	}
	
	if (FSOUND_GetVersion() < FMOD_VERSION) { 
		cerr << "Error : You are using the wrong DLL version!  You should be using FMOD " << FMOD_VERSION << endl;
	}
	
	int key=1; // para OSS
				
    switch (key) {
	#ifdef WIN
        case '1' :  FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);break;
        case '2' :  FSOUND_SetOutput(FSOUND_OUTPUT_WINMM); break;
		case '3' :  FSOUND_SetOutput(FSOUND_OUTPUT_ASIO); break;
	#else
        case '1' :  FSOUND_SetOutput(FSOUND_OUTPUT_OSS); break;
        case '2' :  FSOUND_SetOutput(FSOUND_OUTPUT_ESD); break;
        case '3' :  FSOUND_SetOutput(FSOUND_OUTPUT_ALSA); break;
	#endif
	}
	
	FSOUND_SetDriver(0);	// Select sound card (0 = default)

	unsigned int caps = 0;
	
	FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
	
	cout << endl << "Sound driver capabilities" << endl;

	if (!caps)
		cout << "\tThis driver will support software mode only" << endl << "\tIt does not properly support 3D sound hardware." << endl;
	if (caps & FSOUND_CAPS_HARDWARE)
		cout << "\tDriver supports hardware 3D sound!" << endl;
	if (caps & FSOUND_CAPS_EAX2)
		cout << "\tDriver supports EAX 2 reverb!" << endl;
	//if (caps & FSOUND_CAPS_EAX3)
	//	cout << "\tDriver supports EAX 3 reverb!" << endl;
			
    //if (!FSOUND_Init(44100, 64, FSOUND_INIT_GLOBALFOCUS)) //Output rate in hz, maximum number of SOFTWARE channels
	if (!FSOUND_Init(44100, MAX_SOUND_SAMPLES, 0)) { //Output rate in hz, maximum number of SOFTWARE channels  
		cerr << FMOD_ErrorString(FSOUND_GetError()) << endl;
	}
	
}

#else

Sound::Sound(xml* temp_lll) {
	
}

#endif

/* Destructor */
Sound::~Sound() {
	delete this->posView;
}

/* Set Sound */
void Sound::setSound(char* name) {
	strcpy(this->songFile,name);
	this->songFileUse=1;			// indicate that a file sound was choose
}

#ifdef LINUX

void Sound::playMusic() {
	
	MPI_Status status;
	char error_message[MPI_MAX_ERROR_STRING];
	int message_length;
	int error_code = MPI_SUCCESS;
	
	FMUSIC_MODULE *mod = NULL;
	
	if(this->songFileUse) {
			
		www->getfile(this->songFile, Engine::getSource());
		
		mod = FMUSIC_LoadSong(www->complete(this->songFile)); // Here is read the song file

		if (!mod){
			cerr << "Error reading sound file : ";
			cerr << this->songFile << endl;
			cerr << "Error : ";
			cerr << FMOD_ErrorString(FSOUND_GetError()) << endl ;
		}

		FMUSIC_PlaySong(mod); // PLAY SONG

		//FMUSIC_SetMasterVolume(FMUSIC_MODULE *mod,int volume);
		FMUSIC_SetMasterVolume(mod,20);
		
		#ifdef DEBUG
		cout << "Playing :" << FMUSIC_GetName(mod) << endl;
		#endif
	
	}
	
	#ifdef DEBUG
		cout << "FSOUND Mixer         : ";
		switch (FSOUND_GetMixer()) {
			case FSOUND_MIXER_BLENDMODE:    cout << "FSOUND_MIXER_BLENDMODE\n"; break;
			case FSOUND_MIXER_MMXP5:        cout << "FSOUND_MIXER_MMXP5\n"; break;
			case FSOUND_MIXER_MMXP6:        cout << "FSOUND_MIXER_MMXP6\n"; break;
			case FSOUND_MIXER_QUALITY_FPU:  cout << "FSOUND_MIXER_QUALITY_FPU\n"; break;
			case FSOUND_MIXER_QUALITY_MMXP5:cout <<"FSOUND_MIXER_QUALITY_MMXP5\n"; break;
			case FSOUND_MIXER_QUALITY_MMXP6:cout <<"FSOUND_MIXER_QUALITY_MMXP6\n"; break;
		};
		cout << "FSOUND Driver        : " << FSOUND_GetDriverName(FSOUND_GetDriver()) << endl;
		cout << "Hardware 3D channels : " << FSOUND_GetNumHardwareChannels() << endl;
	#endif
	
	FSOUND_3D_SetRolloffFactor(100);
		
	for(int f=0;f<sampleNumber;f++) {
		channel[f] = FSOUND_PlaySoundEx(FSOUND_FREE, samp[f], NULL, TRUE);
		FSOUND_3D_SetAttributes(channel[f], posSample[f], velSample[f]);
		FSOUND_SetPaused(channel[f], FALSE);
	}
	
	float pos[MAX_SOUND_SAMPLES][3];
	float vel[MAX_SOUND_SAMPLES][3];
		
	pos[0][0]=9999999; // very far to not play any sound
	pos[0][1]=9999999; // very far to not play any sound
	pos[0][2]=9999999; // very far to not play any sound
	vel[0][0]=0;
	vel[0][1]=0;
	vel[0][2]=0;

	float dir_listener[3];
	float up_listener[3];
		
	dir_listener[0]=0;
	dir_listener[1]=0;
	dir_listener[2]=-1;
	up_listener[0]=0;
	up_listener[1]=1;
	up_listener[2]=0;

	MPI_Request request;		// used to get the data from main process

	MPI_Irecv(&this->posView->view, 1, this->posView->viewDatatype, MAIN, RENDER_MOVEMENT, MPI_COMM_WORLD, &request);


// COMMANDS

		MPI_Status statusZ;
		MPI_Request requestZ;		// used to get the data from main process
		int receivedZ=1;
		Commands *comm1 = NULL;
		comm1 = new Commands();
		if(comm1==NULL) {
			cout << "Error in Commands" << endl;
		}

		MPI_Irecv(&comm1->command, 1, comm1->commandsDatatype,MAIN , COMMAND, MPI_COMM_WORLD, &requestZ);

// COMMANDS







	do {
				
		MPI_Wait(&request, &status);
		
		#ifdef DEBUG
		if(this->songFileUse) 
		{
			cout << "order = " << FMUSIC_GetOrder(mod) << " / " << FMUSIC_GetNumOrders(mod) 
				 << ", row = " << FMUSIC_GetRow(mod) << "/" << FMUSIC_GetPatternLength(mod, FMUSIC_GetOrder(mod)) 
				 << " channels playing = " << FSOUND_GetChannelsPlaying() << " cpu usage = " << FSOUND_GetCPUUsage() << endl;
		}
		#endif
		
			/*   This comentary is get from the fmod documentation
			FSOUND_3D_Listener_SetAttributes(F_FLOAT_API *pos,F_FLOAT_API *vel,F_FLOAT_API fx,F_FLOAT_API fy,F_FLOAT_API fz,
											 F_FLOAT_API tx,F_FLOAT_API ty,F_FLOAT_API tz);
			Parameters  pos Pointer to a position vector (xyz float triplet), of the listener in world space,
						measured in distance units. This can be NULL to ignore it.
						vel Pointer to a velocity vector (xyz float triplet), of the listener measured in
						distance units PER SECOND. This can be NULL to ignore it.
						fx x component of a FORWARD unit length orientation vector
						fy y component of a FORWARD unit length orientation vector
						fz z component of a FORWARD unit length orientation vector
						tx x component of a TOP or upwards facing unit length orientation vector
						ty y component of a TOP or upwards facing unit length orientation vector
						tz z component of a TOP or upwards facing unit length orientation vector
			*/	
	
			if(false) {
				
				pos[0][0]=-this->posView->view.pos[0];
				pos[0][1]=-this->posView->view.pos[1];
				pos[0][2]=-this->posView->view.pos[2];
			
				dir_listener[0]=this->posView->view.dir[0];
				dir_listener[1]=this->posView->view.dir[1];
				dir_listener[2]=-this->posView->view.dir[2];
			
				up_listener[0]=-this->posView->view.up[0];  // verificar
				up_listener[1]=-this->posView->view.up[1];  // verificar
				up_listener[2]=this->posView->view.up[2];	// verificar
				
			} else {
				
				pos[0][0]=this->posView->view.pos[0];
				pos[0][1]=this->posView->view.pos[1];
				pos[0][2]=this->posView->view.pos[2];
			
				dir_listener[0]=this->posView->view.dir[0];
				dir_listener[1]=this->posView->view.dir[1];
				dir_listener[2]=-this->posView->view.dir[2];
			
				up_listener[0]=-this->posView->view.up[0];  // verificar
				up_listener[1]=-this->posView->view.up[1];  // verificar
				up_listener[2]=this->posView->view.up[2];	// verificar
			}
	
			#ifdef DEBUG
				cout << "Number of Samples playing : " << sampleNumber << endl;
			#endif
	
			float calc;
			// for longer distances that maxFront or Back
			for(int f=0;f<sampleNumber;f++) {
				if(mute[f]!=-1) {
					calc = sqrt(
						((posSample[f][0]-pos[0][0])*(posSample[f][0]-pos[0][0])) +
						((posSample[f][1]-pos[0][1])*(posSample[f][1]-pos[0][1])) +
						((posSample[f][2]-pos[0][2])*(posSample[f][2]-pos[0][2]))
						);
				
					#ifdef DEBUG
						cout << "calc" << calc << endl;
					#endif
					
					if(calc<=mute[f]) {
						//#ifdef DEBUG
							cout << "Playing sound because is near" << endl;
						//#endif
						FSOUND_SetMute(channel[f],0);
					} else {
						//#ifdef DEBUG
							cout << "Not playing sound because is far" << endl;
						//#endif
						FSOUND_SetMute(channel[f],1);
					}
				}
			}
	
			#ifdef DEBUG
			cout << " Dir listener X: " << dir_listener[0] <<
					" Dir listener Y: " << dir_listener[1] <<
					" Dir listener Z: " << dir_listener[2] << endl;
			cout << " Up listener X: " << up_listener[0] <<
					" Up listener Y: " << up_listener[1] <<
					" Up listener Z: " << up_listener[2] << endl;
			#endif
			
			FSOUND_3D_Listener_SetAttributes(pos[0], vel[0], dir_listener[0], dir_listener[1], dir_listener[2],
															 up_listener[0] , up_listener[1] , up_listener[2]);
			
			#ifdef DEBUG
				cout << "pos : " << pos[0][0] << " " << pos[0][1] << " " << pos[0][2] << endl;
			#endif
			
			FSOUND_Update();
				
			error_code = MPI_Irecv(&this->posView->view, 1, this->posView->viewDatatype, MAIN, RENDER_MOVEMENT, MPI_COMM_WORLD, &request);
			
			if(error_code != MPI_SUCCESS) {
				MPI_Error_string(error_code, error_message, &message_length);
				cerr << "ERRO Recv: " << error_message << endl;
				MPI_Abort(MPI_COMM_WORLD, -1);
			}
			
			// COMMANDS

			receivedZ=1;
			MPI_Test(&requestZ, &receivedZ, &statusZ);
			if(receivedZ) {
			
				MPI_Wait(&requestZ, &statusZ);
				MPI_Irecv(&comm1->command, 1, comm1->commandsDatatype, MAIN , COMMAND, MPI_COMM_WORLD, &requestZ);
			
				#ifdef DEBUG
					cout << "Retorno SOUND : " << comm1->command.reboot << endl;
					cout << "Reading : " << comm1->command.url << endl;
				#endif
				
				Engine::my_Engine->X3D(comm1->command.url);
				
			}

			// COMMANDS
			
			


			
			
		} while (!comm1->command.reboot);

		if(this->songFileUse)
		{
			#ifdef DEBUG
				cout << "Closssing sound module" << endl;
			#endif
			FMUSIC_FreeSong(mod);// FREE SONG AND SHUT DOWN
			FSOUND_Close();
		}
		
}

#else

void Sound::playMusic() {
	
	while(true) {

		breath(); // this is just an sleep
			
	}
	
}

#endif


#ifdef LINUX

void Sound::loadSample(char* sampleName,double pos[3],double vel[3], float mute, int volume) {
	
	www->getfile(sampleName, Engine::getSource());
	
	samp[this->sampleNumber] = FSOUND_Sample_Load(FSOUND_FREE, www->complete(sampleName), FSOUND_HW3D, 0, 0);
	
	if (!samp[this->sampleNumber]) {
		//cerr << " Sound error ->" << FMOD_ErrorString(FSOUND_GetError()) << " : " << sampleName << endl;
		cerr << " Sound error ->" << FMOD_ErrorString(FSOUND_GetError()) << " : " << www->complete(sampleName) << endl;
		FSOUND_Sample_Free(samp[this->sampleNumber]);
	}

	if(false) {
		this->posSample[this->sampleNumber][0] = (float)pos[0];	// reading the position
		this->posSample[this->sampleNumber][1] = (float)pos[1];	// reading the position
		this->posSample[this->sampleNumber][2] = (float)pos[2];	// reading the position
		this->velSample[this->sampleNumber][0] = -(float)vel[0];	// reading the velocity
		this->velSample[this->sampleNumber][1] = -(float)vel[1];	// reading the velocity
		this->velSample[this->sampleNumber][2] = -(float)vel[2];	// reading the velocity
	} else {
		
		this->posSample[this->sampleNumber][0] = -(float)pos[0];	// reading the position
		this->posSample[this->sampleNumber][1] = -(float)pos[1];	// reading the position
		this->posSample[this->sampleNumber][2] = -(float)pos[2];	// reading the position
		this->velSample[this->sampleNumber][0] = -(float)vel[0];	// reading the velocity
		this->velSample[this->sampleNumber][1] = -(float)vel[1];	// reading the velocity
		this->velSample[this->sampleNumber][2] = -(float)vel[2];	// reading the velocity
	}
	
	this->volume[this->sampleNumber] = volume;
	
	this->mute[this->sampleNumber] = mute; // to cancel the sound in certain distance
	
	#ifdef DEBUG
	cout << "posSample" << this->posSample[this->sampleNumber][0] << endl;
	#endif
	
	// increasing mindistnace makes it louder in 3d space
	//FSOUND_Sample_SetMinMaxDistance(samp[this->sampleNumber], 2.0f, 1000.0f);
	
		FSOUND_Sample_SetMinMaxDistance(samp[this->sampleNumber], 50.0f, 2000.0f);   //para Rio
//		FSOUND_Sample_SetMinMaxDistance(samp[this->sampleNumber], 500.0f, 1000.0f); // para Casa
	
	FSOUND_Sample_SetMode(samp[this->sampleNumber], FSOUND_LOOP_NORMAL);    // pau no compilador da intel

	/* signed char F_API  FSOUND_Sample_SetDefaults(FSOUND_SAMPLE *sptr,int deffreq,int defvol,int defpan,int defpri); */
	FSOUND_Sample_SetDefaults(samp[this->sampleNumber],-1,this->volume[this->sampleNumber],FSOUND_STEREOPAN,-1);
	
	//cout << "The volume hight now is : " << FSOUND_GetVolume(samp[this->sampleNumber]) << endl;

	this->sampleNumber++;	// increase the sample number
	if(this->sampleNumber > MAX_SOUND_SAMPLES )
		cerr << "Maximum number of samples reached" << endl;

}

#else

void Sound::loadSample(char* sampleName,double pos[3],double vel[3], float mute, int volume) {

}

#endif
