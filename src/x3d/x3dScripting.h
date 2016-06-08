
#ifndef __x3dScripting_h
#define __x3dScripting_h

#include <xercesc/dom/DOMNode.hpp>


//#include "x3dNetworking.h"
#include "x3d.h"
#include "x3dCore.h"




//class X3DScriptNode : public X3DChildNode, public X3DUrlObject { 
class X3DScriptNode : public X3DChildNode { 
	
  //SFNode *metadata;

};


class Script : public X3DScriptNode {
	
	public:
	
	SFNode    *metadata;
	
	//MFString  *url;
	char url[16][256];
	
	SFBool    *directOutput;
	SFBool    *mustEvaluate;
  	
	//And any number of:
	//fieldType [in]     fieldName
	//fieldType [in,out] fieldName
	//fieldType [out]    fieldName
	//fieldType []       fieldName
	
	field **fieldName;
	
	unsigned int countField;
	unsigned int couter_url;			// counter in the case of many urls
	
	Script();			// constructor
	
	static Script *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
};



#endif
