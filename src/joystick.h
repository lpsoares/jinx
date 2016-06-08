
#ifndef __joystick_h
#define __joystick_h

#include "input.h"

// Speed to be used
#define SPEED 0.85

using namespace std;

class Joystick {
	
	public:

//	Joystick(char *deviceName);				// constructor
	Joystick(xml* temp_lll);				// constructor
	~Joystick();							// destructor

	int joyInit(char *fileDescName);		// Joystick inicialization
	void JoyRead();							// get the data from joystick
	
	xml* lll;
	
	private: 

	char *deviceName;	// name of the device to connect  f.e. /dev/js0
	bool sucess;  		// case connected to joystick
	
	unsigned short int mode;				// with kind of joystick
	char name[128];							// name of the joystick
	int version;							// version of the joystick
		
	float data[6];							// holds the data to be sent
	
	int fd; 								// Joystick FileDescriptor
		
	unsigned short int numberOfAxes;		// number of axes that the joystik supports
	unsigned short int numberOfButtons;		// number of buttons that the joystik supports
		
	short int *axis;						// value of each axe
	short int *button;						// value of each button
		
	float speed;	// this defines the speed used by the joystick to move
	
};

#endif
