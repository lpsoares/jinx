#ifndef __x3dCore_h
#define __x3dCore_h


#include "x3dFields.h"


#include <xercesc/dom/DOMNode.hpp>


using namespace std;


// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
	XERCES_CPP_NAMESPACE_USE
#endif


#include <iostream>
#include <string>

//--- STL includes ---
#ifdef LINUX
	//#ifdef __ICC
	//	#include <stl.h>
	//#else	
		#include <map.h>
	//#endif
#else
	#include <map.h>
#endif





struct ltstr {
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) < 0;
	}
};

class X3DNode {
	
	public:

	X3DNode();
	virtual ~X3DNode() { }
	
	SFNode *metadata;
	
	static map<const char*, void *, ltstr> nodesLink;
	static void setLink(const char* DEF, void *pointer);
	static void *getLink(const char* DEF);

	char DEF[256];
	
};


class X3DChildNode : public X3DNode {

	public:

	virtual ~X3DChildNode() { } 								// destructor

	SFNode *metadata;
	
	virtual void *read(DOMNode *node)=0;						// pure virtual function for read the dataset
	virtual void follow() = 0;									//
	virtual bool ray(float o[4],float d[4],float m[4][4]) = 0;	// 

};


class X3DBindableNode : public X3DChildNode {
	
	public:
	
	SFBool *set_bind;
	SFNode *metadata;
	SFTime *bindTime;
	SFBool *isBound;
	
};


class X3DMetadataObject {
	
	public:
		
	SFString *name;
	SFString *reference;
	
};

class X3DPrototypeInstance : public X3DNode {

	public:

	SFNode *metdata;
	
};


class X3DSensorNode : public X3DChildNode {

	public:
	
	SFBool *enabled;
	SFNode *metadata;
	SFBool *isActive;

};


class MetadataDouble : public X3DNode, public X3DMetadataObject {
	
	SFNode   *metadata;
	SFString *name;
	SFString *reference;
	MFDouble *value;

};


class MetadataFloat : public X3DNode, public X3DMetadataObject { 
	
	public:
		
	SFNode   *metadata;
	SFString *name;
	SFString *reference;
	MFFloat  *value;
	
};


class MetadataInteger : public X3DNode, public X3DMetadataObject { 

	public:
	
	SFNode   *metadata;
	SFString *name;
	SFString *reference;
	MFInt32  *value;
	
};

class MetadataSet : public X3DNode, public X3DMetadataObject { 

	public:
	
	SFNode   *metadata;
	SFString *name;
	SFString *reference;
	MFNode   *value;
	
};

class MetadataString : public X3DNode, public X3DMetadataObject { 

	public:
	
	SFNode   *metadata;
	SFString *name;
	SFString *reference;
	MFString *value;
	
};



#endif
