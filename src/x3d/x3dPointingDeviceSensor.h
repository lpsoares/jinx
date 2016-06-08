

#include "x3dCore.h"
#include "x3dFields.h"


#ifndef __x3dPointingDeviceSensor_h
#define __x3dPointingDeviceSensor_h


using namespace std;

#include <xercesc/dom/DOMNode.hpp>


class X3DPointingDeviceSensorNode : public X3DSensorNode {

	public:
	
	SFString *description;
  	SFBool   *enabled;
	//SFNode   *metadata;
	SFBool   *isActive;
	SFBool   *isOver;
  
};


class X3DTouchSensorNode : public X3DPointingDeviceSensorNode { 
	
	public:
		
	SFString *description;
	SFBool   *enabled;
	//SFNode   *metadata;
	SFBool   *isActive;
	SFBool   *isOver;
	SFTime   *touchTime;
	
	SFTime   **touchTimePointer;
	unsigned short int countTouchTimePointer;
	
};
 

class TouchSensor : public X3DTouchSensorNode {
	
	public:
	
	TouchSensor();
	
	SFString *description;
	SFBool   *enabled;
	//SFNode   *metadata;
	SFVec3f  *hitNormal_changed;
	SFVec3f  *hitPoint_changed;
	SFVec2f  *hitTexCoord_changed;
	SFBool   *isActive;
	SFBool   *isOver;
	SFTime   *touchTime;
	
	void *read(DOMNode *node);
	void follow();
	static TouchSensor *get(DOMNode *node);
	bool ray(float o[4],float d[4],float m[4][4]);
	
		
	bool *cast; // in case of touch bean
	
	
};

#endif
