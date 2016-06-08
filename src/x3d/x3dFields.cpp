
#include "x3dFields.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

#include <xercesc/util/XMLString.hpp>


using namespace std;

field::field() {
	
}

void *field::read(DOMNode *node) {
	
	
	#ifdef DEBUG1
		cout << "Reading (field) " << endl;  
	#endif
	
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
		
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "name")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
				strncpy(this->name,tmp1+1,((int)(strlen(tmp1)))-2);
				this->name[((int)(strlen(tmp1)))-2]='\0';
			} else {
				strcpy(this->name,tmp1);
			}
		} else 
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "type")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
				strncpy(this->type,tmp1+1,((int)(strlen(tmp1)))-2);
				this->type[((int)(strlen(tmp1)))-2]='\0';
			} else {
				strcpy(this->type,tmp1);
			}
		} else
		if(!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "accessType")) {
			if (!strcasecmp( XMLString::transcode (attributes->item(i)->getNodeValue()),"inputOnly")) {
				this->accessType=1; 
			} else 
			if (!strcasecmp( XMLString::transcode (attributes->item(i)->getNodeValue()),"outputOnly")) {
				this->accessType=2; 
			}	
		}
	}
	
	if(!strcmp(this->name,"")) cout << "Name parameter not especified" << endl;

	return(NULL);

}
