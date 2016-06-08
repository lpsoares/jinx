#include "x3d.h"
#include "x3dTime.h"
#include "x3dFields.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


// Constructor
TimeSensor::TimeSensor() {
	
	this->enabled = NULL;			//
	this->cycleInterval = NULL;		// 
	this->fraction_changed = NULL;	//
	this->startTime = NULL;			// the time defined as beggining, used to reset the timer
	this->startTimeNS = NULL;			// the time defined as beggining, used to reset the timer
	
	this->isActive = NULL;			//
	this->loop = NULL;				//
	
	
	countFraction_changedPointer=0;
	fraction_changedPointer = new SFFloat*[128];
	
}

bool TimeSensor::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void TimeSensor::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (TimeSensor)" << endl;	// Message about debuging
	#endif
	
	unsigned short int f;
	
	if(*this->loop) { // in the case of looping, just repeat for ever

		#ifdef DEBUG2
			cout << "In loop" << endl;
			cout << "local time " << x3d::my_x3d->posRender->render.seconds << endl;
			cout << "startTime  " << *this->startTime << endl;
			cout << "local - start = " << x3d::my_x3d->posRender->render.seconds - *this->startTime << endl;
			cout << "Cicle Interval = " << *this->cycleInterval << endl;
			cout << "CI " << ((*this->cycleInterval)) << " ,  " << ( 100 * (float) (*this->cycleInterval) )  << endl;
			cout << "NS " << ((float)(x3d::my_x3d->posRender->render.nseconds)) << endl;
		
			cout << "Pri = " << ((float)(x3d::my_x3d->posRender->render.nseconds) + ( ((float)( (int) (x3d::my_x3d->posRender->render.seconds )) * 100 ))) << endl;
			cout << "Sec = " << ((float)(*this->startTimeNS) + ( ((float)( (int) (*this->startTime )) * 100 ))) << endl;
			cout << "Pri - Sec = " << ( ((double)(x3d::my_x3d->posRender->render.nseconds) + ( ((double)( (int) (x3d::my_x3d->posRender->render.seconds )) * 100 )))  -
						  ((double)(*this->startTimeNS) + ( ((double)( (int) (*this->startTime )) * 100 )))   ) << endl;
		#endif
		
		
		// Make it better
		if(*this->startTime==0.0f) {
			#ifdef DEBUG3
				cout << "RESETING TIME" << endl;
			#endif
			*this->startTime   = x3d::my_x3d->posRender->render.seconds;
			*this->startTimeNS = x3d::my_x3d->posRender->render.nseconds;
		}

		*this->fraction_changed = 
						(float)((   ((x3d::my_x3d->posRender->render.seconds - *this->startTime) * 100) + 
						  ( ((double)(x3d::my_x3d->posRender->render.nseconds)) - ((double)(*this->startTimeNS)) ) ) /
						( 100 * (double) (*this->cycleInterval) ) );
		
		*this->fraction_changed = *this->fraction_changed - (((int)(*this->fraction_changed)));
		
		*this->isActive=true;
		

	} else {
		
		#ifdef DEBUG2
			cout << "Not in loop" << endl;
			cout << "local time " << x3d::my_x3d->posRender->render.seconds << endl;
			cout << "startTime  " << *this->startTime << endl;
		#endif
		
		//if(!strcmp(this->DEF,"CTC1_TVis")) {
		//	cout << "startTime  " << *this->startTime;
		//	cout << " Address StartTime " << this->startTime << endl;
		//}
		
		if(	((double)((   ((x3d::my_x3d->posRender->render.seconds - *this->startTime) * 100) + 
				  ( ((double)(x3d::my_x3d->posRender->render.nseconds)) - ((double)(*this->startTimeNS)) ) ) /
			  	( 100  ) )) <= ((double) (*this->cycleInterval)) ) { 
		
			#ifdef DEBUG2
				cout << "inside cycleInterval" << endl;
			#endif
		
			*this->fraction_changed = 
						(float)((   ((x3d::my_x3d->posRender->render.seconds - *this->startTime) * 100) + 
						  ( ((double)(x3d::my_x3d->posRender->render.nseconds)) - ((double)(*this->startTimeNS)) ) ) /
						( 100 * (double) (*this->cycleInterval) ) );
			
							  
			*this->fraction_changed = *this->fraction_changed - (((int)(*this->fraction_changed)));
	
			*this->isActive=true;
							  
		} else {
			
			*this->isActive=false;
			
		}
		
	}
	
	if(*this->isActive) {
	
		if ( 	      *this->fraction_changed == 0.0 && 
			 		  ( ( ((float)(x3d::my_x3d->posRender->render.nseconds) + ( ((float)( (int) (x3d::my_x3d->posRender->render.seconds )) * 100 ))) >
					  ((float)(*this->startTimeNS) + ( ((float)( (int) (*this->startTime )) * 100 )))   ) ) ) {
	
				  *this->fraction_changed = 1.0f;
						  
				  *this->isActive=false;
		} else {
			//*this->fraction_changed = *this->fraction_changed;
		}
	
		for(f=0;f<countFraction_changedPointer;f++) {
			*fraction_changedPointer[f] = *this->fraction_changed;
		}
	
		
	}
	
	
	for(f=0;f<countFraction_changedPointer;f++) {
		*fraction_changedPointer[f] = *this->fraction_changed;
	}
	
	
	//#ifdef DEBUG3
		//cout << "DEF " << this->DEF;
		//cout << "  fraction changed " << *this->fraction_changed << endl;
	//#endif
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (TimeSensor)" << endl;	// Message about debuging
	#endif
	
}

TimeSensor *TimeSensor::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	TimeSensor *timeSensor;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((TimeSensor *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	timeSensor = new TimeSensor();
	timeSensor->read(node);
	
	return timeSensor;
}

void *TimeSensor::read(DOMNode *node) {
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReading (TimeSensor)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;							// temporary variable
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
		
	attributes = node->getAttributes ();
	for (f = 0; f < attributes->getLength(); f++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(f)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(f)->getNodeValue()));
		} 
	}
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tfrom Node" << endl;	// Message about debuging
	#endif
	
	unsigned int f2;
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"fraction_changed")) {
				#ifdef DEBUG1
					cout << "Setting fraction_changed" << endl;
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
					(*x3d::my_x3d->routeTable[f]->field2) = new SFFloat;
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
				
				fraction_changedPointer[countFraction_changedPointer] = (SFFloat (*))(*x3d::my_x3d->routeTable[f]->field2);
				countFraction_changedPointer++;
				
				
				
			} else {
				if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"isActive")) {
					if(isActive==NULL) {
						if((*x3d::my_x3d->routeTable[f]->field2)==NULL) { 
							(*x3d::my_x3d->routeTable[f]->field2) = new SFBool;
						}
						isActive = (SFBool *)(*x3d::my_x3d->routeTable[f]->field2);
					} else {
						(*x3d::my_x3d->routeTable[f]->field2) = isActive;
					}
				}			
			}
		}
	}
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tto Node" << endl;	// Message about debuging
	#endif
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"startTime")) {
			
				if(startTime == NULL) {
				
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->toNode,x3d::my_x3d->routeTable[f]->toNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->toField,x3d::my_x3d->routeTable[f]->toField)) {
								if( ((*x3d::my_x3d->routeTable[f2]->field2) != NULL) ) {
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
								if( ((*x3d::my_x3d->routeTable[f2]->field2) != NULL) && ((*x3d::my_x3d->routeTable[f2]->field2) != (*x3d::my_x3d->routeTable[f]->field2)) ) {
									cout << "Error of route ( startTime ) numbers : " << f << " and " << f2 << endl;
									cout << "<" << x3d::my_x3d->routeTable[f]->fromNode << "," << x3d::my_x3d->routeTable[f]->fromField <<
											"," << x3d::my_x3d->routeTable[f]->toNode << "," << x3d::my_x3d->routeTable[f]->toField << ">" << endl;
									cout << "And" << endl;
									cout << "<" << x3d::my_x3d->routeTable[f2]->fromNode << "," << x3d::my_x3d->routeTable[f2]->fromField <<
											"," << x3d::my_x3d->routeTable[f2]->toNode << "," << x3d::my_x3d->routeTable[f2]->toField << ">" << endl;
								}
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
	
					}
					startTime = (SFTime *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					//(SFTime *)(*x3d::my_x3d->routeTable[f]->field2) = startTime;
					(*x3d::my_x3d->routeTable[f]->field2) = startTime;
				}
								
			}
		}
	}
	
	if(cycleInterval==NULL) {
		this->cycleInterval = new SFTime;
	}
	if(fraction_changed==NULL) {
		this->fraction_changed = new SFFloat;
		*this->fraction_changed=0.0f;
	}
	if(isActive==NULL) {
		this->isActive = new SFBool;
	}
	if(startTime==NULL) {
		this->startTime = new SFTime;
	}
	
	if(startTimeNS==NULL) {
		this->startTimeNS = new short int;
	}
	
	
	if(loop==NULL) {
		this->loop = new SFBool;
	}
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tsetting default values" << endl;	// Message about debuging
	#endif
	
	// initializing
	
	*this->isActive=false;
	*this->startTime=0.0f;
	//*this->startTime   = x3d::my_x3d->posRender->render.seconds;
	*this->startTimeNS=0;
	//*this->startTimeNS = x3d::my_x3d->posRender->render.nseconds;
	
	*this->cycleInterval=1.0f;
	//*this->loop = true; // na verdade é FALSE
	*this->loop = false;
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\treading data" << endl;	// Message about debuging
	#endif
	
	for (f = 0; f < attributes->getLength(); f++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(f)->getNodeName()) , "loop")) {
			if ( !strcasecmp(XMLString::transcode (attributes->item(f)->getNodeValue()) , "true")) 
				*this->loop = true;
			else
				*this->loop = false;
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(f)->getNodeName()) , "cycleInterval")) {
			*this->cycleInterval = atof(XMLString::transcode(attributes->item(f)->getNodeValue()));
			if(*this->cycleInterval<=0) {
				cerr << "Error cycleInterval must be bigger than 0" << endl;
			}
		}
	}

	
	#ifdef DEBUG2
		cout << "\t\t\t\tFinishing the Reading (TimeSensor)" << endl;	// Message about debuging
	#endif
	
	return(NULL);
	
}
