/* FAZER */

#ifndef __xmlsax_h
#define __xmlsax_h

#include <string>  // do not remove it from here

#include "sax.h"

// Includes C
#include <stdlib.h>

using namespace std;
// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
	XERCES_CPP_NAMESPACE_USE
#endif

class Viewports {
	
	public:
	
	unsigned short int width;
	unsigned short int height;
	bool fullscreen;
	
	short int stereoMode;
	float internocularDistance;
	
	unsigned short int position[2];
	
	float direction[3];
	float up[3];
	
	double frustum[6];
	
	float center[3];
	
	char name[256];
	
	short int projectionMode;
	
};

class Speakers {
	public:
	
	char name[256];
	short int channel;
	float position[3];	
	
};

class InputDevices {
	public:

	short int type;
	
};

class NavigationDevice {
	public:

	short int type;
	float buttons[2][16];
	float axes[2][16];
	bool defined;
	char device[256];
};
	

class Computer {
	public:
	Viewports viewports[8];
	Speakers speakers[8];
	InputDevices inputDevices[8];
	NavigationDevice navigationDevice[8];
};

class Configuration {
	public:

	Configuration();		// Constructor
	~Configuration() {};	// Destructor
	
	// NEW
		
	Computer computer;
	
	// OLD
		
	//char types[64][64];
	
	unsigned short int window; // this is used to define multiple windows (viewports) in one computer
	
	unsigned short int wand;
	unsigned short int getWandGlobal();
	void setWandGlobal();
	
	bool getJoystick(unsigned short int window);
	char *getDeviceName(unsigned short int window);
	
	unsigned short int getWidth(unsigned short int window);
	unsigned short int getHeight(unsigned short int window);
	void setWidth(unsigned short int size);
	void setHeight(unsigned short int size);
	
		
	double getLeft(unsigned short int window);
	double getRight(unsigned short int window);
	double getBottom(unsigned short int window);
	double getTop(unsigned short int window);
	double getPlanarDepth(unsigned short int window);
	
	bool getFullScreen(unsigned short int window);
	int getProjectionView(unsigned short int window);

	unsigned short int getPositionX(unsigned short int window);
	unsigned short int getPositionY(unsigned short int window);
	
	double getDirX(unsigned short int window);
	double getDirY(unsigned short int window);
	double getDirZ(unsigned short int window);
	
	double getUpX(unsigned short int window);
	double getUpY(unsigned short int window);
	double getUpZ(unsigned short int window);
	
	int stereoGlobal;
	void setStereoGlobal();
	int getStereoGlobal();
	
	int getStereo(unsigned short int window);
	void setStereo(bool i);
		
	unsigned short int getWindow(); // this return the number of windows (view ports) in the computer
	int getWidthMax();
	int getHeightMax();
	int getPositionXMin();
	int getPositionYMin();
		
};



class xml {

	public:
	
	xml(char* fileName);
	~xml();
	
	Configuration* conf;
	
};

#endif
