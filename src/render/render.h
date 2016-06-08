/*  */

#ifndef __render_h
#define __render_h

#ifdef GLEW
	// GLSL
	#include "ezGLSL.h"
#endif


#include "../config/xmlsax.h"
#include "../x3d/x3d.h"
#include "../communication.h"
#include "../viewer.h"
#include "../input.h"
#include "../texture/texture.h"
#include "../sball.h"
#include "../camera.h"

// Includes do MPI
#include <mpi.h>

// Includes do OpenGL
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* ASCII code for the escape key. */
#define ESCAPE 27
#define FRAMES          100.0f

#define FRUSTUM_FAR 5000
#define FRUSTUM_NEAR 0.1

using namespace std;

struct Renderer {
	float pos[3];
	float dir[3];
	float up[3];
	float vel[3];
	float rot[3];
	float trans[3];
	float rotat[3];
	int buttons;
	
	double seconds;
	short int nseconds;
	
	int wonoff;
	float head_rotat[3];
	float head_trans[3];
	float fingers[20];
	float quaternion[4];
	int type;
};

class PosRender {
	
	public:
	
	PosRender();					// constructor
	
	struct Renderer render;			//
	
	MPI_Datatype renderDatatype;	//
	
	MPI_Datatype type[16];			//

	int blocklen[16];				//
	
	MPI_Aint disp[16];				//
	
};

class Eye {
	
	public:
	
	Eye();
	~Eye();
	
	double frustum_left;					// The left part of the frustum
	double frustum_right;					// The right part of the frustum
	double frustum_top;						// The top part of the frustum
	double frustum_bottom;					// The bottom part of the frustum
	double frustum_near;					// The near clipping of the frustum
	double frustum_far;						// The far clipping of the frustum
	
};

class Viewport {
	
	public:
		
	Viewport();
	~Viewport();
	
	int posx;
	int posy;
	int width;
	int height;

};

class LookAt {
	
	public:
		
	LookAt();
	~LookAt();

	double frustum_eyex;
	double frustum_eyey;
	double frustum_eyez;
	double frustum_centrex;
	double frustum_centrey;
	double frustum_centrez;
	double frustum_upx;
	double frustum_upy;
	double frustum_upz;
	
};



class x3d; 				// forward because crossing include files
class Input; 			// forward because crossing include files
class Render {
	public:

	Render(xml* temp_lll, x3d* temp_xxx, int temp_ranks);	// Constructor
	~Render();												// Destructor

#ifdef GLEW
	// TO GLSL
	//Shader and Program Objects:
	void initializeGLSL();
	
	//--------------------------
#endif
	
	void graphCommunicator();
	static void graphCommunicatorGlobal();
	
	void drawFrameBuffer();
	
	void execute(int mode, int window);
	
	void headTracking();	// function to calculate the head tracking view
	void getScreenParameter(double *ScreenConners, double *ScreenPlanarDepth, int window);
	void calcViewport(Viewport *viewport, int window);
	void calcDistEye(float *distEye,float eye_separation);
	void setCamera(int window);
	void calcFruntum(Eye *eye, double *ScreenConners, double *headPosition, float *distEye, float PlanarDepth);
	void calcLookAt(LookAt *lookat, double *headPosition, float *distEye, float *cameradDir, float *cameraUP);
	void selectFrameBuffer(int window, int mode);
	void finishCamera(Viewport *viewport, Eye *eye, LookAt *lookat); // Set the glFrustum and gluLookAt
	
	Eye *eye;
	LookAt *lookat;
	Viewport *viewport;
	
	double headPosition[3];	// for head tracking and screen
	double headRotation[3];	// for head tracking and screen
	
	double ScreenConners[4];
	double ScreenPlanarDepth;
	double eye_separation;

	unsigned short int joystickMode;	// defines the behavior of the joystick
	
	int sendInput; // Used to control the commands send as input
	
	Camera *cam;
		
	void info();
	void quality();
	void showPerformance();		// draw the performance statistics on screen and console
	void calcPerf();		// draw the performance statistics on screen and console

		
	bool opengl; 			// esta definindo se usa programa ou leitor x3d
	void (*func)(void);
	void OpenGL(void (*nfunc)(void));

	void Gmenu();			// 
	void Gmenu_close();			// 
	void clock();			// 
	
	int start;
	int framenum;
	
	float cameraDir[3];		// set the camera direction
	float cameraUp[3];		// set the up-vector of the camera
	float distEye[3];		// set the vector that is used to separate the eyes
	
	float HeadRotX;			// Rotation for HeadTracking on X
	float HeadRotY;			// Rotation for HeadTracking on Y
	float HeadRotZ;			// Rotation for HeadTracking on Z
	
	
	float HeadTransX;			// Rotation for HeadTracking on X
	float HeadTransY;			// Rotation for HeadTracking on Y
	float HeadTransZ;			// Rotation for HeadTracking on Z
	
	
	double dist_tela;		// calculate the distance against the screen

	void graphInit(int *argc, char **argv);
	void loop();
	void drawWand();
	
	void menu();
	
	int wandType;
	bool wandRay;
	bool wandClick;
	float opening;

	int window; // The number of our GLUT window
	
	#ifdef DEBUG
	double myMPI_Time;      // to calcule the MPI time used
	double time_temp;		// to calcule the MPI time used
	#endif
	
	void InitGL();
	void Logo();

	void draw(int eye);

	static void DrawGLScene();
	static void RayTracing();
	static void DataLock();
	static void FrameLock();
	static void VisualGroup();
	static void AuxiliaryInformation();
	static void ReSizeGLScene(int Width, int Height);
	static void keyPressed(unsigned char key, int x, int y);
	static void Mouse(int button, int state, int x, int y);
	static void Motion(int x, int y);
	static void Special(int key, int x, int y);
	static void Idle();

	static void HandleMainMenu(int op);
	static void HandleShadingMenu(int op);
	static void HandleCullingMenu(int op);
	static void HandleViewpointMenu(int op);
	
	MPI_Request request_group;
	int groupOnOff;
	
	int my_rank;		// the local rank
	int ranks;			// the total number of ranks

	char *letra;
	char *fps;	
		
	MPI_Group graph_group;
	MPI_Comm graph_comm;
	int* graph_ranks;
	
	MPI_Request request;
	
	xml* lll;
	x3d* xxx;

	MPI_Request request2;

	int move;
	int motionX,motionY;

	PosRender *posRender;
	
	Input *input;

	Tex* tex;
	bool grid;			// flag to draw a grid in the floor
	bool camera;		// flag to draw a camera
	bool calibration;	// flag to draw a calibration grid
	bool framerate;		// flag to draw a framerate
	bool invertEyes;	// flag to invert eyes
	bool gmenu;
	bool gclock;
	bool gmenu_close;

	void calibrationScreen();
	bool degrade;
	unsigned short int calibrationDivisions; // if calibration number of divisions
	unsigned short int calibrationDegradeDirection; // in the case of degrade if informs the direction
	float calibrationClearColor[3];
	float calibrationClearColorPercent;
	float calibrationFrontColor[3];
	float calibrationFrontColorPercent;
	bool frontBackColor;
	GLubyte mask[32*4];
	unsigned short int calibrationPatern;
	unsigned short int calibrationLineWidth;
	bool calibrationColorBars;

	float o[4];
	float d[4];
	
	GLUquadricObj *obj;
	
	double regressivo;


	GLubyte *leftImage;			// image used to get the left view for an autostereo image
	GLubyte *rightImage;		// image used to get the right view for an autostereo image
	GLubyte *autoStereoImage;	// image used to draw the view for an autostereo image


	static Render* my_rrr;
};



#endif
