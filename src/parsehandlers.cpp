
#include "parsehandlers.h"

// Includes C & C++
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <iostream>

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax/AttributeList.hpp>

using namespace std;


PParseHandlers::PParseHandlers() :

    fElementCount(0)
    , fAttrCount(0)
    , fCharacterCount(0)
    , fSpaceCount(0)
{
	
	posx=0;
	posy=0;
	posz=0;
	angx=0;
	angy=0;
	angz=0;
	lft=false;
	mid=false;
	rgt=false;
	quaternion[0]=1.0f;
	quaternion[1]=0.0f;
	quaternion[2]=0.0f;
	quaternion[3]=0.0f;
	type=0; // means euler
	
}

PParseHandlers::~PParseHandlers(){
}


void PParseHandlers::startElement(const   XMLCh* const    name
                                    ,       AttributeList&  attributes) {
    fElementCount++;
    fAttrCount += attributes.getLength();
	
	if (!strcmp (XMLString::transcode(name), "pos")) data_type=POS;
	if (!strcmp (XMLString::transcode(name), "ang")) data_type=ANG;
	//if (!strcmp (XMLString::transcode(name), "ang")) data_type=FIN;
	if (!strcmp (XMLString::transcode(name), "button")) data_type=BUT;
		
	if (!strcmp (XMLString::transcode(name), "w")) xyz_value=VW;
	if (!strcmp (XMLString::transcode(name), "x")) xyz_value=VX;
	if (!strcmp (XMLString::transcode(name), "y")) xyz_value=VY;
	if (!strcmp (XMLString::transcode(name), "z")) xyz_value=VZ;
		
	if (!strcmp (XMLString::transcode(name), "f0")) xyz_value=0;
	if (!strcmp (XMLString::transcode(name), "f1")) xyz_value=1;
	if (!strcmp (XMLString::transcode(name), "f2")) xyz_value=2;
	if (!strcmp (XMLString::transcode(name), "f3")) xyz_value=3;
	if (!strcmp (XMLString::transcode(name), "f4")) xyz_value=4;
	if (!strcmp (XMLString::transcode(name), "f5")) xyz_value=5;
	if (!strcmp (XMLString::transcode(name), "f6")) xyz_value=6;
	if (!strcmp (XMLString::transcode(name), "f7")) xyz_value=7;
	if (!strcmp (XMLString::transcode(name), "f8")) xyz_value=8;
	if (!strcmp (XMLString::transcode(name), "f9")) xyz_value=9;
	if (!strcmp (XMLString::transcode(name), "f10")) xyz_value=10;
	if (!strcmp (XMLString::transcode(name), "f11")) xyz_value=11;
	if (!strcmp (XMLString::transcode(name), "f12")) xyz_value=12;
	if (!strcmp (XMLString::transcode(name), "f13")) xyz_value=13;
	
	lft=false;
	mid=false;
	rgt=false;
	
	unsigned int index; 				// variable to counter
	for (index = 0; index < attributes.getLength(); index++) {
		#ifdef DEBUG3
			cout << "Name : " << XMLString::transcode(attributes.getName(index)) << endl;
			cout << "Value : " <<  XMLString::transcode(attributes.getValue(index)) << endl;
		#endif
		
		if (!strcmp(XMLString::transcode(attributes.getName(index)),"type")) {
			if(!strcmp(XMLString::transcode(attributes.getValue(index)),"euler")) {
				//cout << "euler" << endl;
				type=0;
			} else {
				//cout << "quaternion" << endl;
				type=1;
			}
		}
	}
	

	#ifdef DEBUG									
	cout << XMLString::transcode(name) << endl;
	#endif

}

void PParseHandlers::endElement(const XMLCh* const name , AttributeList&  attributes) {
    fElementCount++;
    fAttrCount += attributes.getLength();
	
	data_type=0;
	xyz_value=0;
										
	#ifdef DEBUG									
	cout << XMLString::transcode(name) << endl;
	#endif

}

void PParseHandlers::characters(const XMLCh* const chars , const unsigned int length) {
	#ifdef DEBUG
	cout << "------------------------" << endl;
	cout << "data " << data_type << endl;
	cout << "xyz " << xyz_value << endl;
	cout << XMLString::transcode(chars) << endl;
	cout << "length" << length << endl;
	#endif

	if(data_type==BUT) {

		if(!strcmp(XMLString::transcode(chars)," LFT ")) {
			#ifdef DEBUG3
				cout << "Pressed left button" << endl;
			#endif
			lft=true;
		}
		if(!strcmp(XMLString::transcode(chars)," MID ")) mid=true;
		if(!strcmp(XMLString::transcode(chars)," RGT ")) rgt=true;
	}
	
	if(length>3) {
	if(data_type==POS) {
		if(xyz_value==VX) posx = atof(XMLString::transcode(chars));
		if(xyz_value==VY) posy = atof(XMLString::transcode(chars));
		if(xyz_value==VZ) posz = atof(XMLString::transcode(chars));
	}
	
	if(data_type==ANG) {
		if(type==0) {
			if(xyz_value==VX) angx = atof(XMLString::transcode(chars));
			if(xyz_value==VY) angy = atof(XMLString::transcode(chars));
			if(xyz_value==VZ) angz = atof(XMLString::transcode(chars));
		} else { //type==1
			if(xyz_value==VW) quaternion[0] = atof(XMLString::transcode(chars));
			if(xyz_value==VX) quaternion[1] = atof(XMLString::transcode(chars));
			if(xyz_value==VY) quaternion[2] = atof(XMLString::transcode(chars));
			if(xyz_value==VZ) quaternion[3] = atof(XMLString::transcode(chars));
		}							
			
	}

	if(data_type==FIN) {
		fingers[xyz_value] = atof(XMLString::transcode(chars));
	}
	
	}
	
    fCharacterCount += length;
}

void PParseHandlers::ignorableWhitespace( const   XMLCh* const chars
										    , const unsigned int length)
{
    fSpaceCount += length;

	#ifdef DEBUG									
	cout << XMLString::transcode(chars) << endl;
	#endif
	

}

void PParseHandlers::resetDocument() {
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}


// ---------------------------------------------------------------------------
//  PParseHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void PParseHandlers::error(const SAXParseException& e) {
    cerr << endl << "Error at file "; cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line "; cerr << e.getLineNumber();
	cerr << ", char "; cerr << e.getColumnNumber();
    cerr << endl << "  Message: "; cerr << XMLString::transcode(e.getMessage()) << endl;
}

void PParseHandlers::fatalError(const SAXParseException& e) {
    cerr << endl << "Fatal Error at file "; cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line "; cerr << e.getLineNumber();
	cerr << ", char "; cerr << e.getColumnNumber();
    cerr << endl << "  Message: "; cerr << XMLString::transcode(e.getMessage()) << endl;
}

void PParseHandlers::warning(const SAXParseException& e) {
    cerr << endl << "Warning at file "; cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line "; cerr << e.getLineNumber();
	cerr << ", char "; cerr << e.getColumnNumber();
    cerr << endl << "  Message: "; cerr << XMLString::transcode(e.getMessage()) << endl;
}
