
#ifndef __x3dProto_h
#define __x3dProto_h

#include "x3d.h"

using namespace std;

#include <xercesc/dom/DOMNode.hpp>

class ExternProtoDeclare : public X3DNode {
	
	public:
	
	ExternProtoDeclare();	// constructor
	
	//void read(DOMNode *node);
	void follow() { cout << "TO DO" << endl; };
	//static Inline *get(DOMNode *node);
	
	char name[256];
	char url[16][256];

	//X3DBase *ixb;
	
};



class ProtoInstance : public X3DNode {
	
	public:
	
	ProtoInstance();	// constructor
	
	//void read(DOMNode *node);
	//void follow();
	//static Inline *get(DOMNode *node);
	
	char name[256];

	//X3DBase *ixb;
	
};



class ProtoDeclare : public X3DNode {
	
	public:
	
	ProtoDeclare();	// constructor
	
	void read(DOMNode *node);
	//void follow();
	static ProtoDeclare *get(DOMNode *node);
	
	char name[256];

	//X3DBase *ixb;
	
};



class ProtoBody : public X3DNode {
	
	public:
	
	ProtoBody();	// constructor
	
	//void read(DOMNode *node);
	//void follow();
	//static Inline *get(DOMNode *node);
	
	char name[256];

	//X3DBase *ixb;
	
};


#endif
