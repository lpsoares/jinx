#ifndef __x3dGrouping2_h
#define __x3dGrouping2_h

#include "x3dCore.h"

#include <xercesc/dom/DOMNode.hpp>

using namespace std;

class X3DObject {
	
	public:

	virtual ~X3DObject() { } // destructor
	
};

class X3DChildNode;
class X3DGroupingNode : public X3DChildNode {
	
	public:

	virtual ~X3DGroupingNode() { } // destructor
	
};


class X3DBoundedObject : public X3DObject {
	
	public:

	virtual ~X3DBoundedObject() { }
	
};

#endif
