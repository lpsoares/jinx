#ifndef __x3dGrouping_h
#define __x3dGrouping_h

#define MAX_NUM_TRANSFORM 2048 // Maximuni number of transform in a transform or group

#include "x3d.h"
#include "x3dFields.h"
#include "x3dCore.h"
#include "x3dGrouping2.h"

#include <xercesc/dom/DOMNode.hpp>

using namespace std;

class WorldInfo {
	
	public:
	
	WorldInfo();		// constructor
	
	void read(DOMNode *node);
	
};

class Group : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	Group();			// constructor
	
	MFNode	   *children[MAX_NUM_TRANSFORM];
	
	static Group *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	bool ami(char *type) { return( !strcmp(type,"Group") ); }
	
	unsigned int countChildren;
	
	bool cast; // in case of touch bean
	
};

class Transform : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	Transform();		// constructor
		
	MFNode	   *children[MAX_NUM_TRANSFORM];
	SFVec3f    *center;
	SFRotation *rotation;
	SFVec3f    *scale;
	SFRotation *scaleOrientation;
	SFVec3f    *translation;
	SFVec3f    *bboxCenter;
	SFVec3f    *bboxSize;
	
	static Transform *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	bool ami(char *type) { return( !strcmp(type,"Transform") ); }
	
	unsigned int countChildren;
	
	bool cast; // in case of touch bean
};


class Switch : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	Switch();		// constructor
	
	short int whichChoice;
	
	MFNode	   *children[MAX_NUM_TRANSFORM];

	static Switch *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]) { return false;} ;
	
	bool ami(char *type) { return( !strcmp(type,"Switch") ); }
	
	unsigned int countChildren;
	
};

class StaticGroup : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	StaticGroup();		// constructor
	
	MFNode	   *children[MAX_NUM_TRANSFORM];

	static StaticGroup *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	bool ami(char *type) { return( !strcmp(type,"StaticGroup") ); }
	
	unsigned int countChildren;
	
	bool cast; // in case of touch bean
};



class Billboard : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	Billboard();		// constructor
	
	MFNode	   *children[MAX_NUM_TRANSFORM];
	
	static Billboard *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]) { return true;} ;
	
	bool ami(char *type) { return( !strcmp(type,"Billboard") ); }
	
	unsigned int countChildren;
	
	bool cast; // in case of touch bean
};


class Avatar : public X3DGroupingNode, public X3DBoundedObject {
	
	public:
	
	Avatar();		// constructor

	unsigned int countChildren;
	MFNode	   *children[MAX_NUM_TRANSFORM];
	
	double translate[3];
	double scale[3];
	double rotate[4];
	
	static Avatar *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

};




#endif
