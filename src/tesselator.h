#ifndef __tesselator_h
#define __tesselator_h

using namespace std;

// Includes do OpenGL
#ifdef GLEW
	// GLSL
	#include <GL/glew.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define NUM_INDICES 90096

class Tesselator {

	public:
	
	Tesselator();
	~Tesselator();
	
	void start();
	void draw();

	GLenum format;
	float *values;	// hold vertex, normal, texture, color
	
	GLenum *type;
	
	GLuint **indices;			// the points to make the polygon (pieces)
	unsigned int *indicesSize; // holt the number of values
	unsigned int counterIndices;		// the polygons (pieces)
	
	GLuint *tmpIndices;
	unsigned int tmpCounterIndices;
	
	unsigned int counter;			// all the vertices
	
	GLUtesselator *tess;// Tesselator
		
	static void begin(GLenum which, void * polygon_data);	
	static void end(void * polygon_data);	
	static void vertex(void *id, void * polygon_data);	
	static void combine(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut, void * polygon_data);
	static void error(GLenum error_number);
	
	//static void edgeFlag(GLboolean flag, void * polygon_data);
	
};


#endif
