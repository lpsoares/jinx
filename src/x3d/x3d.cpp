// JINX


// Jinx includes
#include "x3d.h"
#include "../math.h"
#include "x3dGeometry3D.h"
#include "../tesselator.h"

//--- C & C++ includes ---
#include <iostream>
#include <string>


// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

x3d *x3d::my_x3d = NULL;

unsigned int routeStruct::routeCount=0;

X3DValues *x3d::x3DValues = NULL;

routeStruct::routeStruct() {}

routeStruct::~routeStruct() {}

#ifndef XERCES
	char *x3d::message; // To hold memory for parsing
#endif
	



//#define MAX_ROUTE 2048			// Maximun number of values for the route table
#define MAX_ROUTE 2822			// Maximun number of values for the route table
	
// For Casting
bool *x3d::castStack[CAST_STACK_SIZE];			// the stack for casting addresses
unsigned int x3d::countStack=0;		// the counter for casting addresses
	
// put the cast address in memory
void  x3d::pushCast(bool *castAddress) {
	if(x3d::countStack>=(CAST_STACK_SIZE)) {
		cerr << "Error cast stack is in the maximum position" << endl;
	} else {
		x3d::castStack[x3d::countStack] = castAddress;
		x3d::countStack++;
	}
}

// pop the stack with cast address
void  x3d::popCast() {
	if(x3d::countStack<=0) {
		cerr << "Error cast stack is in the last position" << endl;
	} else {
		x3d::countStack--;
	}
}

// get the cast address
bool *x3d::getCast() {	// this is used in touchSensor
	
	if(x3d::countStack <= 0) {
		cerr << "Error no casting object in the cast stack" << endl;
		return(NULL);
	} else {
		return(x3d::castStack[x3d::countStack-1]);
	}
	
}
	
	
	
	
	
void x3d::xmlInitializing() {

	this->parser = NULL;
	this->parser = new XercesDOMParser; // the parser object is create
	if(this->parser==NULL) {
		cerr << "Error creating parser" << endl;
	}
	
	this->parser->setLoadExternalDTD(false); 									// because the DTD validation off
	this->parser->setValidationScheme (XercesDOMParser::Val_Never);
	this->parser->setDoNamespaces (false);
	this->parser->setDoSchema (false);
	this->parser->setValidationSchemaFullChecking (false);
	this->parser->setCreateEntityReferenceNodes (true);	
	
	this->errReporter = NULL;
	this->errReporter = new DOMTreeErrorReporter();
	if(this->errReporter==NULL) {
			cerr << "Error allocating memory for error reporter" << endl;
	}
	this->parser->setErrorHandler(errReporter);
		
}



/* Constructor */
x3d::x3d() {
	
	#ifndef XERCES
		x3d::message = new char[Size_X3D_Message];
	#endif

	unsigned short int f;	// counter
	
	xmlInitializing();
	
	this->texture_bmp = new Tex(); // Initializing the texture reader object
	
	this->x3dbase = NULL; // this hold all the X3D scene graph
	
	routeTable = new routeStruct*[MAX_ROUTE];  // allocing memory for pointers ( I Believe)
	for(f=0;f<MAX_ROUTE;f++) {
		routeTable[f] = new routeStruct();
		if( (routeTable[f]==0) || (routeTable[f]==NULL) ) {
			cerr << "Error alocing memory for route !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n";
		}
	}

	tesselator = new Tesselator();   // tesselator very used in IndexedFaceSet
	
	#ifdef DEBUG3
		GLdouble TessPropertyValue;
		gluGetTessProperty(this->tesselator->tess, GLU_TESS_BOUNDARY_ONLY, &TessPropertyValue);
		cout << "Value of GLU_TESS_BOUNDARY_ONLY is : ";
		if(TessPropertyValue==GL_TRUE) { cout << "True" << endl; }
		else { cout << "False" << endl; }
		gluGetTessProperty(this->tesselator->tess, GLU_TESS_WINDING_RULE, &TessPropertyValue);
		cout << "Value of GLU_TESS_WINDING_RULE is : ";
		if(TessPropertyValue==GLU_TESS_WINDING_ODD) 		{ cout << "GLU_TESS_WINDING_ODD" << endl; }
		if(TessPropertyValue==GLU_TESS_WINDING_NONZERO)		{ cout << "GLU_TESS_WINDING_NONZERO" << endl; }
		if(TessPropertyValue==GLU_TESS_WINDING_POSITIVE)	{ cout << "GLU_TESS_WINDING_POSITIVE" << endl; }
		if(TessPropertyValue==GLU_TESS_WINDING_NEGATIVE) 	{ cout << "GLU_TESS_WINDING_NEGATIVE" << endl; }
		if(TessPropertyValue==GLU_TESS_WINDING_ABS_GEQ_TWO)	{ cout << "GLU_TESS_WINDING_ABS_GEQ_TWO" << endl; }
		gluGetTessProperty(this->tesselator->tess, GLU_TESS_TOLERANCE, &TessPropertyValue);
		cout << "Value of GLU_TESS_TOLERANCE is : " << TessPropertyValue << endl;
		cout << endl << endl;
	#endif
	
	x3d::my_x3d = this;
	
}


/* Cleaning dinamic memory used */
x3d::~x3d() {

	delete tesselator; // deleting tesselator
		
	delete texture_bmp; // the texture object
	
	#ifndef XERCES
		delete[] x3d::message; // aparentemente pode estar travando
	#endif
	
	delete this->errReporter;
	delete this->parser;
}


// Load a x3d file and returns the xml document pointer
DOMDocument *x3d::load(char* xmlDocument) {
	
	// here the document is parsed and put in memory
	try { 
		this->parser->parse(xmlDocument);
	}
	catch (const XMLException &) 	{
		cerr << "An error during parsing X3D file : " << xmlDocument << endl;
	} 
	catch (const DOMException & e) 	{
		cerr << "An error during parsing X3D file : " << e.code << "  : " << xmlDocument << endl;
	}
	catch (...) {
		cerr << "An error during parsing X3D file : " << xmlDocument << endl;
	}

	return(this->parser->getDocument());
}

/* prepare the X3D tree in memory */
X3DBase *x3d::serialize() {


	x3d::my_x3d->route();
	#ifdef DEBUG
		cout << "routeStruct::routeCount : " << routeStruct::routeCount << endl;
	#endif
	
	
	X3DBase *x3dTMP;
	
	x3dTMP = serializeNode(this->parser->getDocument());
	
	return(x3dTMP);
	
}


// Serialization for graphics
X3DBase *x3d::serialize(DOMDocument *doc) {
	
	unsigned short int f;	// counter

	// Creates the routes
	x3d::my_x3d->route(doc);
	#ifdef DEBUG
		cout << "routeStruct::routeCount : " << routeStruct::routeCount << endl;
	#endif
	
	// Turning on all groups
	this->menuCount=0;
	for(f=0;f<groupOnOff_MAX;f++) {
		groupOnOff[f]=true; //turn on all groups
	}
		
	// Turning off all lights
	x3d::my_x3d->light=GL_LIGHT0;
	for(f=GL_LIGHT0;f < (GL_MAX_LIGHTS+GL_LIGHT0);f++) {
		#ifdef DEBUG
			cout << "Reseting lights" << endl;
		#endif
		glDisable(f);
	}
	
	return(serializeNode(doc));
	
}

/* Start to read the x3d data */
X3DBase *x3d::serializeNode(DOMNode *node) {
	
	DOMNodeList *nodes = NULL;
	X3DBase *tx3dbase = NULL;
	X3DBase *tmp = NULL;
	
	#ifdef DEBUG
		cout << "NODE : " << XMLString::transcode(node->getNodeName()) << endl;
	#endif
	unsigned int i; // generic counter
	
	switch (node->getNodeType()) //look for the kind of node type 
	{
	case (DOMNode::DOCUMENT_NODE):
		nodes = node->getChildNodes();
		if (nodes != NULL) {
			for (i = 0; i < nodes->getLength(); i++) {
				tmp = serializeNode(nodes->item(i));
				if(tmp!=NULL) {
					tx3dbase = tmp;
				}
			}
		}
		break;
	case (DOMNode::ELEMENT_NODE):
		if (!strcmp(XMLString::transcode(node->getNodeName()), "X3D")) {
			tx3dbase = X3D(node);
			return(tx3dbase);
		} else {
			cerr << "Fatal error X3D Group not found" << endl;
			return(NULL);
		}
		break;
	}
	
	return(tx3dbase);
}

/* draw the objects on screen */
int x3d::draw(PosRender *posRendert, int teye) {
	
	this->eye=teye;
	this->posRender = posRendert;

	#ifdef DEBUG2
		cout << "\t\t\t\tStarting to Draw in X3D routine" << endl;	// Message about debuging
	#endif
	x3dbase->follow();
	#ifdef DEBUG2
		cout << "\t\t\t\tFinishing to Draw in X3D routine" << endl;	// Message about debuging
	#endif
	return(1);
}


/* cast a ray to detec colision with objects */
void x3d::ray(float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "Beggining to ray cast in X3D" << endl;
	#endif
	
	float o[4];
	float d[4];
	
	o[0]=this->posRender->render.trans[0];
	o[1]=this->posRender->render.trans[1];
	o[2]=this->posRender->render.trans[2];
	o[3]=1;
	
	d[0]=0;
	d[1]=0;
	d[2]=1;
	d[3]=0;
			
	float matrix[4][4];
	
	#ifdef DEBUG2
		cout << "Ray Casting multiplication operation" << endl;
	#endif
	rotzmatrix(matrix,x3d::my_x3d->posRender->render.rotat[2]);
	multipleMatrix(matrix,d);
	
	rotymatrix(matrix,x3d::my_x3d->posRender->render.rotat[1]);
	multipleMatrix(matrix,d);
	
	rotxmatrix(matrix,(x3d::my_x3d->posRender->render.rotat[0]));
	multipleMatrix(matrix,d);

	#ifdef DEBUG2
		cout << "Looking for the first element in X3D" << endl;
	#endif
	x3dbase->ray(o,d,m);	
	
}


/* */
int x3d::viewpoint(Viewpoints *tviewpoints) {
	viewpoints=tviewpoints;			//
	serializeViewpoint(this->parser->getDocument());
	return Viewpoints::counter;		// return the number of viewpoints
}

void x3d::serializeViewpoint(DOMNode *node) {
		
	char* nodeName;
	char* valueName;
	char* value;
	DOMNodeList *nodes = NULL;
	switch (node->getNodeType()) {
	
	unsigned int i ;  // generic counter
		
	case (DOMNode::DOCUMENT_NODE):
		
		nodes = node->getChildNodes();
		if (nodes != NULL) {
			for (i = 0; i < nodes->getLength(); i++)
				serializeViewpoint(nodes->item(i));
		}
		break;
	
	case (DOMNode::ELEMENT_NODE):
		nodeName = XMLString::transcode(node->getNodeName ());
 
		if (!strcmp (nodeName, "Viewpoint")) {
			
			/* a fazer atualmente esta estatico
			//viewpoints[this->viewpointCount] = new Viewpoints();
			*/
			viewpoints[Viewpoints::counter].position[0]=0;	// default
			viewpoints[Viewpoints::counter].position[1]=0;	// default
			viewpoints[Viewpoints::counter].position[2]=-10; // default // EFECTIVE EM POSVIEW
	
			viewpoints[Viewpoints::counter].orientation[0]=0;
			viewpoints[Viewpoints::counter].orientation[1]=1;
			viewpoints[Viewpoints::counter].orientation[2]=0;
			viewpoints[Viewpoints::counter].orientation[3]=0;
			
			DOMNamedNodeMap *attributes;
			attributes = node->getAttributes();
			
			for (unsigned int ii = 0; ii < attributes->getLength(); ii++) {
				DOMNode *current;
				current = attributes->item (ii);
				valueName = XMLString::transcode (current->getNodeName ());
				value = XMLString::transcode (current->getNodeValue ());
								
				if (!strcmp (valueName, "position")) {
					viewpoints[Viewpoints::counter].position[0] = -atof(strtok(value," "));
					viewpoints[Viewpoints::counter].position[1] = -atof(strtok(NULL," "));
					viewpoints[Viewpoints::counter].position[2] = -atof(strtok(NULL," "));
				} if(!strcmp (valueName, "orientation")) {
					viewpoints[Viewpoints::counter].orientation[0] = atof(strtok(value," ")); // not implemented
					viewpoints[Viewpoints::counter].orientation[1] = atof(strtok(NULL," "));
					viewpoints[Viewpoints::counter].orientation[2] = atof(strtok(NULL," "));
					viewpoints[Viewpoints::counter].orientation[3] = 180*atof(strtok(NULL," "))/PI;
				} else if (!strcmp (valueName, "description")) {
					strcpy(viewpoints[Viewpoints::counter].description,value);
				} else if (!strcmp (valueName, "DEF")) {
					strcpy(viewpoints[Viewpoints::counter].DEF,value);
				}
			}
			
			#ifdef DEBUG
				cout << "Position " << viewpoints[Viewpoints::counter].position[0] <<
					" " << viewpoints[Viewpoints::counter].position[1] <<
					" " << viewpoints[Viewpoints::counter].position[2] << endl;
				cout << "Description " << viewpoints[Viewpoints::counter].description << endl;
			#endif
			
			Viewpoints::counter++;
			if(Viewpoints::counter > 100) {
				cerr << "To much viewpoints" << endl;
			}
		}
		
		DOMNodeList *children = NULL;
		children = node->getChildNodes ();
		if (children != NULL)
			for (i = 0; i < children->getLength (); i++)
				serializeViewpoint(children->item(i));
		break;
		
	}	
		
}


void x3d::route(DOMDocument *doc) {
	#ifdef DEBUG
		cout << "Routing" << endl;
	#endif
	routeStruct::routeCount=0;
	serializeRoute(doc);
}

void x3d::route() {
	#ifdef DEBUG
	cout << "Routing" << endl;
	#endif
	routeStruct::routeCount=0;
	serializeRoute(this->parser->getDocument());
	
	#ifdef DEBUG1
		cout << "serializing Route Total = " << routeStruct::routeCount << endl;
	#endif
	
}


void x3d::serializeRoute(DOMNode *node) {
		
	
	unsigned int i; // generic counter
	
	char* nodeName;
	char* valueName;
	char* value;
	
	switch (node->getNodeType()) {
	
	case (DOMNode::DOCUMENT_NODE):
		DOMNodeList *nodes;
		nodes = node->getChildNodes ();
		if (nodes != NULL)
			for (unsigned int i = 0; i < nodes->getLength (); i++)
				serializeRoute(nodes->item(i));
		break;
	
	case (DOMNode::ELEMENT_NODE):
		nodeName = XMLString::transcode(node->getNodeName ());
 
		if (!strcmp (nodeName, "ROUTE")) {
			
			#ifdef DEBUG1
				cout << "Found Route" << endl;
			#endif
			
			DOMNamedNodeMap *attributes;
			attributes = node->getAttributes();
			
			unsigned int attribLenght = attributes->getLength();
			
			if(attribLenght<4) cout << "Error Route attributes < 4 " << endl;
		
			#ifdef DEBUG1
				cout << "Allocing memory" << endl;
			#endif
			
			//routeTable[routeStruct::routeCount] = new routeStruct();
			// a verificar
			

			for(i = 0; i < attribLenght; i++) {
				DOMNode *current;
				current = attributes->item(i);
				valueName = XMLString::transcode (current->getNodeName ());
				value = XMLString::transcode (current->getNodeValue ());
				if (!strcmp(valueName, "fromNode")) {
					strcpy(routeTable[routeStruct::routeCount]->fromNode,value);
				} else
				if (!strcmp(valueName, "fromField")) {
					strcpy(routeTable[routeStruct::routeCount]->fromField,value);
				} else
				if (!strcmp(valueName, "toNode")) {
					strcpy(routeTable[routeStruct::routeCount]->toNode,value);
				} else
				if (!strcmp(valueName, "toField")) {
					strcpy(routeTable[routeStruct::routeCount]->toField,value);
				}
			}
			
			routeTable[routeStruct::routeCount]->field2 = NULL;
			
			
			// This avoid problems to access the same point 
			#ifdef DEBUG1
				cout << "Looking for error because two or more nodes to the same origin and destination" << endl;
			#endif
			for(i=0;i<routeStruct::routeCount;i++) {
				if (!strcmp(routeTable[i]->toNode,routeTable[routeStruct::routeCount]->toNode)) {
					if(!strcmp(routeTable[i]->toField,routeTable[routeStruct::routeCount]->toField)) {
					
							// TESTANDO ESTA PARTE DEVE SE PENSAR NISSO
							if (!strcmp(routeTable[i]->fromNode,routeTable[routeStruct::routeCount]->fromNode)) {
								if(!strcmp(routeTable[i]->fromField,routeTable[routeStruct::routeCount]->fromField)) {


						cerr << "ROUTEs trying to access the same origin and destination" << endl;
						return;
									
									
								}
							}
									
					}
				}
			}
			
			/*    OBSOLETA 
			// This is used if we have two or more same source, then the field is the same
			#ifdef DEBUG1
				cout << "Rerouting route nodes" << endl;
			#endif
			for(i=0;i<routeStruct::routeCount;i++) {
				if (!strcmp(routeTable[i]->fromNode,routeTable[routeStruct::routeCount]->fromNode)) {
					if(!strcmp(routeTable[i]->fromField,routeTable[routeStruct::routeCount]->fromField)) {
						routeTable[routeStruct::routeCount]->field2 = routeTable[i]->field2;
						#ifdef DEBUG
							// rerouting the address to not have problems with possition of node in X3D file
							cout << " ADresses : " << routeTable[i]->field << " ,  " << routeTable[routeStruct::routeCount]->field << endl;
						#endif
						break;
					}
				}
				
			}
			*/
			
			
			// In the case of no pointer alocation yet
			if(routeTable[routeStruct::routeCount]->field2==NULL) {
				routeTable[routeStruct::routeCount]->field2 = new void*;
				if( (routeTable[routeStruct::routeCount]->field2==0) || (routeTable[routeStruct::routeCount]->field2==NULL) ) {
					cerr << "Error alocing memory for route field !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n";
				}
				*routeTable[routeStruct::routeCount]->field2 = NULL; 
				routeTable[routeStruct::routeCount]->set = false;
			}
			
			
			#ifdef DEBUG
			cout << " route Table (" << routeStruct::routeCount << ") ";
			cout << " <ROUTE "
				<< "fromNode=\"" << routeTable[routeStruct::routeCount]->fromNode << "\" "
				<< "fromField=\"" << routeTable[routeStruct::routeCount]->fromField << "\" " 
				<< "toNode=\"" <<  routeTable[routeStruct::routeCount]->toNode << "\" "
				<< "toField=\"" <<  routeTable[routeStruct::routeCount]->toField << "\"/>" << endl;
			#endif
			

			// Safety routine to avoid error in memory access
			if( routeStruct::routeCount >= (MAX_ROUTE-1) ) {			// if the route counter is bigger the the alloced size
				cerr << "Maximun number of routes reached" << endl;	// warning message, probably the data will not appear anymore
			} else {													// otherwise
				routeStruct::routeCount++;								// increase the counter
			}
		
			
			#ifdef DEBUG1
				cout << "Route Count = " << routeStruct::routeCount << endl;
			#endif
	
		}
		
		// Looking for other children
		DOMNodeList *children;
		children = node->getChildNodes ();
		if (children != NULL)
			for (i = 0; i < children->getLength (); i++)
				serializeRoute(children->item(i));
		break;
		
	}	
}



/*  */
int x3d::group() {
	
	serializeGroup(this->parser->getDocument());
	
	int groupMenu;	// holds the menu
	groupMenu = glutCreateMenu(HandleGroupMenu); // creates the opengl menu
	
	unsigned int f; // counter
	for(f=0;f<menuCount;f++) {
		// This add a item in the opengl default menu
		glutAddSubMenu(menuName[f],Menu[f]);
	}
	
	return groupMenu;
}

/*  */
void x3d::serializeGroup(DOMNode *node) {
		
	char* nodeName;
	char* valueName;
	char* value;
	
	switch (node->getNodeType()) {
	
	case (DOMNode::DOCUMENT_NODE):
		DOMNodeList *nodes;
		nodes = node->getChildNodes();
		if (nodes != NULL)
			for (unsigned int i = 0; i < nodes->getLength (); i++)
				serializeGroup(nodes->item (i));
		break;
	
	case (DOMNode::ELEMENT_NODE):
		nodeName = XMLString::transcode(node->getNodeName ());
 
		DOMNodeList *children;
		children = node->getChildNodes ();
		if (children != NULL)
			for (unsigned int i = 0; i < children->getLength (); i++)
				serializeGroup(children->item (i));
	
		if (!strcmp (nodeName, "Group")) {
			
			DOMNamedNodeMap *attributes;
			attributes = node->getAttributes ();
			
			for (unsigned int ii = 0; ii < attributes->getLength(); ii++) {
				DOMNode *current;
				current = attributes->item (ii);
				valueName = XMLString::transcode (current->getNodeName ());
				value = XMLString::transcode (current->getNodeValue ());
								
				if (!strcmp (valueName, "DEF")) {
					strcpy(menuName[menuCount],value);
					#ifdef DEBUG
						cout << "DEF " << value << endl;
					#endif
				} 
			}

			Menu[menuCount] =   glutCreateMenu(HandleGroupMenu);
			glutAddMenuEntry("On/Off",1);
			menuCount++;
			
			if(menuCount > MAX_MENU) {
				cerr << "Error, to much menus" << endl;
			}
			
			#ifdef DEBUG
				cout << "DEF " << endl;
			#endif

		}
		
		break;

	
	}	
		
}
