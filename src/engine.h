
/* Engine */

#ifndef __engine_h
#define __engine_h

// Jinx Includes
#include "config/xmlsax.h"
#include "x3d/x3d.h"
#include "render/render.h"
#include "xmlBase.h"

using namespace std;

/* class that the user calls to dispatch the application */
class Engine {
	
	public:
	
	Engine(int *argc, char **argv);		// Constructor.
	~Engine();							// Destructor.
	
	void (*func)(void);					// Holds the function fot an openGL application.
	void OpenGL(void (*nfunc)(void));	// Sets an OpenGL application.
	
	void Configuration(char *fileName);	// Defines where the configuration file is.
	void X3D(char *tx3dfile);			// Defines where the X3D file is.
	int run();							// Function that the user calls to start the application.

	static void getTime(PosRender *);	// Gets the system time.
	static char* getSource();			// Gets the url path of X3D file.

	static Engine* my_Engine;			// The local object.

	int my_rank;	// holds the MPI rank
	int procesSize;	// holds the number of MPI Processes
	xmlBase *xmlbase;
	Communication *comm;
	
	private:
	
	void manager();						// Function that control all the system.
	
	static void *createxml(void *);		// used because need to dispatch a thread
	static void *createx3d(void *);		// used because need to dispatch a thread
	
	static void *tinput(void *);		// used because need to dispatch a thread
	static void *tgraph(void *);		// used because need to dispatch a thread
	static void *tsound(void *);		// used because need to dispatch a thread
	static void *tjoyst(void *);		// used because need to dispatch a thread
	static void *twand(void *);			// used because need to dispatch a thread
	static void *ttrack(void *);		// used because need to dispatch a thread
	static void *tmain(void *);			// used because need to dispatch a thread
		
	int* argc;							// it is necessary to MPI and OpenGL Glut.
	char** argv;						// it is necessary to MPI and OpenGL Glut.

	char configurationFileName[256];	// The name of the configurantion file.
	char x3dfile[256];					// The name of X3D file.
	char source[256];					// Holds the URL path, not the file.
	
	xml* lll; 							// Class that hold the xml configuration database.
	x3d* xxx;							// Class that hold the x3d database.
	Render* rrr;						// Class for rendering engine.
	
};

#endif
