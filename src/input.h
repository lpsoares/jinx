
#ifndef __input_h
#define __input_h

#include "viewer.h"
#include "wand/wand.h"
#include "x3d/x3d.h"
#include "x3d/x3dCore.h"
#include "render/render.h"

using namespace std;

class Viewpoints : public X3DNode {
	
	public:
	
	Viewpoints();	
	
	float position[3];
	float orientation[4];
	char description[256];
	static unsigned int counter;
	
};

class PosRender;	// forward because crossing include files
class x3d; 			// forward because crossing include files
class Input {

	public:
	
	Input(x3d *temp_xxx);
	
	
	double seconds;
	short int nseconds;
	
	float dif[3];
	float ini[3];
	
	PosRender *posRender;
	
	x3d *xxx;

	Viewpoints viewpoints[256];
	
	void createViewpoint();
	
	static void *tmovescene(void *ptr);
	void *movescene();
	void movewand();
	
	static void move(float data[6]);
	static void setViewpoint(int id);
	
	private:
	
	
};

// ---  These functions are here because they are inline  --- //



#endif
