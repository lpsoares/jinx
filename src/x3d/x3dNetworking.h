
#ifndef __x3dNetworking_h
#define __x3dNetworking_h

#include "x3dCore.h"
#include "x3dGrouping2.h"
#include "x3dGrouping.h"
#include "x3dsound.h"
#include "x3d.h"
#include "x3dFields.h"

#include <xercesc/dom/DOMNode.hpp>

using namespace std;


class X3DNetworkSensorNode: X3DSensorNode {
	
	public:
	
	virtual ~X3DNetworkSensorNode() { } // destructor
	
	SFBool *enabled;
	SFNode *metadata;
	SFBool *isActive;
	
};

class X3DUrlObject {

	public:

	virtual ~X3DUrlObject() { }
	
};


class X3DBase;
class Anchor : public X3DGroupingNode { 
	
	public:
		
	Anchor();	// constructor
	
	MFNode   *addChildren;
	MFNode   *removeChildren;
	MFNode   *children[MAX_NUM_TRANSFORM];
	SFString *description;
	SFNode   *metadata;
	MFString *parameter;
	MFString url; // corrigir
	SFVec3f  *bboxCenter;
	SFVec3f  *bboxSize;

	static Anchor *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	unsigned int countChildren;
	bool cast;
	DOMDocument *doc;					// holds the X3D XML document

};
	

class Inline : public X3DChildNode, public X3DBoundedObject, public X3DUrlObject {
	
	public:
	
	Inline();	// constructor
	
	
	SFBool   load;
	SFNode   *metadata;
	MFString url;
	SFVec3f  *bboxCenter;
	SFVec3f  *bboxSize;
	
	
	static Inline *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	bool ami(char *type) { return( !strcmp(type,"Inline") ); }

	X3DBase *ixb;
	
};


class LoadSensor : public X3DNetworkSensorNode {

	public:
	
	SFBool  *enabled;
	SFNode  *metadata;
	SFTime  *timeOut;
	MFNode  *watchList;
	SFBool  *isActive;
	SFBool  *isLoaded;
	SFTime  *loadTime;
	SFFloat *progress;
	
};

#endif
