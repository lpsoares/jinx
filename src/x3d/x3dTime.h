#ifndef __x3dTime_h
#define __x3dTime_h

#include "x3dFields.h"
#include "x3dCore.h"


using namespace std;

#include <xercesc/dom/DOMNode.hpp>


class X3DTimeDependentNode : public X3DChildNode {
	
	public:
	
	SFBool  *loop;
	SFNode  *metadata;
	SFTime  *pauseTime;
	SFTime  *resumeTime;
	SFTime  *startTime;
	SFTime  *stopTime;
	SFTime  *elapsedTime;
	SFBool  *isActive;
	SFBool  *isPaused;
	
};

//class TimeSensor : public X3DTimeDependentNode, public X3DSensorNode {
class TimeSensor : public X3DTimeDependentNode { // corrigir ( problema de ambiguidade)
	
	public:
	
	TimeSensor();
	
	SFTime  *cycleInterval;
	SFBool  *enabled;
  	SFBool  *loop;
	SFNode  *metadata;
	SFTime  *pauseTime;
	SFTime  *resumeTime;
	SFTime  *startTime;
	
		// Created by me
		short int  *startTimeNS;
	
	SFTime  *stopTime;
	SFTime  *cycleTime;
	SFTime  *elapsedTime;
	SFFloat *fraction_changed;
	SFBool  *isActive;
	SFBool  *isPaused;
	SFTime  *time;

	void *read(DOMNode *node);
	void follow();
	static TimeSensor *get(DOMNode *node);
	bool ray(float o[4],float d[4],float m[4][4]);


	SFFloat   **fraction_changedPointer;
	unsigned short int countFraction_changedPointer;

};

#endif
