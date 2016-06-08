#ifndef __avatars_h
#define __avatars_h

// Includes de C e C++
#include <string>

#include <iostream>

// Includes Matematicos
#include <math.h>

// Includes do OpenGL
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;



class Avatars {
	
	public:
	
	Avatars();					// constructor
	static void wandgl();
	static void glove(float val[20]);
	
};



#endif
