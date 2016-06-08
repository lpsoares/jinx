

#include "x3dCore.h"
#include "x3dFields.h"


#ifndef __x3dEnvironmentalSensor_h
#define __x3dEnvironmentalSensor_h


using namespace std;

#include <xercesc/dom/DOMNode.hpp>

 
class X3DEnvironmentalSensorNode : public X3DSensorNode {

	public:
	
	SFVec3f *center;
	SFBool  *enabled;
	SFNode  *metadata;
	SFVec3f *size;
	SFTime  *enterTime;
	SFTime  *exitTime;
	SFBool  *isActive;
  
};

class ProximitySensor : public X3DEnvironmentalSensorNode {
	
	public:
	
	ProximitySensor();
	
	SFVec3f    *center;
	SFBool     *enabled;
	//SFNode     *metadata;
	SFVec3f    *size;
	SFTime     *enterTime;
	SFTime     *exitTime;
	SFVec3f    *centerOfRotation_changed;
	SFBool     *isActive;
	SFRotation *orientation_changed;
	SFVec3f    *position_changed;
  
	void *read(DOMNode *node);
	void follow();
	static ProximitySensor *get(DOMNode *node);
	bool ray(float o[4],float d[4],float m[4][4]);
	
};

#endif
