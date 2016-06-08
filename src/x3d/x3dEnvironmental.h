
#include "x3d.h"

#ifndef __x3dEnvironmental_h
#define __x3dEnvironmental_h

#include "../math.h"

using namespace std;

#include <xercesc/dom/DOMNode.hpp>


class Background : public X3DNode {
	
	public:
	
	Background();
	
	bool BackgrounfTexture;

	void read(DOMNode *node);
	void follow();
	static Background *get(DOMNode *node);

	double skyColor[1024][3];
	int point;
	
	double groundColor[1024][3];
	int gpoint;
	
	double skyAngle[1024];				// the sky angles
	double groundAngle[1024];			// the groung angles
	
	unsigned int cskyAngle;				// counter for sky angles
	unsigned int cgroundAngle;			// counter for ground angles

	char backUrl[16][256];
	char bottomUrl[16][256];
	char frontUrl[16][256];
	char leftUrl[16][256];
	char rightUrl[16][256];
	char topUrl[16][256];


};






class Fog : public X3DBindableNode {
	
	public:
	
	Fog();
	
	SFBool   set_bind;
	SFColor  *color;
	SFString *fogType;
	//SFNode   metadata;
	SFFloat  *visibilityRange;
	SFTime   bindTime;
	SFBool   isBound;
	
	void *read(DOMNode *node);
	void follow();
	static Fog *get(DOMNode *node);
	bool ray(float o[4],float d[4],float m[4][4]);
	
};




#endif
