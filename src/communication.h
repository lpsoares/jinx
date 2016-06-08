#ifndef __communication_h
#define __communication_h

// MPI Includes
#include <mpi.h>

#define INPUT_MOVE 19
#define INPUT_WAND 18

#define INPUT_FW 20
#define INPUT_TRANSLATE 21
#define INPUT_UP 22
#define INPUT_RX 23
#define INPUT_RY 24
#define INPUT_RZ 25
#define INPUT_VIEWPOINT 26
#define OUTPUT_NVIEWPOINT 27
#define OUTPUT_VIEWPOINT 28

#define VIEWER_MOVEMENT 3
#define RENDER_MOVEMENT 1
#define WAND_MOVEMENT 4
#define GROUP_ONOFF	8
#define INPUT_UPDATE	9
#define COMMAND 10

#define MAIN 0
#define INPUT 1
#define SOUND 2
#define JOYSTICK 3
#define WAND 4
#define TRACK 5
#define GRAPH 6

#include "viewer.h"
#include "render/render.h"
#include "wand/wand.h"


class Communication {
	
	public:
		
	Communication(int *argc, char **argv);
	~Communication();
	int getRID();							// get rank id
	
};

class PosWand;
class PosRender;
class Tunnel {
	
	public:
		
	Tunnel(int type);
	
	PosViewer* posView;
	PosWand* posWand;
	PosViewer* posView2; // for sound
	
	MPI_Request request;
	
	void getViewPosition(PosRender* posRender);
	void getWand(PosRender* posRender);
	void setRender(PosRender* posRender);
	void setSound(PosRender* posRender);
	
	
};

#endif
