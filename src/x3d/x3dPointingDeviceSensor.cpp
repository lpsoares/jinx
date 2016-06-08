

#include "x3d.h"
#include "x3dPointingDeviceSensor.h"


#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


// Constructor
TouchSensor::TouchSensor() {

	this->description = NULL;
	this->enabled = NULL;
	//this->metadata = NULL;
	this->hitNormal_changed = NULL;
	this->hitPoint_changed = NULL;
	this->hitTexCoord_changed = NULL;
	this->isActive = NULL;
	this->isOver = NULL;
	this->touchTime = NULL;
	
	this->cast = NULL;

	countTouchTimePointer=0;
	touchTimePointer = new SFTime*[128];
	
	
}

void TouchSensor::follow() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (TouchSensor)" << endl;	// Message about debuging
	#endif
	
	#ifdef DEBUG3
		if(this->cast!=NULL) cout << "NAO NULL" << endl;
		if(*this->cast) cout << "TOCOU" << endl;
		if(Render::my_rrr->wandRay) cout << "RAIO" << endl;
		cout << "Touch Time = " << *this->touchTime << endl;
	#endif
	
	unsigned short int f;
		
	if(this->cast!=NULL) {
		if( (*this->cast) && (Render::my_rrr->wandClick) ) {
	
			//#ifdef DEBUG1
				cout << "Object Touched : " << this->DEF << endl;
				cout.flush();
			//#endif
		
			if( !(*this->isActive) ) {
				*this->touchTime = x3d::my_x3d->posRender->render.seconds;
				//cout << "Touch Time = " << *this->touchTime;
				//cout << "  Address TouchTime  " << this->touchTime << endl;
			}
	
			*this->isActive = true;
		
			for(f=0;f<countTouchTimePointer;f++) {
				*touchTimePointer[f] = *this->touchTime;
			//	if(!strcmp(DEF,"CT1_TOUCH") )
			//	cout << "Changing value = " << f << " to " << *touchTimePointer[f] << endl;
			}
			
		} else {
			#ifdef DEBUG1
				cout << "Outside" << endl;
			#endif
		
			if( (*this->isActive) ) {
				//*this->exitTime = x3d::my_x3d->posRender->render.seconds;
			}
		
			*this->isActive = false;
		
		}
		
	}
	
	
	
				
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (TouchSensor)" << endl;	// Message about debuging
	#endif
		
}

TouchSensor *TouchSensor::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
		
	TouchSensor *indexedFaceSet;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((TouchSensor *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	indexedFaceSet = new TouchSensor();
	
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


bool TouchSensor::ray(float o[4],float d[4], float m[4][4]) {
	return(false);
}

void *TouchSensor::read(DOMNode *node) {
	
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	unsigned int i; 				// variable to counter
	
	unsigned int f; 				// variable to counter
		
	#ifdef DEBUG1
		cout << "\t\t\t\tReading (TouchSensor)" << endl;	// Message about debuging
		//cout << "DEF : " << DEF << endl;
	#endif
	
	#ifdef DEBUG2
		cout << "\t\t\t\tfrom Node" << endl;	// Message about debuging
	#endif
	
	this->cast = x3d::getCast(); 
	if(this->cast==NULL) {
		cerr << "Error not getting cast information" << endl;
	}
	
	#ifdef DEBUG2
		cout << " Touch Sensor Cast address " << this->cast << endl;
	#endif

	unsigned int f2;
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"touchTime")) {
				#ifdef DEBUG1
					cout << "Setting enterTime" << endl;
				#endif
				
				for(f2=0;f2<routeStruct::routeCount;f2++) {
					if ( (!strcmp (x3d::my_x3d->routeTable[f2]->toNode,x3d::my_x3d->routeTable[f]->toNode)) && (f!=f2)  ) {
						if(!strcmp(x3d::my_x3d->routeTable[f2]->toField,x3d::my_x3d->routeTable[f]->toField)) {
							if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL ) ) {
								(*x3d::my_x3d->routeTable[f]->field2) = (*x3d::my_x3d->routeTable[f2]->field2);
								break;
							}
						}
					}
				}
				
				if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
					(*x3d::my_x3d->routeTable[f]->field2) = new SFTime;
				}
				
				for(f2=0;f2<routeStruct::routeCount;f2++) {
					if ( (!strcmp (x3d::my_x3d->routeTable[f2]->toNode,x3d::my_x3d->routeTable[f]->toNode)) && (f!=f2)  ) {
						if(!strcmp(x3d::my_x3d->routeTable[f2]->toField,x3d::my_x3d->routeTable[f]->toField)) {
							if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL )  && ((*x3d::my_x3d->routeTable[f2]->field2) != (*x3d::my_x3d->routeTable[f]->field2))) {
								cout << this->DEF << " Error of route ( touchTime ) numbers : " << f << " and " << f2 << endl;
								cout << "<" << x3d::my_x3d->routeTable[f]->fromNode << "," << x3d::my_x3d->routeTable[f]->fromField <<
										"," << x3d::my_x3d->routeTable[f]->toNode << "," << x3d::my_x3d->routeTable[f]->toField << ">" << (*x3d::my_x3d->routeTable[f]->field2) << endl;
								cout << "And" << endl;
								cout << "<" << x3d::my_x3d->routeTable[f2]->fromNode << "," << x3d::my_x3d->routeTable[f2]->fromField <<
										"," << x3d::my_x3d->routeTable[f2]->toNode << "," << x3d::my_x3d->routeTable[f2]->toField << ">" << (*x3d::my_x3d->routeTable[f2]->field2) << endl;
							}
							(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
						}
					}
				}
				
				touchTimePointer[countTouchTimePointer] = (SFTime (*))(*x3d::my_x3d->routeTable[f]->field2);
				countTouchTimePointer++;
				
			}
		}
	}
	
	
	if(touchTime==NULL) {
		this->touchTime = new SFTime;
		*this->touchTime = 0.0f;  // verificar isso
	}
	
	if(isActive==NULL) {
		this->isActive = new SFBool;
	}
	
	*this->isActive = false; // nao necessariamente
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "size")) {
			/*
			*this->size[0] = atof(strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," "))/2;
			*this->size[1] = atof(strtok(NULL," "))/2;
			*this->size[2] = atof(strtok(NULL," "))/2;
			if( (*this->size[0]==0) && (*this->size[1]==0) && (*this->size[2]==0) ) {
				cout << "Warning ProximitySensor size dimension equals zero " << endl;
			}

			*/
		}
		
	}
	
	
	return(NULL);
	
}
