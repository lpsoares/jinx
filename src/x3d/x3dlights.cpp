#include "x3d.h"
#include "x3dlights.h"
#include "../math.h"


#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

// As luzes precisam ser todo instante reinstanciadas, não me pergunte porque.

// Directional light is used to simulate a parallel light, like the sunb light
DirectionalLight::DirectionalLight() {

	direction[0]=0;
	direction[1]=0;
	direction[2]=1;
	direction[3]=0;
	
	color[0]=1.0;
	color[1]=1.0;
	color[2]=1.0;
	color[3]=1.0;
	
	ambientIntensity = 0;
	
	light_ambient[0]=0.0;
	light_ambient[1]=0.0;
	light_ambient[2]=0.0;
	light_ambient[3]=1.0;

}

void DirectionalLight::light() {
	
	glLightfv(this->id, GL_POSITION, direction); // se isto for retirado a luz se movimenta junto (ver casa)
	
	
}

DirectionalLight *DirectionalLight::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	DirectionalLight *directionalLight;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((DirectionalLight *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	directionalLight = new DirectionalLight();
	directionalLight->read(node);
	
	return directionalLight;
}


void DirectionalLight::read(DOMNode *node) { 
	unsigned int i; 								// Variable to counter
	DOMNamedNodeMap *attributes;					// Variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(this->DEF,this);
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "direction")) {
			this->direction[0] = -atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->direction[1] = -atof(strtok(NULL," "));
			this->direction[2] = -atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "color")) {
			this->color[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->color[1] = atof(strtok(NULL," "));
			this->color[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "ambientIntensity")) {
			this->ambientIntensity = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		}
	}

	this->light_ambient[0] = this->ambientIntensity * this->color[0];
	this->light_ambient[1] = this->ambientIntensity * this->color[1];
	this->light_ambient[2] = this->ambientIntensity * this->color[2];
	
	this->id = x3d::my_x3d->light;
	x3d::my_x3d->light++;

	if( (x3d::my_x3d->light) >= (GL_MAX_LIGHTS+GL_LIGHT0) ) {
		cout << "Warning the number of lights has exced the number of lights supportted by hardware" << endl;
	}
	
	
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
	
	glLightfv(this->id, GL_AMBIENT, light_ambient);
	glLightfv(this->id, GL_DIFFUSE, color);
	glLightfv(this->id, GL_SPECULAR, color);
	
	glEnable(this->id);
	
}


// Point Light is
PointLight::PointLight() {
	location[0]=0.0;
	location[1]=0.0;
	location[2]=0.0;
	location[3]=1.0;
		
	color[0]=1.0;
	color[1]=1.0;
	color[2]=1.0;
	color[3]=1.0;
	
	ambientIntensity = 0;
	
	light_ambient[0]=0.0;
	light_ambient[1]=0.0;
	light_ambient[2]=0.0;
	light_ambient[3]=1.0;

}

void PointLight::light() {

	glLightfv(this->id, GL_POSITION, location); // se isto for retirado a luz se movimenta junto (ver casa)
	
	
}


PointLight *PointLight::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	PointLight *pointLight;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((PointLight *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	pointLight = new PointLight();
	pointLight->read(node);
	
	return pointLight;
}


void PointLight::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
		
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "location")) {
			location[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			location[1] = atof(strtok(NULL," "));
			location[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "color")) {
			color[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			color[1] = atof(strtok(NULL," "));
			color[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "ambientIntensity")) {
			ambientIntensity = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else 
		if (strcmp (XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) {
			cout << "Point Light argument not recognized : ";
			cout << (XMLString::transcode(attributes->item(i)->getNodeName())) << endl;
		}
	}
	
	light_ambient[0] = ambientIntensity * color[0];
	light_ambient[1] = ambientIntensity * color[1];
	light_ambient[2] = ambientIntensity * color[2];

	this->id = x3d::my_x3d->light;
	x3d::my_x3d->light++;

	if( (x3d::my_x3d->light) >= (GL_MAX_LIGHTS+GL_LIGHT0) ) {
		cout << "Warning the number of lights has exced the number of lights supportted by hardware" << endl;
	}
	
	
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
	
	
	glLightfv(this->id, GL_AMBIENT, light_ambient);
	glLightfv(this->id, GL_DIFFUSE, color);
	glLightfv(this->id, GL_SPECULAR, color);
	
	glEnable(this->id);
}

// Spot Light is used to simulate a cone, and the light source is a point
SpotLight::SpotLight() {

	location[0]=0.0;
	location[1]=0.0;
	location[2]=0.0;
	location[3]=1.0;

	direction[0]=0;
	direction[1]=0;
	direction[2]=-1;
	
	color[0]=1.0;
	color[1]=1.0;
	color[2]=1.0;
	color[3]=1.0;
	
	ambientIntensity = 0;
	
	light_ambient[0]=0.0;
	light_ambient[1]=0.0;
	light_ambient[2]=0.0;
	light_ambient[3]=1.0;

	cutOffAngle = 0.785398*180/PI;			//
}

void SpotLight::light() {

	glLightfv(this->id, GL_POSITION, location); // se isto for retirado a luz se movimenta junto (ver casa)


}



SpotLight *SpotLight::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	SpotLight *spotLight;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((SpotLight *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	spotLight = new SpotLight();
	spotLight->read(node);
	
	return spotLight;
}

void SpotLight::read(DOMNode *node) {
	unsigned int i; 								// variable to counter
	DOMNamedNodeMap *attributes;					// variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "location")) {
			this->location[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->location[1] = atof(strtok(NULL," "));
			this->location[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "color")) {
			this->color[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->color[1] = atof(strtok(NULL," "));
			this->color[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "direction")) {
			this->direction[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->direction[1] = atof(strtok(NULL," "));
			this->direction[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "ambientIntensity")) {
			this->ambientIntensity = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "cutOffAngle")) {
			this->cutOffAngle = 180*atof(XMLString::transcode (attributes->item(i)->getNodeValue()))/PI;
		}
	}
	
	this->light_ambient[0] = this->ambientIntensity * this->color[0];
	this->light_ambient[1] = this->ambientIntensity * this->color[1];
	this->light_ambient[2] = this->ambientIntensity * this->color[2];

	this->id = x3d::my_x3d->light;
	x3d::my_x3d->light++;
	
	if( (x3d::my_x3d->light) >= (GL_MAX_LIGHTS+GL_LIGHT0) ) {
		cout << "Warning the number of lights has exced the number of lights supportted by hardware" << endl;
	}

	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	
	// glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
	
	glLightf(this->id, GL_SPOT_CUTOFF, cutOffAngle);
	glLightfv(this->id, GL_SPOT_DIRECTION, direction);
	
	glLightfv(this->id, GL_AMBIENT, light_ambient);
	glLightfv(this->id, GL_DIFFUSE, color);
	glLightfv(this->id, GL_SPECULAR, color);
	
	glEnable(this->id);
	
}
