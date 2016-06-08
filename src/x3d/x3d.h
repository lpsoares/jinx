// X3D 

#ifndef __x3d_h
#define __x3d_h

// Includes do OpenGL
#ifdef GLEW
	// GLSL
	#include <GL/glew.h>
#endif
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>


#define SOUND_MAX 16
#define MAX_MENU 50
#define CAST_STACK_SIZE 128
#define groupOnOff_MAX 64

//#define Minimum_MFVec3f 65536	// 3*15,000 values.  ISO/IEC 19775:200x, X3D
#define Minimum_MFVec3f 265536	// 3*15,000 values.  ISO/IEC 19775:200x, X3D


#define Minimum_MFVec2f 16384	// 2*15000	//15,000 values.  ISO/IEC 19775:200x, X3D
#define Minimum_MFColor 16384	// 4*15,000 values.  ISO/IEC 19775:200x, X3D     // RGBA //


//#define Minimum_MFInt32 16384	// 20,000 values.
#define Minimum_MFInt32 163840	// 20,000 values.


#define Minimum_MFFloat 16384	//


//#define Size_X3D_Message 65536   // Used in X3D Message
#define Size_X3D_Message 655360   // Used in X3D Message   // moleculas usam tudo isso
//#define Size_X3D_Message 6553600   // Used in X3D Message   // moleculas usam tudo isso


// Jinx includes
#include "../sound/sound.h"				// Sound
#include "../domerror.h" 			// DOMTreeErrorReporter
#include "../render/render.h" 	// posrender
#include "../texture/texture.h" 	// Tex

#include "x3dCore.h" 				// used for X3DNode
#include "x3dsound.h" 				// used for Sound2
#include "x3dgprimitives.h" 		// scene, head
#include "../datavector.h" 		// scene, head
#include "x3dGrouping2.h"

#include "../tesselator.h"		// tesselator

#define QUALITY 32		// this is used in the primitives

using namespace std;

class routeStruct {
	
	public:
	
	routeStruct();
	~routeStruct();
	
	static unsigned int routeCount;
	
	// Got from specification
	//Names for DEF/PROTO/field 	50 utf8 octets.
	
	char fromNode[50];
	char fromField[50];
	char toNode[50];
	char toField[50];
	
	void **field2;
	bool set;
	
};



class X3DBase : public X3DNode {
	
	public:

	X3DBase();
	
	Scene *scene;
	
	void read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	Head *head;
	
};


class X3DValues {
	public:
		
	X3DValues() {}
	
	float MFColor[Minimum_MFColor];
	float MFVec3f[Minimum_MFVec3f];
	int   MFInt32[Minimum_MFInt32];
	float MFFloat[Minimum_MFFloat];
};

class Sound;
class Sound2;
class Viewpoints;		// forward because crossing includ files
class PosRender;		// forward because crossing includ files
// This class is used to read X3d Files
class x3d {
	
	public:
	
	x3d();						// Constructor
	~x3d();									// Destructor

	
	// For casting
	static bool *castStack[CAST_STACK_SIZE];
	static unsigned int countStack;
	static void  pushCast(bool *castAddress);
	static void  popCast();
	static bool *getCast();
	
	static X3DValues *x3DValues;

	//GLUtesselator *tess;// Tesselator used in IndexedFaceSet
	
	Tesselator *tesselator;
	
	static unsigned short int getChildren(DOMNodeList *nodeList, MFNode *children[MAX_NUM_TRANSFORM]);
	
	DOMDocument *load(char* x3dFile);		
	
	// XML must be removed
	void xmlInitializing();
	
	#ifndef XERCES
		static char *message;
	#endif
	// end XML stuff
	
	int draw(PosRender *posRendert, int teye);
	X3DBase *serialize();
	X3DBase *serialize(DOMDocument *doc);
	void ray(float m[4][4]);				// cast a ray to detec objects
	
	
	void route(DOMDocument *doc);
	void route();							// Find the links
	void serializeRoute(DOMNode * node); 	// Find the links

	int group();							// Find the links
	void serializeGroup(DOMNode * node); 	// Find the links

	void serializeSoundNode(DOMNode* node);	// Find the links
	int serializeSound(Sound* temp_sound);	// Find the links
	
	static void HandleGroupMenu(int op);	//

	X3DBase *X3D(DOMNode *node);
	
	int viewpoint(Viewpoints *tviewpoints);		// Find the viewpoints
	void serializeViewpoint(DOMNode *node);		// Find the viewpoints

	static void scheck(char *DEF,double pto[3][2]);	//Function that checks if a point is inside a geometry

	void Char_a();				// Draw the letter a
	void createLetters();





	XercesDOMParser *parser;			// used to parse the x3d file
	DOMTreeErrorReporter *errReporter;	// used to report possible errors
	//DOMDocument *doc;					// holds the X3D XML document

	unsigned int countSound;
	//Sound2 *sound2[1024];
	Sound2 *sound2[SOUND_MAX];

	int eye;							// inform which eye it is rendering
	int spatialize;
	
	float location[3];
	
	Sound* sound;
			
	unsigned int light;					// Define the light number of each light
	
	Tex* texture_bmp;			// Used to open textures
	
	Viewpoints *viewpoints;
	
	X3DBase *x3dbase;
		
	PosRender *posRender;

	routeStruct **routeTable;
	



	int menuCount;							// to count the menus
	int Menu[MAX_MENU];							// to hold the menu number
	bool groupOnOff[groupOnOff_MAX];					// to hold the menu on/off
	char menuName[MAX_MENU][50];				// to hold the menu number
	
	static x3d* my_x3d;

	private:

	X3DBase *serializeNode(DOMNode * node);
	
};






#endif
