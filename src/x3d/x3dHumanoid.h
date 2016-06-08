/* FAZER */

#ifndef __x3dHumanoid_h
#define __x3dHumanoid_h


using namespace std;


#include <xercesc/dom/DOMNode.hpp>


#include "x3d.h"

class HAnimSegment : public X3DNode {
	public:
	
	HAnimSegment();
		
	void read(DOMNode *node);
	void follow();

};
	

class HAnimJoint : public X3DNode {
	
	public:
	
	HAnimJoint();
		
	void read(DOMNode *node);
	void follow();
		
	float center[3];
	float rotation[4];
	char name[256];

	HAnimJoint *hAnimJoint[1024];
	
	HAnimSegment *hAnimSegment[1024];
	
	int countHAnimJoint;
	int countHAnimSegment;
	
	
};

class HAnimHumanoid : public X3DGroupingNode {  // ta errado
	
	public:
	
	HAnimHumanoid();
		
	void *read(DOMNode *node);
	void follow();
	
	float center[3];
	float rotation[4];
	char name[256];

	
	HAnimJoint *hAnimJoint[1024];
	
	HAnimSegment *hAnimSegment[1024];

	int countHAnimJoint;
	int countHAnimSegment;
	
	bool ray(float o[4],float d[4],float m[4][4]) { return false;} ;
	
};




#endif
