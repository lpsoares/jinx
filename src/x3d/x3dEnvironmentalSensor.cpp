

#include "x3d.h"
#include "x3dEnvironmentalSensor.h"


#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


// Constructor
ProximitySensor::ProximitySensor() {

	this->center = NULL;
	this->enabled = NULL;
	this->metadata = NULL;
	this->size = NULL;
	this->enterTime = NULL;
	this->exitTime = NULL;
	this->centerOfRotation_changed = NULL;
	this->isActive = NULL;
	this->orientation_changed = NULL;
	this->position_changed = NULL;
	
}

void ProximitySensor::follow() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (ProximitySensor)" << endl;	// Message about debuging
	#endif
	
	#ifdef DEBUG2
		cout << "Position Values" << endl;
		cout << "Pos 0  " << -Render::my_rrr->posRender->render.pos[0] << "  Size  " << *this->size[0] << endl;
		cout << "Pos 1  " << -Render::my_rrr->posRender->render.pos[1] << "  Size  " << *this->size[1] << endl; 
		cout << "Pos 2  " << -Render::my_rrr->posRender->render.pos[2] << "  Size  " << *this->size[2] << endl; 
	#endif
	
	if( 
		
		(-Render::my_rrr->posRender->render.pos[2]) <= (*this->size[2]) &&
		(-Render::my_rrr->posRender->render.pos[2]) >= -(*this->size[2]) &&
		(-Render::my_rrr->posRender->render.pos[1]) <= (*this->size[1]) &&
		(-Render::my_rrr->posRender->render.pos[1]) >= -(*this->size[1]) &&
		(-Render::my_rrr->posRender->render.pos[0]) <= (*this->size[0]) &&
		(-Render::my_rrr->posRender->render.pos[0]) >= -(*this->size[0])
	
																			) {
		#ifdef DEBUG1
			cout << "Inside" << endl;
		#endif
	
		if( !(*this->isActive) ) {
			*this->enterTime = x3d::my_x3d->posRender->render.seconds;
		}
	
		*this->isActive = true;
				
	} else {
		#ifdef DEBUG1
			cout << "Outside" << endl;
		#endif
		
		if( (*this->isActive) ) {
			*this->exitTime = x3d::my_x3d->posRender->render.seconds;
		}
		
		*this->isActive = false;
		
	}
	
	#ifdef DEBUG2
		cout << "enterTime = " << *this->enterTime << endl;
	#endif
	
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (ProximitySensor)" << endl;	// Message about debuging
	#endif
		
}

ProximitySensor *ProximitySensor::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
		
	ProximitySensor *indexedFaceSet;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((ProximitySensor *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	indexedFaceSet = new ProximitySensor();
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(indexedFaceSet->DEF,DEF2);
		setLink(indexedFaceSet->DEF,indexedFaceSet);
	}
	
	indexedFaceSet->read(node);

	return indexedFaceSet;
}


bool ProximitySensor::ray(float o[4],float d[4], float m[4][4]) {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tRay Casting (ProximitySensor)" << endl;	// Message about debuging
	#endif
	
	return(false);
}

void *ProximitySensor::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReading (ProximitySensor)" << endl;	// Message about debuging
	#endif
	
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	unsigned int i; 				// variable to counter
	unsigned int f; 				// variable to counter

	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"enterTime")) {
				#ifdef DEBUG1
					cout << "Setting enterTime" << endl;
				#endif
				if(enterTime==NULL) {
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) { 
						(*x3d::my_x3d->routeTable[f]->field2) = new SFTime;
					}
					enterTime = (SFTime *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = enterTime;
				}
			} 			
		}
	}
	
	if(enterTime==NULL) {
		this->enterTime = new SFTime;
	}
	
	if(exitTime==NULL) {
		this->exitTime = new SFTime;
	}
	
	if(isActive==NULL) {
		this->isActive = new SFBool;
	}
	*this->isActive = false; // nao necessariamente
	
	if(size==NULL) {
		this->size = new SFVec3f[3];
	}
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "size")) {
			
			*this->size[0] = atof(strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," "))/2;
			*this->size[1] = atof(strtok(NULL," "))/2;
			*this->size[2] = atof(strtok(NULL," "))/2;
			if( (*this->size[0]==0) && (*this->size[1]==0) && (*this->size[2]==0) ) {
				cout << "Warning ProximitySensor size dimension equals zero " << endl;
			}

			
		}
		
	}

	#ifdef DEBUG1
		cout << "\t\t\t\tLeaving Reading (ProximitySensor)" << endl;	// Message about debuging
	#endif
	
	return(NULL);
	
}
