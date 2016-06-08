#ifndef __x3dNavigation_h
#define __x3dNavigation_h

#include "x3d.h"

#include "x3dGrouping2.h"
#include "x3dGrouping.h"

using namespace std;

#include <xercesc/dom/DOMNode.hpp>

class NavigationInfo : public X3DNode {
	
	public:
	
	NavigationInfo();		// constructor

	void *read(DOMNode *node);
	
};


class X3DGroupingNode;
class Collision : public X3DGroupingNode,public X3DSensorNode {
	
	public:
	
	Collision();							// constructor
	
	
	unsigned int countChildren;				// count the number of children
	MFNode *children[MAX_NUM_TRANSFORM];	// hold the children
		
	static Collision *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
};


class X3DGroupingNode;
class LOD : public X3DGroupingNode {
	
	public:
	
	LOD();		// constructor
	
	unsigned int countChildren;
	MFNode *children[MAX_NUM_TRANSFORM];
	
	unsigned short int counterRange;
	float *range;									// a vector with the distance values to change the geometryes
		
	static LOD *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	float transformMatrix[4][4]; 					// this matrix is used to calculate the position of the object
	
};



#endif
