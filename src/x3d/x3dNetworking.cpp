
#include "x3d.h"
#include "x3dNetworking.h"
#include "../commands.h"
#include "../engine.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

Inline::Inline() {

	this->load = true;
	
}

void Inline::follow() {
	
	if(load) {

		#ifdef DEBUG
			cout << "Loading" << endl;
		#endif
		
		this->ixb->follow();
		
	}
	
}

Inline *Inline::get(DOMNode *node) {
unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Inline *inline2 = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			inline2 = (Inline *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( inline2->ami("Inline") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(inline2);
			
		} 
	}
		
	
	// verificar se o USE ta retornando NULL
	
	inline2 = new Inline();
	if(inline2==NULL) { 
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(inline2->DEF,DEF2);
		setLink(inline2->DEF,inline2);			
	}
	
	inline2->read(node);
	
	return inline2;

}


void *Inline::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
	int couter_url=0;		// in the case of many urls in one specification
	
	#ifdef DEBUG
		cout << "Inline NODE ---------------" << endl;
	#endif
	
	attributes = node->getAttributes ();
		
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "load")) {
			if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeValue()) , "true")) {
				this->load = true;
			} else {
				this->load = false;
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," \n");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->url[couter_url],tmp1+1,((int)(strlen(tmp1)))-2);
					this->url[couter_url][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->url[couter_url],tmp1);
				}
				
				#ifdef DEBUG1
					cout << " Inline [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," \n");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		}
	}

	DOMDocument *doc;		// holds the X3D XML document
	char *filename;			// to put the hhtp at the begining	
	
	if(couter_url==0) {
		this->load=false;		// it is not possible read something that does not exist
	}
	
	if(couter_url>0) {
		
		if(strncmp("http://",url[0],7)) { // in the case that the http is not included
					
			filename = new char[256];
			
			#ifdef DEBUG
				cout << " path " << Engine::getSource() << endl;
			#endif

			strcpy(filename,Engine::getSource()); 	// put the path in the tmp variable
			strcat(filename,url[0]);				// cat the file
			
			strcpy(url[0],filename);				// return to the local variable
			
			delete[] filename;
			
		} 
		
		#ifdef DEBUG1
			cout << "Loading Inline" << endl;
		#endif
		doc = x3d::my_x3d->load(url[0]);			// set the filename
		
		if(doc==NULL) {
			cout << "Problems parsing document" << endl;
			this->load = false;
		} else {
			#ifdef DEBUG1
				cout << "Serializing Inline" << endl;
			#endif
			this->ixb = x3d::my_x3d->serialize();		// read and parse
		}
		
	}
	
	return(NULL);
}


bool Inline::ray(float o[4],float d[4],float m[4][4]) {
	
	bool test=false;
	
	float mori[4][4];
	
	mcopy(m,mori);
	
	#ifdef DEBUG1
	cout << "m " << m[0][0] << "  " << m[0][1] << "  " << m[0][2] << "  " << m[0][3] << endl;
	cout << "  " << m[1][0] << "  " << m[1][1] << "  " << m[1][2] << "  " << m[1][3] << endl;
	cout << "  " << m[2][0] << "  " << m[2][1] << "  " << m[2][2] << "  " << m[2][3] << endl;
	cout << "  " << m[3][0] << "  " << m[3][1] << "  " << m[3][2] << "  " << m[3][3] << endl << endl;
	#endif
	
	(this->ixb->ray(o,d,mori)?test=true:test=test); // nada inteligente
	
	return(test);

}



Anchor::Anchor() {

	this->countChildren=0;
		
	for(int f=0;f<MAX_NUM_TRANSFORM;f++)
		this->children[f] = NULL;

	this->cast=false;
	
}

void Anchor::follow() {
	
	unsigned int f;
	f=0;
	
	#ifdef DEBUG1
		cout << "\t\t\t\t\tLooking for children" << endl;	// Message about debuging
	#endif
	//-- This rotine follow each children node --- //
	for(f=0;f<this->countChildren;f++) {
		this->children[f]->follow(); 
	}

	Commands *comm1 = NULL;
	MPI_Request request;
	
	if(cast==true) {

		#ifdef DEBUG
			cout << "Loading the Anchor" << endl;
		#endif
		
		comm1 = new Commands();
		if(comm1==NULL) {
			cout << "Error on comm1" << endl;
		}
		
		#ifdef DEBUG
		cout << "ReSerializing" << this->url[0] << endl;
		#endif
		x3d::my_x3d->x3dbase = x3d::my_x3d->serialize(this->doc);
		
		#ifdef DEBUG
		cout << "Sending reboot" << endl;
		#endif
		
		comm1->command.reboot=1;
		strcpy(comm1->command.url,this->url[0]);
		MPI_Isend(&comm1->command, 1, comm1->commandsDatatype, MAIN, COMMAND, MPI_COMM_WORLD, &request);

	//menu();
		
	}
	
}

Anchor *Anchor::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Anchor *anchor;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Anchor *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	anchor = new Anchor();
	anchor->read(node);
	
	return anchor;
}


void *Anchor::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
	int couter_url=0;		// in the case of many texture files in one specification
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
		} 
	}
	
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->url[couter_url],tmp1+1,((int)(strlen(tmp1)))-2);
					this->url[couter_url][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->url[couter_url],tmp1);
				}
				
				#ifdef DEBUG
					cout << " Anchor [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," ");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		}
	}

	char *filename;			// to put the hhtp at the begining	

	if(couter_url>0) {
		
		
		
		if(strncmp("http://",url[0],7)) { // in the case that the http is not included
					
			filename = new char[256];
			
			#ifdef DEBUG
				cout << " path " << Engine::getSource() << endl;
			#endif

			strcpy(filename,Engine::getSource()); 	// put the path in the tmp variable
			strcat(filename,url[0]);				// cat the file
			
			strcpy(url[0],filename);				// return to the local variable
			
			delete[] filename;
			
		} 
		
		this->doc = x3d::my_x3d->load(url[0]);			// set the filename
				
	}

	DOMNodeList *children;			//* variable to hold the node children
	children = node->getChildNodes ();
	this->countChildren = x3d::getChildren(children,this->children);

	return(NULL);
	
}

bool Anchor::ray(float o[4],float d[4],float m[4][4]) {
	
	bool test=false;
	
	unsigned int f=0;
		
	float mori[4][4];
	
		for(f=0;f<this->countChildren;f++) {
		mcopy(m,mori);
		(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}

	this->cast=test;
	
	return(test);

}
