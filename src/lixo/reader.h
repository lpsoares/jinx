
#ifndef __reader_h
#define __reader_h

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>


using namespace std;


class Conf {
	public:
	
	Conf();
	
	int width;
	int height;
	
	int fullscreen;
	
	int stereo;
	
	double left;
	double right;
	double bottom;
	double top;
	double near;
	double far;
	
	int le(char* arg);
	
	int positionx;
	int positiony;
	
	char* print_node(DOMNode *elm);
};

Conf::Conf() {
	this->width=0;
	this->height=0;
	
	this->fullscreen=0;
	
	this->stereo=0;
	
	this->left=0;
	this->right=0;
	this->bottom=0;
	this->top=0;
	this->near=0;
	this->far=0;

	this->positionx=0;
	this->positiony=0;
}

int Conf::le(char* arg) {

	int retval = 0;
	
	static char* gXmlFile = 0;
	static bool gDoNamespaces = false;
	static bool gDoSchema= false;
	static bool gSchemaFullChecking = false;
	static bool gDoCreate = true;
	static XercesDOMParser::ValSchemes gValScheme = XercesDOMParser::Val_Auto;

	try {
		XMLPlatformUtils::Initialize();
	}
	catch(const XMLException &toCatch) {
		cerr << "Error during Xerces-c Initialization.\n"
		<< "  Exception message:"
		<< endl;
		return 1;
	}

	XercesDOMParser *parser = new XercesDOMParser;
	parser->setValidationScheme(gValScheme);
	parser->setDoNamespaces(gDoNamespaces);
	parser->setDoSchema(gDoSchema);
	parser->setValidationSchemaFullChecking(gSchemaFullChecking);
	parser->setCreateEntityReferenceNodes(gDoCreate);
	
	bool errorsOccured = false;
	
	try {
		gXmlFile = arg;
		parser->parse(gXmlFile);		
	}
	catch (const XMLException& e) {
		cerr << "An error occurred during parsing\n   Message: "
		<< endl;errorsOccured = true;
	}
	catch (const DOMException& e) {
		cerr << "A DOM error occurred during parsing\n   DOMException code: "
		<< e.code << endl;errorsOccured = true;
	}
	catch (...) {
		cerr << "An error occurred during parsing\n " << endl;
		errorsOccured = true;
	}

    if (!errorsOccured) {
		try {
			char* message[4];
			
			// The DOMDocument handle the tree
			DOMDocument  *doc = parser->getDocument();  // this is very specific of xerces maybe other parser have problems
			
			DOMElement *elm =   doc->getDocumentElement();
						
			const XMLCh* name = elm->getTagName();
			message[0] = XMLString::transcode(name);
			cout << "tag " << message[0] << endl;	
			const XMLCh* nodeName = elm->getNodeName();
			message[1] = XMLString::transcode(nodeName);
			cout << "NodeName " << message[1] << endl;
			const XMLCh* value = elm->getNodeValue();
			message[2] = XMLString::transcode(value);
			cout << "NodeValue " << message[2] << endl;

			print_node(elm);

		}
		catch (XMLException& e) {
			cerr << "An error occurred during creation of output transcoder. Msg is:"
			<< endl;
			retval = 4;
		}
	}
	else
		retval = 4;

	
	delete parser;

	XMLPlatformUtils::Terminate();

	return retval;
}



char* Conf::print_node(DOMNode *elm) {

	
	DOMNode *child;
	
	char* message4;
	char* message5;

	const XMLCh* childName;
	const XMLCh* childValue;
	
	if(elm->hasChildNodes())
		child = elm->getFirstChild();
	else
		return NULL;
	
	while(elm->hasChildNodes()) {
		
		childName = child->getNodeName();
		message4 = XMLString::transcode(childName);
		cout << "childName " << message4 << endl;
		
		childValue = child->getNodeValue();
		message5 = XMLString::transcode(childValue);
		cout << "childValue " << message5 << endl;

		if(!strcmp(message4,"width")) {
			this->width = atoi(print_node(child));
		}
		else if(!strcmp(message4,"height")) {
			this->height = atoi(print_node(child));
		}
		else if(!strcmp(message4,"fullscreen")) {
			this->fullscreen=1;
		} 
		else if(!strcmp(message4,"stereo")) {
			this->stereo=1;
		} 
		else if(!strcmp(message4,"left")) {
				this->left = atof(print_node(child));
		}
		else if(!strcmp(message4,"right")) {
				this->right = atof(print_node(child));
		}
		else if(!strcmp(message4,"bottom")) {
				this->bottom = atof(print_node(child));
		}
		else if(!strcmp(message4,"top")) {
				this->top = atof(print_node(child));
		}
		else if(!strcmp(message4,"near")) {
				this->near = atof(print_node(child));
		}
		else if(!strcmp(message4,"far")) {
				this->far = atof(print_node(child));
		} 
		else if(!strcmp(message4,"positionx")) {
				this->positionx = atoi(print_node(child));
		} 
		else if(!strcmp(message4,"positiony")) {
				this->positiony = atoi(print_node(child));
		} 
		else {
			print_node(child);
		}
		
		child = child->getNextSibling();
		
		if(child==NULL) return message5;
	}

	return NULL;

}

#endif
