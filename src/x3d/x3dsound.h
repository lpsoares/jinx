#ifndef __x3dSound_h
#define __x3dSound_h



#include <xercesc/dom/DOMNode.hpp>


#include "x3d.h"
#include "x3dNetworking.h"

using namespace std;


//class X3DSoundNode : public X3DChildNode {
class X3DSoundNode {
	
	public:

	virtual ~X3DSoundNode() { }
	
};



class X3DSoundSourceNode : public X3DUrlObject {
	
	public:

	virtual ~X3DSoundSourceNode() { }
		
};




class AudioClip : public X3DSoundSourceNode {
	
	public:
	
	AudioClip();

	void *read(DOMNode *node);
	void follow();
	
	char url[16][256];
	char description[256];
	double vel[3];	
	
};

class Sound2 : public X3DSoundNode {
	
	public:
	
	Sound2();

	//static Sound2 *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	
	bool spatialize;
	double location[3];
	float maxFront;
	float intensity;
	
	AudioClip *audio;
	
	
};

#ifdef LINUX
#endif

#endif
