

#include "x3d.h"
#include "x3dInterpolation.h"
#include "../math.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>



OrientationInterpolator::OrientationInterpolator() {
	
	this->value_changed = NULL;	// the return value
		
	nkeys=0;

	set_fraction_tmp=-1; // avoid first step
	
	
	countValue_changedPointer=0;
	value_changedPointer = new SFRotation*[128];
	
}


bool OrientationInterpolator::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void OrientationInterpolator::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (OrientationInterpolator)" << endl;	// Message about debuging
	#endif

	
	int g=0;							// temporary variable
	unsigned short int f;

	if(set_fraction_tmp!=*set_fraction) {
		
		set_fraction_tmp=*set_fraction;
	
	if(*set_fraction <= key[0] ) {
		*value_changed[0]=keyValues[0][0];
		*value_changed[1]=keyValues[1][0];
		*value_changed[2]=keyValues[2][0];
		*value_changed[3]=180*keyValues[3][0]/PI;
	} else if(*set_fraction >= key[nkeys-1] ) {
		*value_changed[0]=keyValues[0][nkeys-1];
		*value_changed[1]=keyValues[1][nkeys-1];
		*value_changed[2]=keyValues[2][nkeys-1];
		*value_changed[3]=180*keyValues[3][nkeys-1]/PI;
	}  else {
		g=0;
		while(*set_fraction > key[g]) {
			g++;
		}
		
		
		*value_changed[0]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[0][g]-keyValues[0][g-1]))+keyValues[0][g-1];
		*value_changed[1]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[1][g]-keyValues[1][g-1]))+keyValues[1][g-1];
		*value_changed[2]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[2][g]-keyValues[2][g-1]))+keyValues[2][g-1];
		*value_changed[3]=180* ((((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[3][g]-keyValues[3][g-1]))+keyValues[3][g-1]) /PI;
	
		
		//The results are undefined if the two orientations are diagonally opposite
		if(  (keyValues[0][g]!=0) && (keyValues[0][g-1]!=0) && ((keyValues[0][g])==(-keyValues[0][g-1])) ) {
			*value_changed[0]=keyValues[0][g-1];
			if( fabs(keyValues[3][g] - keyValues[3][g-1]) > (PI/2) )
			*value_changed[3]=180* ((((*set_fraction-key[g-1])/(key[g]-key[g-1]))*( ((2*PI)-keyValues[3][g]) -  (keyValues[3][g-1]) ))+ (keyValues[3][g-1])    ) /PI;
		}
		if(  (keyValues[1][g]!=0) && (keyValues[1][g-1]!=0) && ((keyValues[1][g])==(-keyValues[1][g-1])) ) {
			*value_changed[1]=keyValues[1][g-1];
			if( fabs(keyValues[3][g] - keyValues[3][g-1]) > (PI/2) )
			*value_changed[3]=180* ((((*set_fraction-key[g-1])/(key[g]-key[g-1]))*( ((2*PI)-keyValues[3][g]) -  (keyValues[3][g-1]) ))+ (keyValues[3][g-1])    ) /PI;
		}
		if(  (keyValues[2][g]!=0) && (keyValues[2][g-1]!=0) && ((keyValues[2][g])==(-keyValues[2][g-1])) ) {
			*value_changed[2]=keyValues[2][g-1];
			if( fabs(keyValues[3][g] - keyValues[3][g-1]) > (PI/2) )
			*value_changed[3]=180* ((((*set_fraction-key[g-1])/(key[g]-key[g-1]))*( ((2*PI)-keyValues[3][g]) -  (keyValues[3][g-1]) ))+ (keyValues[3][g-1])    ) /PI;
		}
		
	}

	
		for(f=0;f<countValue_changedPointer;f++) {
			//cout << "Changing value = " << f << endl;
			*value_changedPointer[f][0] = *this->value_changed[0];
			*value_changedPointer[f][1] = *this->value_changed[1];
			*value_changedPointer[f][2] = *this->value_changed[2];
			*value_changedPointer[f][3] = *this->value_changed[3];
		}
	
	}
	
	#ifdef DEBUG1
		cout << "Values of Orientation Interpolator : ";
		cout << *value_changed[0] << ", ";	
		cout << *value_changed[1] << ", ";
		cout << *value_changed[2] << ", ";
		cout << (*value_changed[3]*PI)/180 << endl;
	#endif
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (OrientationInterpolator)" << endl;	// Message about debuging
	#endif
	
}

OrientationInterpolator *OrientationInterpolator::get(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	OrientationInterpolator *orientationInterpolator;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((OrientationInterpolator *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	orientationInterpolator = new OrientationInterpolator();
	orientationInterpolator->read(node);
	
	return orientationInterpolator;

}

void *OrientationInterpolator::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (OrientationInterpolator) " << endl;  
	#endif
	
	
	char *ctemp;					// temporary variable
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	attributes = node->getAttributes ();

	
	#ifdef DEBUG1
		cout << " Getting DEF -> ";  
	#endif

	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

		
	#ifdef DEBUG1
		cout << this->DEF << endl;  
	#endif

	
	#ifdef DEBUG1
		cout << " Looking for routing" << endl;;  
	#endif

	unsigned int f2;							// temporary variable
	unsigned int f;							// temporary variable


	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"value_changed")) {
				#ifdef DEBUG1
					cout << "Setting value_changed" << endl;
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
					(*x3d::my_x3d->routeTable[f]->field2) = new SFRotation[4];
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
				
				
				value_changedPointer[countValue_changedPointer] = (SFRotation (*))(*x3d::my_x3d->routeTable[f]->field2);
				countValue_changedPointer++;
	
			}
		}
	}


					
	
	
	#ifdef DEBUG1
		cout << " Initializing values" << endl;;  
	#endif
	
	char *ptrptr;
	
	// initializing variables
	if(value_changed == NULL) {
		value_changed = new SFRotation[4];	// the return value
		*this->value_changed[0] = 0.0f;
		*this->value_changed[1] = 0.0f;
		*this->value_changed[2] = 0.0f;
		*this->value_changed[3] = 0.0f;
	}
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_fraction")) {
			//
		}
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "key")) {
			#ifdef DEBUG1
				cout << "Key : " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			#endif		
			ctemp = strtok_r(XMLString::transcode (attributes->item(i)->getNodeValue())," /n,",&ptrptr);
			while (ctemp != NULL) {
				key[nkeys++] = atof(ctemp);
				#ifdef DEBUG1
					cout << "(" << key[nkeys-1]	<< ")" << endl;
				#endif
				ctemp = strtok_r(NULL," /n,",&ptrptr);
				if(nkeys>=512) {
					cerr << "Not enouth memory for all keys in OrientationInterpolator" << endl;
					break;
				}
			}
		}
	}
	
	
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "keyValue")) {
			keyValues[0][0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," /n,"));
			keyValues[1][0] = atof(strtok(NULL," /n,"));
			keyValues[2][0] = atof(strtok(NULL," /n,"));
			keyValues[3][0] = atof(strtok(NULL," /n,"));
			//while(keyValues[3][0]>PI) {
			//	keyValues[3][0] = keyValues[3][0]-2*PI;
			//}
			//cout << "ROT = " << keyValues[3][0] << endl;
			for(f=1;f<nkeys;f++) {
				keyValues[0][f] = atof(strtok(NULL," /n,"));
				keyValues[1][f] = atof(strtok(NULL," /n,"));
				keyValues[2][f] = atof(strtok(NULL," /n,"));
				keyValues[3][f] = atof(strtok(NULL," /n,"));
				//while(keyValues[3][f]>PI) {
				//	keyValues[3][f] = keyValues[3][f]-2*PI;
				//}
				//cout << "ROT = " << keyValues[3][f] << endl;
			}
		}
	}
	
	set_fraction = NULL;

	#ifdef DEBUG1
		cout << " putting values in routeTable" << endl;  
	#endif
	
	
	
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_fraction")) {
				
				
				if(set_fraction == NULL) {
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
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					set_fraction = (SFFloat *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = set_fraction;
				}
				
				
			}
		}
	}
	
	
	if(set_fraction==NULL) {
		set_fraction = new float;
	}
	
	
	// Ini
	*set_fraction=0;			//* fraction variable that defines where is positioneted the values	

	set_fraction_tmp=-1;
	// this is not a good comparation if the Trans define (0,0,0), then is replaced
	if( (*this->value_changed[0] == 0.0f) && (*this->value_changed[1] == 0.0f) && (*this->value_changed[2] == 0.0f) && (*this->value_changed[3] == 0.0f) ) {
		follow();
	}
	set_fraction_tmp=0;
	
		
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (OrientationInterpolator)" << endl;	// Message about debuging
	#endif
	
	return(NULL);
	
}

// Constructor
PositionInterpolator::PositionInterpolator() {
	
	#ifdef DEGUB1
	cout << "Constructing (PositionInterpolator)" << endl;
	#endif
	
	this->value_changed = NULL;	// the return value
	
	this->nkeys=0;
	
	set_fraction_tmp=-1;
	
	countValue_changedPointer=0;
	value_changedPointer = new SFVec3f*[128];
	
	
}

bool PositionInterpolator::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void PositionInterpolator::follow() {

	int g=0;							// temporary variable
	unsigned short int f;
	
	if(set_fraction_tmp!=*set_fraction) {
		
		//cout << "TMP : " <<  set_fraction_tmp << endl;
		//cout << "OLD : " <<  *set_fraction << endl;
		set_fraction_tmp=*set_fraction;
	
	if(*set_fraction <= key[0] ) {
		*value_changed[0]=keyValues[0][0];
		*value_changed[1]=keyValues[1][0];
		*value_changed[2]=keyValues[2][0];
	} else 
	if(*set_fraction >= key[nkeys-1] ) {
		*value_changed[0]=keyValues[0][nkeys-1];
		*value_changed[1]=keyValues[1][nkeys-1];
		*value_changed[2]=keyValues[2][nkeys-1];
	}  else {
		g=0;
		while(*set_fraction > key[g]) {
			g++;
		}
		*value_changed[0]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[0][g]-keyValues[0][g-1]))+keyValues[0][g-1];
		*value_changed[1]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[1][g]-keyValues[1][g-1]))+keyValues[1][g-1];
		*value_changed[2]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[2][g]-keyValues[2][g-1]))+keyValues[2][g-1];
	}

	
		for(f=0;f<countValue_changedPointer;f++) {
			if(set[f]) {
				set[f]=false;
			} else {
				*value_changedPointer[f][0] = *this->value_changed[0];
				*value_changedPointer[f][1] = *this->value_changed[1];
				*value_changedPointer[f][2] = *this->value_changed[2];
			}
		}
	
	}
	
	#ifdef DEBUG
		cout << "Value of Interpolator : " << *set_fraction << endl;
		cout << "Translate Intr : " << *value_changed[0] << " , " << *value_changed[1] << " , " << *value_changed[2] << endl;
		cout << "Translate Intr addr : " << value_changed[0] << " , " << value_changed[1] << " , " << value_changed[2] << endl;
	#endif
	
}

PositionInterpolator *PositionInterpolator::get(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Geting (PositionInterpolator) " << endl;  
	#endif

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
		
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	
	PositionInterpolator *positionInterpolator = NULL;

	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			//cout << "DEF = " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			cout << "USE = " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			return((PositionInterpolator *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	#ifdef DEBUG1
		cout << "Creating a new PositionInterpolator" << endl;  
	#endif
	
	positionInterpolator = new PositionInterpolator();
	
	if(positionInterpolator==NULL) { 
		cerr << "Error allocing memory" << endl;
	} else {
	
		#ifdef DEBUG1
			cout << "Setting DEF" << endl;  
		#endif
	
	
		if(strcmp(DEF2,"")) {
			#ifdef DEBUG3
				cout << "DEF : " << DEF2 << endl;
			#endif
			strcpy(positionInterpolator->DEF,DEF2);
			setLink(positionInterpolator->DEF,positionInterpolator);			
		}
	
		#ifdef DEBUG1
			cout << "Ready to read (PositionInterpolator)" << endl;  
		#endif	
		positionInterpolator->read(node);
	}
	
	return positionInterpolator;

}


void *PositionInterpolator::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (PositionInterpolator) " << endl;  
	#endif


	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *ctemp;					// temporary variable
	
	#ifdef DEBUG1
		cout << "Looking for routing" << endl;;  
	#endif

	

	unsigned int f2;							// temporary variable
	unsigned int f;							// temporary variable
	
	
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"value_changed")) {
				#ifdef DEBUG1
					cout << "Setting value_changed" << endl;
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
					(*x3d::my_x3d->routeTable[f]->field2) = new SFVec3f[3];
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
				
				set[countValue_changedPointer] = x3d::my_x3d->routeTable[f]->set;
				value_changedPointer[countValue_changedPointer] = (SFVec3f (*))(*x3d::my_x3d->routeTable[f]->field2);
				countValue_changedPointer++;
	
			}
		}
	}

	
	
	#ifdef DEBUG1
		cout << "Initializing values" << endl;;  
	#endif
	
	// initializing variables
	
	if(this->value_changed==NULL) {
		this->value_changed = new SFVec3f[3];	// the return value
		*this->value_changed[0] = 0.0f;
		*this->value_changed[1] = 0.0f;
		*this->value_changed[2] = 0.0f;
	}
	
	
	#ifdef DEBUG1
		cout << "Reading data" << endl;;  
	#endif
		
	char *ptrptr;
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		//cout << "AQUI !!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_fraction")) {
			//
		}
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "key")) {
			#ifdef DEBUG1
				cout << "Key : " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			#endif		
			ctemp = strtok_r(XMLString::transcode (attributes->item(i)->getNodeValue())," /n,",&ptrptr);
			while (ctemp != NULL) {
				key[nkeys++] = atof(ctemp);
				#ifdef DEBUG1
					cout << "(" << key[nkeys-1]	<< ")" << endl;
				#endif
				ctemp = strtok_r(NULL," /n,",&ptrptr);
				if(nkeys>=512) {
					cerr << "Not enouth memory for all keys in OrientationInterpolator" << endl;
					break;
				}
			}
		}
	}
	
	
	
	#ifdef DEBUG1
		cout << "Reading keyValues" << endl;;  
	#endif
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "keyValue")) {
			keyValues[0][0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,\n"));
			keyValues[1][0] = atof(strtok(NULL," ,\n"));
			keyValues[2][0] = atof(strtok(NULL," ,\n"));
			for(f=1;f<nkeys;f++) {
				keyValues[0][f] = atof(strtok(NULL," ,\n"));
				keyValues[1][f] = atof(strtok(NULL," ,\n"));
				keyValues[2][f] = atof(strtok(NULL," ,\n"));
			}
		}
	}

	set_fraction = NULL;

	
	
	#ifdef DEBUG1
		cout << " putting values in routeTable" << endl;  
	#endif
	
	
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_fraction")) {
				
				if(set_fraction == NULL) {
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
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					set_fraction = (SFFloat *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = set_fraction;
				}
				
				

			}
		}
	}
	if(set_fraction==NULL) {
		set_fraction = new float;
	}

	*set_fraction=0;			//* fraction variable that defines where is positioneted the values	

	set_fraction_tmp=-1;
	
	#ifdef DEBUG1
		cout << "Returning (PositionInterpolator) " << endl;  
	#endif
	
	return(NULL);	

}

CoordinateInterpolator::CoordinateInterpolator() {
	
	this->nkeys=0;	//* the keys and the number of components
	this->nvalues=0;
	
	this->value_changed=NULL;
	
	set_fraction_tmp=-1; // avoid first step
	
}


bool CoordinateInterpolator::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void CoordinateInterpolator::follow() {

	unsigned int g=0;
	unsigned int h=0;							// temporary variable
	
	
		if(set_fraction_tmp!=*set_fraction) {
		
		//cout << "TMP : " <<  set_fraction_tmp << endl;
		//cout << "OLD : " <<  *set_fraction << endl;
		set_fraction_tmp=*set_fraction;

	
	for(h=0;h<nvalues/nkeys;h++) {
		if(*set_fraction <= key[0] ) {
			value_changed[(3*h)+0]=keyValues[0][h];
			value_changed[(3*h)+1]=keyValues[1][h];
			value_changed[(3*h)+2]=keyValues[2][h];
		} else 
		if(*set_fraction >= key[nkeys-1] ) {
			value_changed[(3*h)+0]=keyValues[0][((nkeys-1)*(nvalues/nkeys))+h];
			value_changed[(3*h)+1]=keyValues[1][((nkeys-1)*(nvalues/nkeys))+h];
			value_changed[(3*h)+2]=keyValues[2][((nkeys-1)*(nvalues/nkeys))+h];
		}  else {
			g=0;
			while(*set_fraction > key[g]) {
				g++;
			}
			value_changed[(3*h)+0]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[0][(g*(nvalues/nkeys))+h]-keyValues[0][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[0][((g-1)*(nvalues/nkeys))+h];
			value_changed[(3*h)+1]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[1][(g*(nvalues/nkeys))+h]-keyValues[1][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[1][((g-1)*(nvalues/nkeys))+h];
			value_changed[(3*h)+2]=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[2][(g*(nvalues/nkeys))+h]-keyValues[2][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[2][((g-1)*(nvalues/nkeys))+h];
		}
		
					
	}
	
	}
		
}

CoordinateInterpolator *CoordinateInterpolator::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	CoordinateInterpolator *coordinateInterpolator;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((CoordinateInterpolator *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	coordinateInterpolator = new CoordinateInterpolator();
	coordinateInterpolator->read(node);
	
	return coordinateInterpolator;
	
}

void *CoordinateInterpolator::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (CoordinateInterpolator) " << endl;  
	#endif
	
	unsigned int f; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	char *ctemp;					//* temporary variable
	attributes = node->getAttributes ();
	

	for (f = 0; f < attributes->getLength(); f++) {		
		if (!strcmp(XMLString::transcode(attributes->item(f)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(f)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(f)->getNodeValue()),this);
		}
 	}

	for (f = 0; f < attributes->getLength(); f++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(f)->getNodeName()) , "set_fraction")) {
			//
		}
		if ( !strcmp(XMLString::transcode(attributes->item(f)->getNodeName()) , "key")) {
			ctemp = strtok(XMLString::transcode(attributes->item(f)->getNodeValue())," \n,");
			while (ctemp != NULL) {
				key[nkeys++] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				if(nkeys>=512) {
					cerr << "Not enouth memory for all keys in CoordinateInterpolator" << endl;
					break;
				}
			}
		}
	}

	#ifdef DEBUG1
		cout << "Getting the key Values" << endl;  
	#endif
	
	
	for (f = 0; f < attributes->getLength(); f++) {
		if ( !strcmp(XMLString::transcode(attributes->item(f)->getNodeName()) , "keyValue")) {
			keyValues[0][0] = atof(strtok(XMLString::transcode (attributes->item(f)->getNodeValue())," \n,"));
			keyValues[1][0] = atof(strtok(NULL," \n,"));
			keyValues[2][0] = atof(strtok(NULL," \n,"));
			nvalues=1;
			#ifdef DEBUG1
				cout << "Values : " << keyValues[0][nvalues-1] << " , " << keyValues[1][nvalues-1] << " , " << keyValues[2][nvalues-1] << endl;
			#endif
			ctemp = strtok(NULL," \n,");
			while (ctemp != NULL) {
				keyValues[0][nvalues] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				keyValues[1][nvalues] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				keyValues[2][nvalues++] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				#ifdef DEBUG1
					cout << "Values : " << keyValues[0][nvalues-1] << " , " << keyValues[1][nvalues-1] << " , " << keyValues[2][nvalues-1] << endl;
				#endif
			}
		}
	}
	
	

	unsigned int f2;							// temporary variable
	
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"value_changed")) {
				if(value_changed == NULL) {
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
						(*x3d::my_x3d->routeTable[f]->field2) = new float[3*(nvalues/nkeys)];
					}
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->toNode,x3d::my_x3d->routeTable[f]->toNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->toField,x3d::my_x3d->routeTable[f]->toField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					value_changed = (float (*))(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = value_changed;
				}
			}
		}
	}
	
	
	if(value_changed==NULL) { 
		#ifdef DEBUG1
			cout << "Allocing memory" << endl;  
		#endif
		value_changed = new float[3*(nvalues/nkeys)];
		if(value_changed==NULL) { 
			cerr << "Error allocing memory for CoordinateInterpolator" << endl;
		}
	}

	set_fraction = NULL;

	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_fraction")) {
				
				
				if(set_fraction == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
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
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					set_fraction = (SFFloat *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = set_fraction;
				}
				
				
				
				
			}
		}
	}
	if(set_fraction==NULL) {
		set_fraction = new float;
	}
	
	
	// Initializing
	*set_fraction=0;			//* fraction variable that defines where is positioneted the values	
	
	
	#ifdef DEBUG1
		cout << "Returning (CoordinateInterpolator) " << endl;  
	#endif
	
	return(NULL);
	
}

ColorInterpolator::ColorInterpolator() {
	
	nkeys=0;	//* the keys and the number of components
	nvalues=0;
	
	set_fraction_tmp=-1; // avoid first step
	//this->value_changed = NULL;	// the return value
	
}


bool ColorInterpolator::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void ColorInterpolator::follow() {

	float value_changed[256][3];	// the return value
	
	unsigned int g=0;		// temporary variable
	unsigned int h=0;		// temporary variable
	
	//	if(set_fraction_tmp!=*set_fraction) {
		
		//cout << "TMP : " <<  set_fraction_tmp << endl;
		//cout << "OLD : " <<  *set_fraction << endl;
	//set_fraction_tmp=*set_fraction;

	
	float set_fraction=0;			//* fraction variable that defines where is positioneted the values
	
	for(h=0;h<nvalues/nkeys;h++) {
		if(set_fraction <= key[0] ) {
			value_changed[h][0]=keyValues[0][h];
			value_changed[h][1]=keyValues[1][h];
			value_changed[h][2]=keyValues[2][h];
		} else 
		if(set_fraction >= key[nkeys-1] ) {
			value_changed[h][0]=keyValues[0][((nkeys-1)*(nvalues/nkeys))+h];
			value_changed[h][1]=keyValues[1][((nkeys-1)*(nvalues/nkeys))+h];
			value_changed[h][2]=keyValues[2][((nkeys-1)*(nvalues/nkeys))+h];
		}  else {
			g=0;
			while(set_fraction > key[g]) {
				g++;
			}
			value_changed[h][0]=(((set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[0][(g*(nvalues/nkeys))+h]-keyValues[0][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[0][((g-1)*(nvalues/nkeys))+h];
			value_changed[h][1]=(((set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[1][(g*(nvalues/nkeys))+h]-keyValues[1][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[1][((g-1)*(nvalues/nkeys))+h];
			value_changed[h][2]=(((set_fraction-key[g-1])/(key[g]-key[g-1]))*			
									(keyValues[2][(g*(nvalues/nkeys))+h]-keyValues[2][((g-1)*(nvalues/nkeys))+h]))+
									keyValues[2][((g-1)*(nvalues/nkeys))+h];
		}
				
	}
	
	//}
	
}

ColorInterpolator *ColorInterpolator::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	ColorInterpolator *colorInterpolator;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((ColorInterpolator *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	colorInterpolator = new ColorInterpolator();
	colorInterpolator->read(node);
	
	return colorInterpolator;
	
}

void *ColorInterpolator::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (ColorInterpolator) " << endl;  
	#endif
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes

	char *ctemp;					// temporary variable
	
	attributes = node->getAttributes ();



	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		}
 	}

	
	unsigned int f2;						// temporary variable
	unsigned int f;							// temporary variable
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"value_changed")) {
				//if(value_changed == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
							(*x3d::my_x3d->routeTable[f]->field2) = new float[3*(nvalues/nkeys)];
						}
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->toNode,x3d::my_x3d->routeTable[f]->toNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->toField,x3d::my_x3d->routeTable[f]->toField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					//delete value_changed;
					//value_changed = (float (*))(*x3d::my_x3d->routeTable[f]->field2);
				//} else {
					//(*x3d::my_x3d->routeTable[f]->field2) = value_changed;
				//}
			}
		}
	}
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_fraction")) {
			//
		}
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "key")) {
			ctemp = strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," \n,");
			while (ctemp != NULL) {
				key[nkeys++] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				if(nkeys>=512) {
					cerr << "Not enouth memory for all keys in ColorInterpolator" << endl;
					break;
				}
			}
		}
	}

	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "keyValue")) {
			keyValues[0][0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," \n,"));
			keyValues[1][0] = atof(strtok(NULL," \n,"));
			keyValues[2][0] = atof(strtok(NULL," \n,"));
			nvalues=1;
			ctemp = strtok(NULL," \n,");
			while (ctemp != NULL) {
				keyValues[0][nvalues] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				keyValues[1][nvalues] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
				keyValues[2][nvalues++] = atof(ctemp);
				ctemp = strtok(NULL," \n,");
			}
		}
	}

	
	#ifdef DEBUG1
		cout << "Returning (ColorInterpolator) " << endl;  
	#endif
	
	return(NULL);
	
}





















// Constructor
ScalarInterpolator::ScalarInterpolator() {
	
	#ifdef DEGUB1
	cout << "Constructing (ScalarInterpolator)" << endl;
	#endif
	
	//this->value_changed = new SFFloat;	// the return value
	this->value_changed = NULL;	// the return value
	this->set_fraction = NULL;
	
	this->nkeys=0;
	
	set_fraction_tmp=-1; // avoid first step
	
	countValue_changedPointer=0;
	value_changedPointer = new SFFloat*[128];
	
}

bool ScalarInterpolator::ray(float o[4],float d[4],float m[4][4]) { 
	return false;
}

void ScalarInterpolator::follow() {

	int g=0;							// temporary variable
	
	unsigned short int f;
	
	if(set_fraction_tmp!=*set_fraction) {
		
		//cout << " ScalarInterpolator !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		
		//cout << "TMP : " <<  set_fraction_tmp << endl;
		//cout << "OLD : " <<  *set_fraction << endl;
		set_fraction_tmp=*set_fraction;
	
		if(*set_fraction <= key[0] ) {
			*value_changed=keyValues[0];
		} else 
		if(*set_fraction >= key[nkeys-1] ) {
			*value_changed=keyValues[nkeys-1];
		}  else {
			g=0;
			while(*set_fraction > key[g]) {
				g++;
			}
			*value_changed=(((*set_fraction-key[g-1])/(key[g]-key[g-1]))*(keyValues[g]-keyValues[g-1]))+keyValues[g-1];
		}
		
		for(f=0;f<countValue_changedPointer;f++) {
			//cout << "Changing value = " << f << endl;
			*value_changedPointer[f] = *this->value_changed;
		}
		
		
	}
	
	#ifdef DEBUG
		cout << "Value of Interpolator : " << *set_fraction << endl;
		cout << "Translate Intr : " << *value_changed[0] << " , " << *value_changed[1] << " , " << *value_changed[2] << endl;
		cout << "Translate Intr addr : " << value_changed[0] << " , " << value_changed[1] << " , " << value_changed[2] << endl;
	#endif
	
}

ScalarInterpolator *ScalarInterpolator::get(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Geting (ScalarInterpolator) " << endl;  
	#endif

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
		
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	
	ScalarInterpolator *positionInterpolator = NULL;

	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			cout << "USE = " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			return((ScalarInterpolator *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	#ifdef DEBUG1
		cout << "Creating a new PositionInterpolator" << endl;  
	#endif
	
	positionInterpolator = new ScalarInterpolator();
	
	if(positionInterpolator==NULL) { 
		cerr << "Error allocing memory" << endl;
	} else {
	
		#ifdef DEBUG1
			cout << "Setting DEF" << endl;  
		#endif
	
	
		if(strcmp(DEF2,"")) {
			#ifdef DEBUG3
				cout << "DEF : " << DEF2 << endl;
			#endif
			strcpy(positionInterpolator->DEF,DEF2);
			setLink(positionInterpolator->DEF,positionInterpolator);			
		}
	
		#ifdef DEBUG1
			cout << "Ready to read (ScalarInterpolator)" << endl;  
		#endif	
		positionInterpolator->read(node);
	}
	
	return positionInterpolator;

}


void *ScalarInterpolator::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (ScalarInterpolator) " << endl;  
	#endif


	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *ctemp;					// temporary variable
	
	#ifdef DEBUG1
		cout << "Looking for routing" << endl;;  
	#endif
	
	unsigned int f2;							// temporary variable
	unsigned int f;							// temporary variable

	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->fromField,"value_changed")) {
				#ifdef DEBUG1
					cout << "Setting value_changed" << endl;
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
				
				value_changedPointer[countValue_changedPointer] = (SFFloat (*))(*x3d::my_x3d->routeTable[f]->field2);
				countValue_changedPointer++;
	
			}
		}
	}
	

	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_fraction")) {
				if(set_fraction == NULL) {
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
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					set_fraction = (SFFloat *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = set_fraction;
				}
			}
		}
	}

	
	if(set_fraction==NULL) {
		set_fraction = new SFFloat;
		*set_fraction=0;			//* fraction variable that defines where is positioneted the values	
	}
	*set_fraction=0;			//* fraction variable that defines where is positioneted the values	

	
	#ifdef DEBUG1
		cout << "Initializing values" << endl;;  
	#endif
	
	// initializing variables
	if(this->value_changed == NULL) {
		this->value_changed = new SFFloat;	// the return value
		*this->value_changed = 0.0f;
	}
		
	#ifdef DEBUG1
		cout << "Reading data" << endl;;  
	#endif
		
	char *ptrptr;
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_fraction")) {
			//
		}
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "key")) {
			#ifdef DEBUG1
				cout << "Key : " << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl;
			#endif		
			ctemp = strtok_r(XMLString::transcode (attributes->item(i)->getNodeValue())," /n,",&ptrptr);
			while (ctemp != NULL) {
				key[nkeys++] = atof(ctemp);
				#ifdef DEBUG1
					cout << "(" << key[nkeys-1]	<< ")" << endl;
				#endif
				ctemp = strtok_r(NULL," /n,",&ptrptr);
				if(nkeys>=512) {
					cerr << "Not enouth memory for all keys in OrientationInterpolator" << endl;
					break;
				}
			}
		}
	}
	
	
	
	#ifdef DEBUG1
		cout << "Reading keyValues" << endl;;  
	#endif
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "keyValue")) {
			keyValues[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,\n"));
			for(f=1;f<nkeys;f++) {
				keyValues[f] = atof(strtok(NULL," ,\n"));
			}
		}
	}


	//set_fraction_tmp=-1;
	// this is not a good comparation if the Trans define (0,0,0), then is replaced
	//if( (*this->value_changed == 0.0f) ) {
	//	follow();
	//}
	set_fraction_tmp=0;

	
	#ifdef DEBUG1
		cout << "Returning (ScalarInterpolator) " << endl;  
	#endif
	
	return(NULL);	

}
