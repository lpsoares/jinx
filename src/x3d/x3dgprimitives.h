
#ifndef __x3dgprimitives_h
#define __x3dgprimitives_h

//--- C & C++ includes ---
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

// Jinx includes
#include "x3d.h"

#include "x3dNavigation.h"
#include "x3dGrouping.h"
#include "x3dHumanoid.h"
#include "x3dlights.h"
#include "x3dEnvironmental.h"
#include "x3dInterpolation.h"
#include "x3dTime.h"
#include "x3dCore.h"
#include "x3dsound.h"
#include "x3dNetworking.h"
#include "x3dProto.h"

#include <xercesc/dom/DOMNode.hpp>

#define MAX_NODES 2048

using namespace std;

class Meta {
	
	public:
	
	Meta();
	
	void read(DOMNode *node);
	
	char content[1024];
	char name[1024];
	
};


class Head {
	
	public:
	
	Head();
	
	void read(DOMNode *node);
	
	Meta *meta;
	
};

class Switch;
class Avatar;
class WorldInfo;
class Background;
class ProtoDeclare;
class Billboard;
class Shape;
class Transform;
class Group;
class HAnimHumanoid;
class Collision;
class Scene : public X3DNode{
	
	public:

	Scene();
	
	void read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	NavigationInfo *navigationInfo;
	
	WorldInfo *worldInfo;

	// Environment
	Fog *fog;
	Background *background;
	
	unsigned int countTimeSensor;
	TimeSensor *timeSensor[MAX_NODES];
	
	unsigned int countInterpolator;
	X3DInterpolatorNode *interpolator[MAX_NODES];
	
	unsigned int countLight;
	X3DLightNode *light[MAX_NODES];
	
	unsigned int countShape;
	Shape *shape[MAX_NODES];

	unsigned int countTransform;
	Transform *transform[MAX_NODES];

	unsigned int countAvatar;
	Avatar *avatar[MAX_NODES];

	unsigned int countGroup;
	Group *group[MAX_NODES];
	
	unsigned int countHAnimHumanoid;
	HAnimHumanoid *hAnimHumanoid[MAX_NODES];

	unsigned int countSwitch;
	Switch *switchnode[MAX_NODES];

	unsigned int countInline;
	Inline *inlinenode[MAX_NODES];

	unsigned int countProtoDeclare;
	ProtoDeclare *ProtoDeclarenode[MAX_NODES];

	unsigned int countBillboard;
	Billboard *billboard[MAX_NUM_TRANSFORM];

	unsigned int countCollision;
	Collision *collision[MAX_NUM_TRANSFORM];

	unsigned int countLOD;
	LOD *lod[MAX_NUM_TRANSFORM];


};

#endif
