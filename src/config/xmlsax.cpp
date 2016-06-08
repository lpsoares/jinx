

#include "xmlsax.h"

#include <stdlib.h>
#include <iostream>

Configuration::Configuration() {
	this->window = 0;	
	
	// This is very usefull if the computer doesnot appears in the computer list
	// Because it is just reseted if the parser goes at least one time by a computer node
	// that is not always true
	this->computer.viewports[0].width = 640;			//
	this->computer.viewports[0].height = 480;		//
	this->computer.viewports[0].fullscreen = false;	//

	this->computer.viewports[0].frustum[0] = -1.0; 	//left
	this->computer.viewports[0].frustum[1] =  1.0; 	//right
	this->computer.viewports[0].frustum[2] = -1.0; 	//bottom
	this->computer.viewports[0].frustum[3] =  1.0; 	//top
	this->computer.viewports[0].frustum[4] =  1.5; 	//left

	this->computer.viewports[0].direction[0] =  0; 	//X
	this->computer.viewports[0].direction[1] =  0; 	//Y
	this->computer.viewports[0].direction[2] = -1; 	//Z
			
	this->computer.viewports[0].up[0] =  0; 			//X
	this->computer.viewports[0].up[1] =  1; 			//Y
	this->computer.viewports[0].up[2] =  0; 			//Z

	this->computer.viewports[0].position[0] =  0; 	//pos X
	this->computer.viewports[0].position[1] =  0; 	//pos Y
			
	this->computer.viewports[0].stereoMode=0;		// Mono
	this->computer.viewports[0].projectionMode=0;	// Planar

	this->computer.inputDevices[0].type=0;

	this->computer.navigationDevice[0].defined=false;
	strcpy(this->computer.navigationDevice[0].device,"/dev/js0");
	
}

bool Configuration::getJoystick(unsigned short int window){ 
	return(this->computer.navigationDevice[window].defined);
}

char *Configuration::getDeviceName(unsigned short int window){ 
	return(this->computer.navigationDevice[window].device);
}

unsigned short int Configuration::getWidth(unsigned short int window) {
	return computer.viewports[window].width; 
}
	
unsigned short int Configuration::getHeight(unsigned short int window) {
	return computer.viewports[window].height; 
}

void Configuration::setWidth(unsigned short int size) {
	computer.viewports[0].width = size;
}

void Configuration::setHeight(unsigned short int size) {
	computer.viewports[0].height = size;
}

bool Configuration::getFullScreen(unsigned short int window) { 
	return computer.viewports[window].fullscreen; 
}

double Configuration::getLeft(unsigned short int window) { 
	return computer.viewports[window].frustum[0]; 
}

double Configuration::getRight(unsigned short int window) { 
	return computer.viewports[window].frustum[1]; 
}

double Configuration::getBottom(unsigned short int window) {
	return computer.viewports[window].frustum[2]; 
}

double Configuration::getTop(unsigned short int window) {
	return computer.viewports[window].frustum[3]; 
}

double Configuration::getPlanarDepth(unsigned short int window) {
	return computer.viewports[window].frustum[4];
}

int Configuration::getProjectionView(unsigned short int window) { 
	return computer.viewports[window].projectionMode;
}

unsigned short int Configuration::getPositionX(unsigned short int window) { 
	return computer.viewports[window].position[0];
}

unsigned short int Configuration::getPositionY(unsigned short int window) {
	return computer.viewports[window].position[1];
}

double Configuration::getDirX(unsigned short int window)  { 
	return computer.viewports[window].direction[0]; 
}

double Configuration::getDirY(unsigned short int window)  { 
	return computer.viewports[window].direction[1];
}

double Configuration::getDirZ(unsigned short int window)  { 
	return computer.viewports[window].direction[2];
}
	
double Configuration::getUpX(unsigned short int window)  { 
	return computer.viewports[window].up[0];
}

double Configuration::getUpY(unsigned short int window)  { 
	return computer.viewports[window].up[1];
}

double Configuration::getUpZ(unsigned short int window)  { 
	return computer.viewports[window].up[2];
}

unsigned short int Configuration::getWandGlobal() { 
	#ifdef DEBUG
	cout << " MODO EM " << this->wand << endl;
	#endif
	return this->wand;
}

void Configuration::setWandGlobal() { 	
	#ifdef DEBUG
	cout << "Defining the wand configuration" << endl;
	#endif
	this->wand=0;	// variable that holds the stereo situation
	int f;					// temporary counter
	for(f=0; f<this->window; f++) {
		if(computer.inputDevices[0].type!=0) {
			this->wand=computer.inputDevices[0].type;
		}
	}
}


void Configuration::setStereoGlobal() { 
	this->stereoGlobal=0;	// variable that holds the stereo situation
	unsigned short int f;	// temporary counter
	for(f=0; f<this->window; f++) {
		if( computer.viewports[f].stereoMode == 1 ) this->stereoGlobal=1;
		if( (  ( (computer.viewports[f].stereoMode) == 2) || ( (computer.viewports[f].stereoMode) == 3)    ) && (this->stereoGlobal!=1) ) this->stereoGlobal=2;
		if( computer.viewports[f].stereoMode == 4 ) this->stereoGlobal=4;
	}
}

int Configuration::getStereoGlobal() { 
	#ifdef DEBUG
	cout << " Mode in " << this->stereoGlobal << endl;
	#endif
	return this->stereoGlobal;
}



int Configuration::getStereo(unsigned short int window) { 
	return computer.viewports[window].stereoMode; 
}

void Configuration::setStereo(bool i) {
	(i?computer.viewports[0].stereoMode=1:
		computer.viewports[0].stereoMode=0); 
}
		
unsigned short int Configuration::getWindow() {
		return window; 
} // this return the number of windows (view ports) in the computer
	
int Configuration::getWidthMax() { 
		
		int max=0;
		int xmin=getPositionXMin();
		int posx,width;
		
		for(int f=0; f<this->window; f++) {
			
			posx=computer.viewports[window].position[0];
			width=computer.viewports[window].width;
			
			if( ( posx - xmin  + width ) > max ) 
				max = posx - xmin + width;
			
		}
		
	return max; 
}
		
int Configuration::getHeightMax() { 
		
		int max=0;
		
		int ymin=getPositionYMin();
		
		int posy,height;
		
		for(int f=0; f<this->window; f++) {
			
			posy=computer.viewports[window].position[1];
			height=computer.viewports[window].height;

			if( ( posy - ymin  + height ) > max ) 
				max = posy - ymin + height;
			
		}
		return max; 
}
	
int Configuration::getPositionXMin() {
		int min=32767;
		unsigned short int f;
		for(f=0; f<this->window; f++) {
			if(computer.viewports[f].position[0] < min ) 
				min = computer.viewports[f].position[0];
		}
		return min; 
}
		
int Configuration::getPositionYMin() {
		int min=32767;
		unsigned short int f;
		for(f=0; f<this->window; f++) {
			if(computer.viewports[f].position[1] < min ) 
				min = computer.viewports[f].position[1];
		}
		return min; 
}
	



xml::xml(char *fileName) {

	conf = new Configuration();

	static const char*          encodingName    = "LATIN1";
	static XMLFormatter::UnRepFlags unRepFlags      = XMLFormatter::UnRep_CharRef;
	static bool					expandNamespaces= false;

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgXercesSchema, true);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, false);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);

    //  Create the handler object and install it as the document and error
    //  handler for the parser. Then parse the file and catch any exceptions that propogate out
    try {
        SAX2PrintHandlers handler(encodingName, unRepFlags, expandNamespaces, conf);
        parser->setContentHandler(&handler);
        parser->setErrorHandler(&handler);
  		parser->parse(fileName);
    }
    catch (const XMLException& toCatch) {
        cerr << endl << "An error occurred" << endl << "  Error: ";
        cerr << XMLString::transcode(toCatch.getMessage()) << endl;
    }

    // already in the memory
	delete parser; //  Delete the parser itself.  Must be done prior to calling Terminate, below.

	conf->setStereoGlobal();
	conf->setWandGlobal();
	
}

xml::~xml() {
	delete conf;	
}
