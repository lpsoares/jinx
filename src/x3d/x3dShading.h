#ifndef __x3dShading_h
#define __x3dShading_h

#include "x3d.h"
#include "x3dFields.h"
#include "x3dCore.h"

#include <xercesc/dom/DOMNode.hpp>

using namespace std;


class VertexShader : public X3DNode {
	
	public:
	
	VertexShader();		// constructor
		
	// A definir
	/*
	SFVec3f    *center;
	SFRotation *rotation;
	SFVec3f    *bboxSize;
	*/
		
	static VertexShader *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	char url[16][256];
	char *getURL();
	unsigned int couter_url;

};



class FragmentShader : public X3DNode {
	
	public:
	
	FragmentShader();		// constructor
		
	// A definir
	/*
	SFVec3f    *center;
	SFRotation *rotation;
	SFVec3f    *bboxSize;
	*/
		
	static FragmentShader *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	char url[16][256];
	char *getURL();
	unsigned int couter_url;
	
};



#endif
