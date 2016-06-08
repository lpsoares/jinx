

#ifdef GLEW
	// To GLSL
	#include "../render/ezGLSL.h"
#endif


#include "x3dShading.h"

#include "x3d.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


using namespace std;


VertexShader::VertexShader() {
	
}


VertexShader *VertexShader::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	VertexShader *vertexShader;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((VertexShader *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
		
	vertexShader = new VertexShader();
	vertexShader->read(node);
		
	return vertexShader;
	
}

bool VertexShader::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG1
		cout << "VertexShader Ray Routine" << endl;  
	#endif
	
	return(false);

}

void VertexShader::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (VertexShader)" << endl;	// Message about debuging
	#endif
	
	
}

void *VertexShader::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (VertexShader) " << endl;  
	#endif
	
	unsigned int i; 				//* variable to counter
	//DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}
		
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "url")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," \n");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->url[couter_url],tmp1+1,((int)(strlen(tmp1)))-2);
					this->url[couter_url][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->url[couter_url],tmp1);
				}
				
				#ifdef DEBUG
					cout << " Shader [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," \n");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		}
	}

	
	#ifdef DEBUG1
		cout << "Returning (VertexShader) " << endl;  
	#endif
	
	return(NULL);
}


char *VertexShader::getURL() {
	return(url[0]);
}



FragmentShader::FragmentShader() {
	
}


FragmentShader *FragmentShader::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	FragmentShader *fragmentShader;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((FragmentShader *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
		
	fragmentShader = new FragmentShader();
	fragmentShader->read(node);
		
	return fragmentShader;
	
}

bool FragmentShader::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG1
		cout << "FragmentShader Ray Routine" << endl;  
	#endif
	
	return(false);

}

void FragmentShader::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (FragmentShader)" << endl;	// Message about debuging
	#endif
	
	
}

void *FragmentShader::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (FragmentShader) " << endl;  
	#endif
	
	unsigned int i; 				//* variable to counter
	//DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}
		
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "url")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->url[couter_url],tmp1+1,((int)(strlen(tmp1)))-2);
					this->url[couter_url][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->url[couter_url],tmp1);
				}
				
				#ifdef DEBUG
					cout << " Shader [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," ");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		}
	}
	
	
	#ifdef DEBUG1
		cout << "Returning (FragmentShader) " << endl;  
	#endif
	
	return(NULL);
}



char *FragmentShader::getURL() {
	return(url[0]);
}
