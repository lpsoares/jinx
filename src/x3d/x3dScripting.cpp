

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

#include <xercesc/dom/DOMCDATASection.hpp>

#include "../www.h"

#include "../engine.h"


#include "x3dScripting.h"


using namespace std;


// Constructor
Script::Script() {
	
	this->directOutput	= new SFBool;				//
	*this->directOutput	= false;					//
	
	this->mustEvaluate 	= new SFBool;				//
	*this->mustEvaluate	= false;					//

	//this->url			= new MFString[16][256];	//
	strcpy(this->url[0],"");						// default value
	
	this->couter_url=0;								// counter for urls
	
	this->countField=0;
	
	this->fieldName = new field*[32];				// just 32 fields are supported now
	
}

Script *Script::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Script *script;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Script *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
		
	
	// verificar se o USE ta retornando NULL
	
	script = new Script();
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(script->DEF,DEF2);
		setLink(script->DEF,script);
	}
	
	script->read(node);
	
	return script;
	
}


void *Script::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "Reading (Script) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	DOMNodeList *children;			// variable to hold the node children
	
	char *tmp1;				// just to read the temporary tokens
	
	
	attributes = node->getAttributes();	
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
				tmp1 = strtok(NULL," ");
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"directOutput")) {
			if (!strcasecmp(XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->directOutput=true;
			} else {
				*this->directOutput=false;
			}
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"mustEvaluate")) {
			if (!strcasecmp(XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->mustEvaluate=true;
			} else {
				*this->mustEvaluate=false;
			}
		} 
	}
	
	char *cdata=NULL;  // holds the script
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			
			if (!strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#cdata-section")) {
				
				cdata = XMLString::transcode(children->item(i)->getNodeValue());
				
				#ifdef DEBUG1
					cout << "Showing CDATA " << cdata << endl;
				#endif
			} else 
			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"field")) {
				fieldName[countField] = new field();
				fieldName[countField++]->read(children->item(i));
			} 
			
			
			
		}
	}
	
	
	
	////////////////////// opening the file /////////////////////////
	
	if(strcmp(this->url[0],"")) {
	
		FILE *file;				// holds the texture

		WWW *www = NULL; 		// to open WWW files
		www = new WWW();		// creating WWW reader
		if(www==NULL) {
			cerr << "Error alocating memory for WWW" << endl;
			return(0);	
		}
	
		#ifdef DEBUG
			cout << " path " << Engine::getSource() << endl;
		#endif
	
		www->getfile(this->url[0], Engine::getSource());
	
		struct stat st;
		char *newname = this->url[0];

		if (stat(www->complete(this->url[0]), &st)) {
			newname = strrchr(www->complete(this->url[0]), '\\')+1;
			if (!(newname-1) || stat(newname, &st)) {
				cerr << "Not able to read file : " << this->url[0] << endl;
				return(0);
			}
		}
	
	    // make sure the file is there.
		if ((file = fopen(www->complete(this->url[0]), "rb"))==NULL) {
			cerr << "File Not Found : " << this->url[0] << endl;
			return(0);
	    }
    
		delete www;					// destroy the www reader
	
	
	
	
			// FAZ O QUE TEM DE FAZER AQUI
	
	
	
	
		fclose(file);		// close file descriptor
	
	}
	
	unsigned int f,g; // counters
	
	
	// INPUT
	for(f=0;f<routeStruct::routeCount;f++) {

		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
		
			for(g=0;g<countField;g++) {

				if( (!strcmp(x3d::my_x3d->routeTable[f]->toField,fieldName[g]->name)) ) {
		
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						
						if(!strcmp(this->fieldName[g]->type,"SFFloat")) {

							(*x3d::my_x3d->routeTable[f]->field2) = new SFFloat;
							fieldName[g]->value = (void (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFColor")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFColor[3];
							fieldName[g]->value = (void (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFVec3f")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFVec3f[3];
							fieldName[g]->value = (void (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFTime")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFTime;
							fieldName[g]->value = (void (*))(*x3d::my_x3d->routeTable[f]->field2);
						}
						
					}
					
				} 

			}

		}
		
	}




	
	// OUTPUT
	for(f=0;f<routeStruct::routeCount;f++) {

		if (!strcmp (x3d::my_x3d->routeTable[f]->fromNode,DEF)) {
		
			for(g=0;g<countField;g++) {

				if( (!strcmp(x3d::my_x3d->routeTable[f]->fromField,fieldName[g]->name)) ) {
		
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						
						if(!strcmp(this->fieldName[g]->type,"SFFloat")) {

							(*x3d::my_x3d->routeTable[f]->field2) = new SFFloat;
							fieldName[g]->value = (SFFloat (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFColor")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFColor[3];
							fieldName[g]->value = (SFColor (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFVec3f")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFVec3f[3];
							fieldName[g]->value = (void (*))(*x3d::my_x3d->routeTable[f]->field2);
						} else
						if(!strcmp(this->fieldName[g]->type,"SFTime")) {
							(*x3d::my_x3d->routeTable[f]->field2) = new SFTime;
							fieldName[g]->value = (SFTime (*))(*x3d::my_x3d->routeTable[f]->field2);
						}
						
					}
					
				} 

			}

		}
		
	}









	
	return(NULL);
	
}


bool Script::ray(float o[4],float d[4],float m[4][4]) {
	
	return(false);
	
}

void Script::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Script)" << endl;	// Message about debuging
	#endif
	
	
	unsigned int g;
	
	SFVec3f *vec;
	vec = new SFVec3f[3];
	
	for(g=0;g<countField;g++) {

		if( (!strcmp("pos",fieldName[g]->name)) ) {
			
			vec = (SFVec3f (*))(fieldName[g]->value);
			
			*vec[0]=-2.0f;
			*vec[1]=1.0f;
			*vec[2]=-10.0f;
			
		}
	
	}
		
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Group)" << endl;	// Message about debuging
	#endif

}
