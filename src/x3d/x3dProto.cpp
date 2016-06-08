
#include "x3dProto.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


ExternProtoDeclare::ExternProtoDeclare() {	// constructor
	
}


ProtoInstance::ProtoInstance() {	// constructor
	
}


ProtoDeclare::ProtoDeclare() {	// constructor
	
}

ProtoDeclare *ProtoDeclare::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	ProtoDeclare *protoDeclare2;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"USE")) {
			return((ProtoDeclare *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	protoDeclare2 = new ProtoDeclare();
	protoDeclare2->read(node);
	
	return protoDeclare2;
}


void ProtoDeclare::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	#ifdef DEBUG
		cout << "ProtoDeclare Node" << endl;
	#endif
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode(attributes->item(i)->getNodeValue()));
		} 
	}
	
	
	
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "name")) {
			strcpy(this->name,XMLString::transcode(attributes->item(i)->getNodeValue()));
		} 
	}
	
	
}





ProtoBody::ProtoBody() {	// constructor
	
}
