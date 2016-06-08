/*  Fazer */

#include "../math.h"

#include "x3d.h"
#include "x3dgprimitives.h"
#include "x3dShape.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


Scene::Scene() {
	
	this->countTimeSensor=0;
	this->countInterpolator=0;
	this->countLight=0;
	this->countShape=0;
	this->countTransform=0;
	this->countGroup=0;
	this->countHAnimHumanoid=0;
	this->countSwitch=0;
	this->countInline=0;
	this->countProtoDeclare=0;
	this->countBillboard=0;
	this->countCollision=0;
	this->countLOD=0;

	this->countAvatar=0;
	
	this->background=NULL;
	this->fog=NULL;
	
}


bool Scene::ray(float o[4],float d[4], float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (Scene)" << endl;	// Message about debuging
	#endif
	
	unsigned int f; // counter

	bool test=false;
	
	float mori[4][4];
	
	for(f=0;f<countLight;f++) {
		//this->light[f]->ray(o,d,m);
	}

	for(f=0;f<countShape;f++) {
		mcopy(m,mori);
		(this->shape[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countTransform;f++) {
		mcopy(m,mori);
		(this->transform[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	
	/*
	//-- This rotine follow each transform node --- //
	for(f=0;f<countAvatar;f++) {
		mcopy(m,mori);
		(this->avatar[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	*/
	
	//-- This rotine follow each group node --- //
	for(f=0;f<countGroup;f++) {
		mcopy(m,mori);
		(this->group[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}

	//-- This rotine follow each humanoid node --- //
	for(f=0;f<countHAnimHumanoid;f++) {
		//this->hAnimHumanoid[f]->ray(o,d,m);
	}

	//-- This rotine follow each switchnode node --- //
	for(f=0;f<countSwitch;f++) {
		//this->switchnode[f]->ray(o,d,m);
	}
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countBillboard;f++) {
	//	this->billboard[f]->ray(o,d,m);
	}

	
	//-- This rotine follow each Collision node --- //
	for(f=0;f<countCollision;f++) {
		mcopy(m,mori);
		(this->collision[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}

	
	//-- This rotine follow each LOD node --- //
	for(f=0;f<countLOD;f++) {
		mcopy(m,mori);
		(this->lod[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Casting Ray (Scene)" << endl;	// Message about debuging
	#endif
	
	return(test);
	
}

void Scene::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Scene)" << endl;	// Message about debuging
	#endif
		
	unsigned int f; // counter

	for(f=0;f<countTimeSensor;f++) {
		#ifdef DEBUG1
			cout << "Looking for timeSensor " << endl;  
		#endif
		this->timeSensor[f]->follow();
	}
	
	
	if(this->fog!=NULL) {
		this->fog->follow();
	}
	
	if(this->background!=NULL) {
		this->background->follow();
	}
	
	for(f=0;f<countInterpolator;f++) {
		this->interpolator[f]->follow(); 
	}

	for(f=0;f<countLight;f++) {
		this->light[f]->light(); 
	}

	for(f=0;f<countShape;f++) {
		this->shape[f]->follow();
	}
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countTransform;f++) {
		this->transform[f]->follow(); 
	}
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countAvatar;f++) {
		#ifdef DEBUG1
			cout << "Looking for avatar" << endl;  
		#endif
		this->avatar[f]->follow(); 
	}
	
	//-- This rotine follow each group node --- //
	for(f=0;f<countGroup;f++) {
		#ifdef DEBUG1
			cout << "Looking for group " << endl;  
		#endif
		this->group[f]->follow(); 
	}

	//-- This rotine follow each humanoid node --- //
	for(f=0;f<countHAnimHumanoid;f++) {
		#ifdef DEBUG1
			cout << "Looking for hAnimHumanoid " << endl;  
		#endif
		this->hAnimHumanoid[f]->follow(); 
	}

	//-- This rotine follow each switchnode node --- //
	for(f=0;f<countSwitch;f++) {
		#ifdef DEBUG1
			cout << "Looking for switch " << endl;  
		#endif
		this->switchnode[f]->follow(); 
	}
	
	//-- This rotine follow each switchnode node --- //
	for(f=0;f<countInline;f++) {
		#ifdef DEBUG1
			cout << "Looking for inline " << endl;  
		#endif
		this->inlinenode[f]->follow();
	}
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countBillboard;f++) {
		#ifdef DEBUG1
			cout << "Looking for billboard " << endl;  
		#endif
		this->billboard[f]->follow(); 
	}

	
	//-- This rotine follow each transform node --- //
	for(f=0;f<countCollision;f++) {
		#ifdef DEBUG1
			cout << "Looking for collision" << endl;  
		#endif
		this->collision[f]->follow(); 
	}

	
	//-- This rotine follow each LOD node --- //
	for(f=0;f<countLOD;f++) {
		#ifdef DEBUG1
			cout << "Looking for LOD" << endl;  
		#endif
		this->lod[f]->follow(); 
	}

	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Scene)" << endl;	// Message about debuging
	#endif
		
}

void Scene::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (Scene) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children
	
	children = node->getChildNodes();
	
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {
			#ifdef DEBUG1
				cout << "Looking for: " << XMLString::transcode(children->item(i)->getNodeName()) << endl;  
			#endif
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"NavigationInfo")) {
				this->navigationInfo = new NavigationInfo();
				this->navigationInfo->read(children->item(i)); 
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Billboard")) {
				if(this->countBillboard<MAX_NUM_TRANSFORM) {
					this->billboard[this->countBillboard] = Billboard::get(children->item(i));
					if(this->countBillboard > (MAX_NODES-1) ) {
						cerr << "Error max countBillboard nodes" << endl;
					} else {
						this->countBillboard++;
					}
				} else {
					cerr << "Maximum limite of billboard in a Group node " << endl;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"TimeSensor")) {
				this->timeSensor[this->countTimeSensor] = TimeSensor::get(children->item(i));
				//cout << " TimeSensor " << this->countTimeSensor << endl;
				if(this->countTimeSensor > (MAX_NODES-1) ) {
					cerr << "Error max countTimeSensor nodes" << endl;
				} else {
					this->countTimeSensor++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"CoordinateInterpolator")) {
				this->interpolator[this->countInterpolator] = CoordinateInterpolator::get(children->item(i));
				if(this->countInterpolator > (MAX_NODES-1) ) {
					cerr << "Error max countInterpolator nodes" << endl;
				} else {
					this->countInterpolator++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"ColorInterpolator")) {
				this->interpolator[this->countInterpolator] = ColorInterpolator::get(children->item(i));
				if(this->countInterpolator > (MAX_NODES-1) ) {
					cerr << "Error max countInterpolator nodes" << endl;
				} else {
					this->countInterpolator++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"PositionInterpolator")) {
				this->interpolator[this->countInterpolator] = PositionInterpolator::get(children->item(i));
				if(this->countInterpolator > (MAX_NODES-1) ) {
					cerr << "Error max countInterpolator nodes" << endl;
				} else {
					this->countInterpolator++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"OrientationInterpolator")) {
				this->interpolator[this->countInterpolator] = OrientationInterpolator::get(children->item(i));
				if(this->countInterpolator > (MAX_NODES-1) ) {
					cerr << "Error max countInterpolator nodes" << endl;
				} else {
					this->countInterpolator++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Background")) {
				this->background = Background::get(children->item(i));
			} else 
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Fog")) {
				this->fog = Fog::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"SpotLight")) {
				this->light[this->countLight] = SpotLight::get(children->item(i));
				if(this->countLight > (MAX_NODES-1) ) {
					cerr << "Error max countLight nodes" << endl;
				} else {
					this->countLight++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"PointLight")) {
				this->light[this->countLight] = PointLight::get(children->item(i));
				if(this->countLight > (MAX_NODES-1) ) {
					cerr << "Error max countLight nodes" << endl;
				} else {
					this->countLight++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"DirectionalLight")) {
				this->light[this->countLight] = DirectionalLight::get(children->item(i));
				if(this->countLight > (MAX_NODES-1) ) {
					cerr << "Error max countLight nodes" << endl;
				} else {
					this->countLight++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"WorldInfo")) {
				this->worldInfo = new WorldInfo();
				this->worldInfo->read(children->item(i)); 
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Shape")) {
				this->shape[this->countShape] = Shape::get(children->item(i));
				if(this->countShape > (MAX_NODES-1) ) {
					cerr << "Error max countShape nodes" << endl;
				} else {
					this->countShape++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Transform")) {
				this->transform[this->countTransform] = Transform::get(children->item(i));
				if(this->countTransform > (MAX_NODES-1) ) {
					cerr << "Error max countTransform nodes" << endl;
				} else {
					this->countTransform++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Avatar")) {
				this->avatar[this->countAvatar] = Avatar::get(children->item(i));
				if(this->countAvatar > (MAX_NODES-1) ) {
					cerr << "Error max countAvatar nodes" << endl;
				} else {
					this->countAvatar++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Group")) {
				this->group[this->countGroup] = Group::get(children->item(i));
				if(this->countGroup > (MAX_NODES-1) ) {
					cerr << "Error max countGroup nodes" << endl;
				} else {
					this->countGroup++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Collision")) {
				this->collision[this->countCollision] = Collision::get(children->item(i));
				if(this->countCollision > (MAX_NODES-1) ) {
					cerr << "Error max countCollision nodes" << endl;
				} else {
					this->countCollision++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"HAnimHumanoid")) {
				this->hAnimHumanoid[this->countHAnimHumanoid] = new HAnimHumanoid();
				this->hAnimHumanoid[this->countHAnimHumanoid]->read(children->item(i)); 
				if(this->countHAnimHumanoid > (MAX_NODES-1) ) {
					cerr << "Error max countHAnimHumanoid nodes" << endl;
				} else {
					this->countHAnimHumanoid++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Switch")) {
				this->switchnode[this->countSwitch] = Switch::get(children->item(i));
				if(this->countSwitch > (MAX_NODES-1) ) {
					cerr << "Error max countSwitch nodes" << endl;
				} else {
					this->countSwitch++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Inline")) {
				this->inlinenode[this->countInline] = Inline::get(children->item(i));
				if(this->countInline > (MAX_NODES-1) ) {
					cerr << "Error max countInline nodes" << endl;
				} else {
					this->countInline++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"LOD")) {
				this->lod[this->countLOD] = LOD::get(children->item(i));
				if(this->countLOD > (MAX_NODES-1) ) {
					cerr << "Error max countLOD nodes" << endl;
				} else {
					this->countLOD++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"ProtoDeclare")) {
				this->ProtoDeclarenode[this->countProtoDeclare] = ProtoDeclare::get(children->item(i));
				if(this->countProtoDeclare > (MAX_NODES-1) ) {
					cerr << "Error max countProtoDeclare nodes" << endl;
				} else {
					this->countProtoDeclare++;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Viewpoint")) {
				// to do
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Sound")) {
				// to do
			} else
			if ( (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#text")) && 
				 (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#comment")) && 
				 (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"ROUTE")) ) {
				cout << "Scene member not recognized : ";
				cout << XMLString::transcode(children->item(i)->getNodeName()) << endl;
			}
		
		}
	}

}


X3DBase::X3DBase() {
	
	scene = NULL;
	
}

bool X3DBase::ray(float o[4],float d[4], float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (X3DBase)" << endl;	// Message about debuging
	#endif
		
	bool test=false;
	
	if (scene!=NULL) {
		(scene->ray(o,d,m)?test=true:test=test); // nada inteligente
	}
	
	if(test) {
		//cout << "colidiu" << endl;
	}
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Casting Ray (X3DBase)" << endl;	// Message about debuging
	#endif
	
	return(test);
}

void X3DBase::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (X3D Base)" << endl;	// Message about debuging
	#endif
	
	if (scene!=NULL) {
		scene->follow();
	}
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (X3D Base)" << endl;	// Message about debuging
	#endif
				
}

void X3DBase::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (X3D Base) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children

	children = node->getChildNodes();

	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {		
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Scene")) {
				scene = new Scene();
				scene->read(children->item(i));
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"head")) {
				head = new Head();
				head->read(children->item(i));
			} else 
			if ( (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#text")) && 
				 (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#comment")) ) {
				cout << "Not recognized : ";
				cout << XMLString::transcode(children->item(i)->getNodeName()) << endl;
			}
		}
	}

}


X3DBase *x3d::X3D(DOMNode *node) {

	x3dbase=NULL;
	X3DBase *x3dbase = new X3DBase();					// this hold the X3D scene
	if(x3dbase==NULL) {
		cerr << "Error alocating x3dbase" << endl;
	}
	
	x3dbase->read(node);
	
	return(x3dbase);
	
}


Head::Head() {
	
		meta = NULL;
	
}

void Head::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (Head) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"meta")) {
				meta = new Meta();
				meta->read(children->item(i));
			}
		}
	}	
	
}

Meta::Meta() {
	
	strcpy(content,"");
	strcpy(name,"");
	
}

void Meta::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (Meta) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	cout << "\033[32m" ;
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "content")) {
			cout << XMLString::transcode(attributes->item(i)->getNodeValue()) << " = " ;
			strcpy(this->content,XMLString::transcode(attributes->item(i)->getNodeValue()));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "name")) {
			cout << XMLString::transcode(attributes->item(i)->getNodeValue()) << endl ;
			strcpy(name,XMLString::transcode(attributes->item(i)->getNodeValue()));
		} 
	}
	cout << "\033[30m" ;
}

void x3d::scheck(char *DEF, double pto[3][2]) {
}

datavector::datavector() {
	aloced=false;
}

datavector::~datavector() {
	if(aloced)
		delete[] data;
}

void datavector::aloc(int size) {
	this->size = size;
	//#ifdef JINX_DOUBLE
		this->data = new double[size];
	//#else
	//	this->data = new float[size];
	//#endif
	if(this->data==NULL) {
		cerr << "Memory alocation problem" << endl;
	}
	this->aloced=true;
}

int counterBiggerThan=0;

//#ifdef JINX_DOUBLE
double datavector::get(int pos) {
//#else
//float datavector::get(int pos) {
//#endif	
	if(pos>this->size) {
		//cerr << "Error asking for possition bigger than vector in datavector" << endl;
		if(counterBiggerThan==0) {
			cerr << ">";
			counterBiggerThan=100;
		} else {
			counterBiggerThan--;
		}
		return(0);
	}
	return(this->data[pos]);
}
