#ifndef __x3dNurbs_h
#define __x3dNurbs_h

#include "x3d.h"
#include "x3dRendering.h"

using namespace std;

#include <xercesc/dom/DOMNode.hpp>


class X3DGeometryNode;
class NurbsCurve : public X3DGeometryNode {
	
	public:
	
	NurbsCurve();
		
	static NurbsCurve *get(DOMNode *node);

	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	int f;						//* counter to fill the variable coord
	
	int numberOfPoints;
	float points[4096][3];

	float *controlPoint;
	
};

#endif
