#ifndef __x3dLights_h
#define __x3dLights_h

using namespace std;

#include <xercesc/dom/DOMNode.hpp>

#include "x3d.h"

class X3DLightNode : public X3DNode {
	
	public:

	X3DLightNode() { }
	virtual ~X3DLightNode() { }

	virtual void read(DOMNode *node)=0;	// pure virtual function for read the dataset
	virtual void light()=0;	// pure virtual function to draw
	
};


class SpotLight : public X3DLightNode {
	
	public:
	
	SpotLight();
	
	void read(DOMNode *node);
	void light();
	static SpotLight *get(DOMNode *node);
	
	float location[4];
	float direction[3];		// Define the X, Y, Z, it is not important the location
	float color[4];			// Color of the light
	float ambientIntensity;		// The intensity of the light
	float light_ambient[4];	// the light ambient, calculated by Intensity and Color
	float cutOffAngle;			//
	
	int id; // identifi the openGL light
	
};



class PointLight : public X3DLightNode {
	
	public:
	
	PointLight();
	
	void read(DOMNode *node);
	void light();
	static PointLight *get(DOMNode *node);

	float location[4];
	float color[4];			// Color of the light
	float ambientIntensity;		// The intensity of the light
	float light_ambient[4];	// the light ambient, calculated by Intensity and Color
	
	int id; // identifi the openGL light
	
};


class DirectionalLight : public X3DLightNode {
	
	public:
	
	DirectionalLight();
	
	void read(DOMNode *node);
	void light();
	static DirectionalLight *get(DOMNode *node);
	
	float direction[4];		// Define the X, Y, Z and W direction, it is not important the location
	float color[4];			// Color of the light
	float ambientIntensity;		// The intensity of the light
	float light_ambient[4];	// the light ambient, calculated by Intensity and Color

	int id; // identifi the openGL light
	
};


#endif
