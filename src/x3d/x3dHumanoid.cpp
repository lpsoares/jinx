#include "x3d.h"
#include "../math.h"

#include "x3dHumanoid.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


HAnimHumanoid::HAnimHumanoid() {
	
	center[0]=0;
	center[1]=0;
	center[2]=0;
	
	rotation[0]=0;
	rotation[1]=0;
	rotation[2]=1;
	rotation[3]=0;
	
	strcpy(name,"");

	countHAnimJoint=0;
	countHAnimSegment=0;

}

void HAnimHumanoid::follow() {

	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (HAnimHumanoid)" << endl;	// Message about debuging
	#endif
	
	int f=0;

/*	
	for(f=0;f<x3d::my_x3d->routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f].toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f].toField,"center")) {
				//this->humanoidCenter[0] = center[0] = atof(strtok(x3d::my_x3d->routeTable[f].fvalue," "));
				//this->humanoidCenter[1] = center[1] = atof(strtok(NULL," "));
				//this->humanoidCenter[2] = center[2] = atof(strtok(NULL," "));
			} else
			if( !strcmp(x3d::my_x3d->routeTable[f].toField,"rotation")  ) {
				rotation[0] = atof(strtok(x3d::my_x3d->routeTable[f].fvalue," "));
				rotation[1] = atof(strtok(NULL," "));
				rotation[2] = atof(strtok(NULL," "));
				rotation[3] = 180*atof(strtok(NULL," "))/PI;
			} else
			if(!strcmp(x3d::my_x3d->routeTable[f].toField,"name")) {
				strcpy(name,x3d::my_x3d->routeTable[f].fvalue);
			}	
		}
	}
	*/
	glRotatef(rotation[3],rotation[0],rotation[1],rotation[2]);
	
	for(f=0;f<countHAnimJoint;f++) {
		hAnimJoint[f]->follow();
	}
	
	for(f=0;f<countHAnimSegment;f++) {
		hAnimSegment[f]->follow();
	}
	
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (HAnimHumanoid)" << endl;	// Message about debuging
	#endif
	
}
	
void *HAnimHumanoid::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes

	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
		} 
	}

	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "center")) {
			//this->humanoidCenter[0] = center[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			//this->humanoidCenter[1] = center[1] = atof(strtok(NULL," "));
			//this->humanoidCenter[2] = center[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "rotation")) {
			rotation[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			rotation[1] = atof(strtok(NULL," "));
			rotation[2] = atof(strtok(NULL," "));
			rotation[3] = 180*atof(strtok(NULL," "))/PI;
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "name")) {
			strcpy(name,XMLString::transcode (attributes->item(i)->getNodeValue()));
		}
	}

	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength(); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"HAnimJoint")) {
				hAnimJoint[this->countHAnimJoint] = new HAnimJoint();
				hAnimJoint[this->countHAnimJoint]->read(children->item(i));
				this->countHAnimJoint++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"HAnimSegment")) {
				hAnimSegment[this->countHAnimSegment] = new HAnimSegment();
				hAnimSegment[this->countHAnimSegment]->read(children->item(i));
				this->countHAnimSegment++;
			} 
		}
	}

	return NULL;
	
}


HAnimJoint::HAnimJoint() {
	
	center[0]=0;
	center[1]=0;
	center[2]=0;
	
	rotation[0]=0;
	rotation[1]=0;
	rotation[2]=1;
	rotation[3]=0;
	
	strcpy(name,"");
	
	countHAnimJoint=0;
	countHAnimSegment=0;

}

void HAnimJoint::follow() {

	int f=0;
	/*
	for(f=0;f<x3d::my_x3d->routeCount;f++) {
		if (!strcmp(x3d::my_x3d->routeTable[f].toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f].toField,"center")) {
				center[0] = atof(strtok(x3d::my_x3d->routeTable[f].fvalue," "));
				center[1] = atof(strtok(NULL," "));
				center[2] = atof(strtok(NULL," "));
			} else
			if( !strcmp(x3d::my_x3d->routeTable[f]->toField,"rotation")  ) {
				rotation[0] = atof(strtok(x3d::my_x3d->routeTable[f]->fvalue," "));
				rotation[1] = atof(strtok(NULL," "));
				rotation[2] = atof(strtok(NULL," "));
				rotation[3] = 180*atof(strtok(NULL," "))/PI;
			} else
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"name")) {
				strcpy(name,x3d::my_x3d->routeTable[f]->fvalue);
			}	
		}
	}
	*/
	glPushMatrix();
	
	glRotatef(rotation[3],rotation[0],rotation[1],rotation[2]);
	//glTranslatef(center[0]-this->humanoidCenter[0],center[1]-this->humanoidCenter[1],center[2]-this->humanoidCenter[2]);

	for(f=0;f<countHAnimSegment;f++) {
		this->hAnimSegment[f]->follow(); 
	}

	glPopMatrix();
	
	for(f=0;f<countHAnimJoint;f++) {
		this->hAnimJoint[f]->follow();
	}
	
	
}

void HAnimJoint::read(DOMNode *node) {
	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "center")) {
			center[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			center[1] = atof(strtok(NULL," "));
			center[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "rotation")) {
			rotation[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			rotation[1] = atof(strtok(NULL," "));
			rotation[2] = atof(strtok(NULL," "));
			rotation[3] = 180*atof(strtok(NULL," "))/PI;
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "name")) {
			strcpy(name,XMLString::transcode (attributes->item(i)->getNodeValue()));
		}
	}

	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength(); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"HAnimSegment")) {
				hAnimJoint[this->countHAnimJoint] = new HAnimJoint();
				hAnimJoint[this->countHAnimJoint]->read(children->item(i));
				this->countHAnimJoint++;
			} 
		}
		for (i = 0; i < children->getLength(); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"HAnimJoint")) {
				hAnimSegment[this->countHAnimSegment] = new HAnimSegment();
				hAnimSegment[this->countHAnimSegment]->read(children->item(i));
				this->countHAnimSegment++;
			}
		}
	}

	
	
}

HAnimSegment::HAnimSegment() {
}

void HAnimSegment::follow() {
	
}

void HAnimSegment::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes

	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
		} 
	}

	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength(); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Shape")) {
				//shape(children->item(i));
			}
		}
	}

	
}
