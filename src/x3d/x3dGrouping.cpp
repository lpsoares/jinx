
#include "x3dGrouping.h"
#include "x3dShape.h"
#include "x3dScripting.h"
#include "x3dEnvironmentalSensor.h"
#include "x3dPointingDeviceSensor.h"

#include "x3d.h"
#include "../math.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


using namespace std;


unsigned short int x3d::getChildren(DOMNodeList *nodeList, MFNode *children[MAX_NUM_TRANSFORM]) {
	
	unsigned int i; 				//* variable to counter
	unsigned short int countChildren = 0;
	
	if (nodeList != NULL) {
		for (i = 0; i < nodeList->getLength(); i++) {			
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"Inline")) {
				#ifdef DEBUG1
						cout << "Looking for (Inline) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Inline::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of Inline node " << endl;
				}
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"LOD")) {
				#ifdef DEBUG1
						cout << "Looking for (LOD) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = LOD::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of billboard in a Child node " << endl;
				}
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"Shape")) {
				#ifdef DEBUG1
						cout << "Looking for (Shape) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Shape::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of shapes in a Child node " << endl;
				}
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"Transform")) {
				#ifdef DEBUG1
						cout << "Looking for (Transform) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Transform::get(nodeList->item(i));
					countChildren++;
				}else {
					cerr << "Maximum limite of transforms in a Child node " << endl;
				}
			} else
			if (!strcmp(XMLString::transcode(nodeList->item(i)->getNodeName()),"Group")) {
				#ifdef DEBUG1
						cout << "Looking for (Group) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Group::get(nodeList->item(i));
					countChildren++;
				}else {
					cerr << "Maximum limite of groups in a Child node " << endl;
				}
			} else
			if (!strcmp(XMLString::transcode(nodeList->item(i)->getNodeName()),"Anchor")) {
				#ifdef DEBUG1
						cout << "Looking for (Anchor) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Anchor::get(nodeList->item(i));
					countChildren++;
				}else {
					cerr << "Maximum limite of anchors in a Child node " << endl;
				}
			} else
			if (!strcmp(XMLString::transcode(nodeList->item(i)->getNodeName()),"HAnimHumanoid")) {
				#ifdef DEBUG1
						cout << "Looking for (HAnimHumanoid) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					//this->children[this->countChildren] = HAnimHumanoid::get(children->item(i));
					countChildren++;
				}else {
					cerr << "Maximum limite of Humanoid in a Child node " << endl;
				}
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"Billboard")) {
				#ifdef DEBUG1
						cout << "Looking for (Billboard) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Billboard::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of billboard in a Child node " << endl;
				}
			} else

			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"Script")) {
				#ifdef DEBUG1
						cout << "Looking for (Script) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = Script::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of Script in a Child node " << endl;
				}
			} else

			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"ProximitySensor")) {
				#ifdef DEBUG1
						cout << "Looking for (ProximitySensor) " << endl;  
				#endif
				if(countChildren<MAX_NUM_TRANSFORM) {
					children[countChildren] = ProximitySensor::get(nodeList->item(i)); 
					countChildren++;
				} else {
					cerr << "Maximum limite of ProximitySensor in a Child node " << endl;
				}
			} else
			
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"TimeSensor")) {
				#ifdef DEBUG1
						cout << "Looking for (TimeSensor) " << endl;  
				#endif
				children[countChildren] = TimeSensor::get(nodeList->item(i));
				countChildren++;
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"CoordinateInterpolator")) {
				#ifdef DEBUG1
						cout << "Looking for (CoordinateInterpolator) " << endl;  
				#endif
				children[countChildren] = CoordinateInterpolator::get(nodeList->item(i));
				countChildren++;
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"ColorInterpolator")) {
				#ifdef DEBUG1
						cout << "Looking for (ColorInterpolator) " << endl;  
				#endif
				children[countChildren] = ColorInterpolator::get(nodeList->item(i)); 
				countChildren++;
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"PositionInterpolator")) {
				#ifdef DEBUG1
						cout << "Looking for (PositionInterpolator) " << endl;  
				#endif
				children[countChildren] = PositionInterpolator::get(nodeList->item(i)); 
				countChildren++;
			} else
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"OrientationInterpolator")) {
				#ifdef DEBUG1
						cout << "Looking for (OrientationInterpolator) " << endl;  
				#endif
				children[countChildren] = OrientationInterpolator::get(nodeList->item(i)); 
				countChildren++;
			} else 
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"TouchSensor")) {
				#ifdef DEBUG1
						cout << "Looking for (TouchSensor) " << endl;  
				#endif
				children[countChildren] = TouchSensor::get(nodeList->item(i));
				countChildren++;
			} else 
			if (!strcmp (XMLString::transcode(nodeList->item(i)->getNodeName()),"ScalarInterpolator")) {
				#ifdef DEBUG1
						cout << "Looking for (ScalarInterpolator) " << endl;  
				#endif
				children[countChildren] = ScalarInterpolator::get(nodeList->item(i));
				countChildren++;
			}
		}
	}
	
	return(countChildren);
	
}




Billboard::Billboard() {

	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;

}


Billboard *Billboard::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Billboard *billboard = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			billboard = (Billboard *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( billboard->ami("Billboard") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(billboard);
			
		} 
	}
		
	
	// verificar se o USE ta retornando NULL
	
	billboard = new Billboard();
	if(billboard==NULL) { 
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(billboard->DEF,DEF2);
		setLink(billboard->DEF,billboard);			
	}
	
	billboard->read(node);
	
	return billboard;
	
}



void Billboard::follow() {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Billboard)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;
	
	//-- This rotine follow each children node --- //
	for(f=0;f<this->countChildren;f++) {
		this->children[f]->follow(); 
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Billboard)" << endl;	// Message about debuging
	#endif
	
}

void *Billboard::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode(attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		}
	}

	children = node->getChildNodes();

	this->countChildren = x3d::getChildren(children,this->children);
	
	return(NULL);
	
}

Group::Group() {
	
	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;
	
}

Group *Group::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Group *group = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			group = (Group *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( group->ami("Group") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(group);
			
		} 
	}
		
	group = new Group();
	if(group==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(group->DEF,DEF2);
		setLink(group->DEF,group);			
	}
	
	group->read(node);
	
	return group;
	
}


void *Group::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (Group) " << endl;  
	#endif
	
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "bboxSize")) {
			//bboxSize[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			//bboxSize[1] = atof(strtok(NULL," "));
			//bboxSize[2] = atof(strtok(NULL," "));
		}
	}
		
	x3d::pushCast(&this->cast);  // put the address of cast in a stack to be used by a touchSensor
	
	children = node->getChildNodes ();

	this->countChildren = x3d::getChildren(children,this->children);
	
	x3d::popCast();  			// remove the address of cast in a stack to be used by a touchSensor
	
	#ifdef DEBUG1
		cout << "Returning (Group) " << endl;  
	#endif
	
	
	return(NULL);
	
}


bool Group::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG
		cout << "Group Ray routine " << endl;
	#endif
	
	unsigned int f=0;
	
	bool faz=true;

	bool test=false;
	
	float mori[4][4];
	
	for(int h=0; h<x3d::my_x3d->menuCount; h++) {
		if(!strcmp(x3d::my_x3d->menuName[h],DEF)) {
			faz=x3d::my_x3d->groupOnOff[h];
		}
	}
			
	if(faz) {
		
		for(f=0;f<this->countChildren;f++) {
			mcopy(m,mori);
			(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
		}
	
	}
	
	#ifdef DEBUG2
		if(test) { 
			cout << "OBJECT TOUCHED" << endl;
		}
	#endif
	
	this->cast=test;
	
	return(test);
	
}

void Group::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Group)" << endl;	// Message about debuging
	#endif
	
	#ifdef DEBUG2
		if(this->cast) { 
			cout << "In Group TOUCHED in Address" << &this->cast << endl;
		}
	#endif
	
	unsigned int f=0;
		
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"bboxSize")) {
				//translate[0] = atof(strtok(routeTable[f]->fvalue," "));
				//translate[1] = atof(strtok(NULL," "));
				//translate[2] = atof(strtok(NULL," "));
			} 	
		}
	}
	
	bool faz=true;
	
	for(int h=0; h<x3d::my_x3d->menuCount; h++) {
		if(!strcmp(x3d::my_x3d->menuName[h],DEF)) {
			faz=x3d::my_x3d->groupOnOff[h];
		}
	}
			
	if(faz) {

		#ifdef DEBUG1
			cout << "\t\t\t\t\tLooking for children" << endl;	// Message about debuging
		#endif
		//-- This rotine follow each children node --- //
		for(f=0;f<this->countChildren;f++) {
			this->children[f]->follow(); 
		}
		
	}	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Group)" << endl;	// Message about debuging
	#endif
}


Transform::Transform() {

	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;

	this->rotation    = NULL;	//
	this->translation = NULL;		//
	this->scale       = NULL;		//
	
/*	
	this->rotation    = new SFRotation[4];	//
	this->translation = new SFVec3f[3];		//
	this->scale       = new SFVec3f[3];		//
*/
}


Transform *Transform::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Transform *transform=NULL;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			transform = (Transform *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			if( transform->ami("Transform") ) {
				cout << "USE of a different DEF type" << endl;
			}
			return(transform);
		} 
	}
		
	transform = new Transform();
	
	if(transform==NULL) { 	// checks if creation return null
		cerr << "Error on getting Transform" << endl;
	} else {
	
		if(strcmp(DEF2,"")) {
			#ifdef DEBUG3
				cout << "DEF : " << DEF2 << endl;
			#endif
			strcpy(transform->DEF,DEF2);
			setLink(transform->DEF,transform);			
		}
	
		transform->read(node);
	}
	
	return transform;
	
}

bool Transform::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG1
		cout << "Transform Ray Routine" << endl;  
	#endif
		
	bool test=false;
	
	unsigned int f=0;
		
	float m2[4][4];
	
	transmatrix(m2,*this->translation[0],*this->translation[1],*this->translation[2]);	multipleMatrix(m,m2);
	mcopy(m2,m);
		
	scalematrix(m2,*this->scale[0],*this->scale[1],*this->scale[2]);	multipleMatrix(m,m2);
	mcopy(m2,m);
	
	rotmatrix(m2,*this->rotation[3],*this->rotation[0],*this->rotation[1],*this->rotation[2]);	multipleMatrix(m,m2);
	mcopy(m2,m);
	
	float mori[4][4];
	
	for(f=0;f<this->countChildren;f++) {
		mcopy(m,mori);
		(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}

	this->cast=test;
	
	return(test);

}

void Transform::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Transform)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;
	f=0;
	
	if(!((*this->scale[0]==0)&&(*this->scale[1]==0)&&(*this->scale[2]==0))) {
	
		glPushMatrix();
	
		if(!((*this->translation[0]==0)&&(*this->translation[1]==0)&&(*this->translation[2]==0))) {
			glTranslated(*this->translation[0],*this->translation[1],*this->translation[2]);
		}
		
		if(!((*this->rotation[0]==0)&&(*this->rotation[1]==0)&&(*this->rotation[2]==0))) {
			glRotated(*this->rotation[3],*this->rotation[0],*this->rotation[1],*this->rotation[2]);
		}
		
		if(!((*this->scale[0]==1)&&(*this->scale[1]==1)&&(*this->scale[2]==1))) {
			glScaled(*this->scale[0],*this->scale[1],*this->scale[2]);
		}
	
		#ifdef DEBUG1
			cout << "\t\t\t\t\tLooking for children" << endl;	// Message about debuging
		#endif
		//-- This rotine follow each children node --- //
		for(f=0;f<this->countChildren;f++) {
			this->children[f]->follow(); 
		}
		
		glPopMatrix();
	
	}
	
}

// Reading Transform
void *Transform::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (Transform) " << endl;  
	#endif
	
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	
	x3d::pushCast(&this->cast);  // put the address of cast in a stack to be used by a touchSensor
	
	attributes = node->getAttributes();
	
	bool set=false;
	unsigned int f;
	unsigned int f2;
	for(f=0;f<routeStruct::routeCount;f++) {
		set=false;  // enable or disable chan in next from step
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if( (!strcmp(x3d::my_x3d->routeTable[f]->toField,"translation")) || (!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_translation")) ) {


				for (i = 0; i < attributes->getLength(); i++) {		
					if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "translation")) {
						if ( strcmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"")) {
							set=true;
						}
					} 
				}
				x3d::my_x3d->routeTable[f]->set = set;
				
				if(translation == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL ) ) {
									(*x3d::my_x3d->routeTable[f]->field2) = (*x3d::my_x3d->routeTable[f2]->field2);
									x3d::my_x3d->routeTable[f2]->set = set;
									break;
								}
							}
						}
					}
				
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						(*x3d::my_x3d->routeTable[f]->field2) = new SFVec3f[3];
					}
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
								x3d::my_x3d->routeTable[f2]->set = set;
							}
						}
					}
					translation = (SFVec3f (*))(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = translation;
				}
				
				
			} else 
			if( (!strcmp(x3d::my_x3d->routeTable[f]->toField,"rotation")) || (!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_rotation")) ) {

				for (i = 0; i < attributes->getLength(); i++) {		
					if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "rotation")) {
						if ( strcmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"")) {
							set=true;
						}
					} 
				}
				x3d::my_x3d->routeTable[f]->set = set;

				if(rotation == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL ) ) {
									(*x3d::my_x3d->routeTable[f]->field2) = (*x3d::my_x3d->routeTable[f2]->field2);
									x3d::my_x3d->routeTable[f2]->set = set;
									break;
								}
							}
						}
					}
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						(*x3d::my_x3d->routeTable[f]->field2) = new SFRotation[4];
					}
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
								x3d::my_x3d->routeTable[f2]->set = set;
							}
						}
					}
					rotation = (SFRotation (*))(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = rotation;
				}

				
			} else
			if( (!strcmp(x3d::my_x3d->routeTable[f]->toField,"scale")) || (!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_scale")) ) {
				
				for (i = 0; i < attributes->getLength(); i++) {		
					if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "scale")) {
						if ( strcmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"")) {
							set=true;
						}
					} 
				}
				x3d::my_x3d->routeTable[f]->set = set;
				
				if(scale == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL ) ) {
									(*x3d::my_x3d->routeTable[f]->field2) = (*x3d::my_x3d->routeTable[f2]->field2);
									x3d::my_x3d->routeTable[f2]->set = set;
									break;
								}
							}
						}
					}
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						(*x3d::my_x3d->routeTable[f]->field2) = new SFVec3f[3];
					}
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
								x3d::my_x3d->routeTable[f2]->set = set;
							}
						}
					}
					scale = (SFVec3f (*))(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = scale;
				}
				
			}
		}
	}
	
	// Initializing variables

	if(this->rotation==NULL) {
		this->rotation    = new SFRotation[4];	//
	}
	
	if(this->translation==NULL) {
		this->translation = new SFVec3f[3];		//
	}
	
	if(this->scale==NULL) {
		this->scale       = new SFVec3f[3];		//
	}


	
	*this->translation[0] = 0.0f;
	*this->translation[1] = 0.0f;
	*this->translation[2] = 0.0f;
	
	*this->scale[0] = 1.0f;
	*this->scale[1] = 1.0f;
	*this->scale[2] = 1.0f;
	
	*this->rotation[0] = 0.0f;
	*this->rotation[1] = 0.0f;
	*this->rotation[2] = 0.0f;
	*this->rotation[3] = 0.0f;
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "translation")) {
			*this->translation[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,"));
			*this->translation[1] = atof(strtok(NULL," ,"));
			*this->translation[2] = atof(strtok(NULL," ,"));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "rotation")) {
			*this->rotation[0] = (float)atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,"));
			*this->rotation[1] = (float)atof(strtok(NULL," ,"));
			*this->rotation[2] = (float)atof(strtok(NULL," ,"));
			*this->rotation[3] = (float)180*atof(strtok(NULL," ,"))/PI;
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "scale")) {
			*this->scale[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,"));
			*this->scale[1] = atof(strtok(NULL," ,"));
			*this->scale[2] = atof(strtok(NULL," ,"));
		}
	}
	


	children = node->getChildNodes();

	this->countChildren = x3d::getChildren(children,this->children);
	
	
	x3d::popCast();  			// remove the address of cast in a stack to be used by a touchSensor
	
	
	#ifdef DEBUG1
		cout << "Returning (Transform) " << endl;  
	#endif
	
	return(NULL);
}

// Constructor
WorldInfo::WorldInfo() {
	
}

void WorldInfo::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (WorldInfo) " << endl;  
	#endif
	
	unsigned short int i; 			//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
		
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "info")) {
			//cout << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl ; // fazer algo com isso
		} 
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "title")) {
			//cout << XMLString::transcode (attributes->item(i)->getNodeValue()) << endl ; // fazer algo com isso
		} 
	}

	#ifdef DEBUG1
		cout << "Returning (WorldInfo) " << endl;  
	#endif
	
}



Switch::Switch() {

	whichChoice=-1;
	
	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;

}


Switch *Switch::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Switch *switch1 = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			switch1 = (Switch *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( switch1->ami("Switch") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(switch1);
			
		} 
	}
		
	switch1 = new Switch();
	if(switch1==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(switch1->DEF,DEF2);
		setLink(switch1->DEF,switch1);			
	}
	
	switch1->read(node);
	
	return switch1;
	
}

void Switch::follow() {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Switch)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;
	
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"bboxSize")) {
				//translate[0] = atof(strtok(routeTable[f]->fvalue," "));
				//translate[1] = atof(strtok(NULL," "));
				//translate[2] = atof(strtok(NULL," "));
			} 	
		}
	}
	
	if( (this->whichChoice>=0) && (this->whichChoice<((short int)this->countChildren)) ) {
		this->children[f]->follow(); 
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Switch)" << endl;	// Message about debuging
	#endif
}

void *Switch::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (Switch) " << endl;  
	#endif
		
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "whichChoice")) {
			this->whichChoice = atoi(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else 
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "bboxSize")) {
			//translate[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			//translate[1] = atof(strtok(NULL," "));
			//translate[2] = atof(strtok(NULL," "));
		} else 
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
		     (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
				cout << "\033[31m" ;
				cout << "Switch member not recognized : ";
				cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
				cout << "\033[30m" ;
		}
	}
	
	children = node->getChildNodes ();

	this->countChildren = x3d::getChildren(children,this->children);
	
	return(NULL);
}


// Constructor
StaticGroup::StaticGroup() {
	
	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;

}

StaticGroup *StaticGroup::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	StaticGroup *group = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			group = (StaticGroup *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( group->ami("Group") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(group);
			
		} 
	}
		
	group = new StaticGroup();
	if(group==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(group->DEF,DEF2);
		setLink(group->DEF,group);			
	}
	
	group->read(node);
	
	return group;
	
}

bool StaticGroup::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tRay casting (StaticGroup)" << endl;	// Message about debuging
	#endif
	
	unsigned int f=0;
	
	bool faz=true;

	bool test=false;
	
	float mori[4][4];
	
	for(int h=0; h<x3d::my_x3d->menuCount; h++) {
		if(!strcmp(x3d::my_x3d->menuName[h],DEF)) {
			faz=x3d::my_x3d->groupOnOff[h];
		}
	}
			
	if(faz) {
		
		for(f=0;f<this->countChildren;f++) {
			mcopy(m,mori);
			(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
		}
	
	}
	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Ray casting (StaticGroup)" << endl;	// Message about debuging
	#endif
	
	return(test);
	
}

void StaticGroup::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Group)" << endl;	// Message about debuging
	#endif
	
	unsigned int f=0;
		
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"bboxSize")) {
				//translate[0] = atof(strtok(routeTable[f]->fvalue," "));
				//translate[1] = atof(strtok(NULL," "));
				//translate[2] = atof(strtok(NULL," "));
			} 	
		}
	}
	
	bool faz=true;
	
	for(int h=0; h<x3d::my_x3d->menuCount; h++) {
		if(!strcmp(x3d::my_x3d->menuName[h],DEF)) {
			faz=x3d::my_x3d->groupOnOff[h];
		}
	}
			
	#ifdef DEBUG1
		cout << "\t\t\t\t\tLooking for children" << endl;	// Message about debuging
	#endif
	//-- This rotine follow each children node --- //
	for(f=0;f<this->countChildren;f++) {
		this->children[f]->follow(); 
	}
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Group)" << endl;	// Message about debuging
	#endif
}

void *StaticGroup::read(DOMNode *node) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReading (Group)" << endl;	// Message about debuging
	#endif
	
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "bboxSize")) {
			//translate[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			//translate[1] = atof(strtok(NULL," "));
			//translate[2] = atof(strtok(NULL," "));
		}
	}
	
	children = node->getChildNodes();
	
	this->countChildren = x3d::getChildren(children,this->children);
	
	return(NULL);
	
}


	
	
	


Avatar::Avatar() {

	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;
		
	translate[0] = 0;
	translate[1] = 0;
	translate[2] = 0;
	
	scale[0] = 1;
	scale[1] = 1;
	scale[2] = 1;
	
	rotate[0] = 0;
	rotate[1] = 0;
	rotate[2] = 0;
	rotate[3] = 0;
	
}


Avatar *Avatar::get(DOMNode *node) {
	Avatar *transform = new Avatar();
	Avatar *transform2;
	
	transform2 = (Avatar *)transform->read(node);
	
	if(transform2!=NULL) {
		delete transform;
		return transform2;
	} 
	
	return transform;
	
}

bool Avatar::ray(float o[4],float d[4],float m[4][4]) {
	
	return(true);

}

void Avatar::follow() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Avatar)" << endl;	// Message about debuging
	#endif
	
	unsigned short int f=0;
	
	glPushMatrix();

	
	// Translate to the users position	
	glTranslated(-Render::my_rrr->posRender->render.pos[0],
				 -Render::my_rrr->posRender->render.pos[1],
				 -Render::my_rrr->posRender->render.pos[2]);


	#ifdef DEBUG3
	cout << "Pos : " << endl;
	cout << -Render::my_rrr->posRender->render.pos[0] << endl;
	cout << -Render::my_rrr->posRender->render.pos[1] << endl;
	cout << -Render::my_rrr->posRender->render.pos[2] << endl;
	#endif
	
	glRotated(Render::my_rrr->posRender->render.rot[1],0,1,0);
	
	glTranslated(this->translate[0],this->translate[1],this->translate[2]);
	glRotated(this->rotate[3],this->rotate[0],this->rotate[1],this->rotate[2]);
	glScaled(this->scale[0],this->scale[1],this->scale[2]);
	
	float matrix2[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix2);
				
	//-- This rotine follow each children node --- //
	for(f=0;f<this->countChildren;f++) {
		this->children[f]->follow(); 
	}

	glPopMatrix();

	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Avatar)" << endl;	// Message about debuging
	#endif

}

// Reading function
void *Avatar::read(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "translation")) {
			this->translate[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->translate[1] = atof(strtok(NULL," "));
			this->translate[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "rotation")) {
			this->rotate[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->rotate[1] = atof(strtok(NULL," "));
			this->rotate[2] = atof(strtok(NULL," "));
			this->rotate[3] = 180*atof(strtok(NULL," "))/PI;
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "scale")) {
			this->scale[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			this->scale[1] = atof(strtok(NULL," "));
			this->scale[2] = atof(strtok(NULL," "));
		}
	}
	
	children = node->getChildNodes ();
	this->countChildren = x3d::getChildren(children,this->children);
	
	
	return(NULL);
}
