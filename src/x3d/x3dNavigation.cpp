#include "x3d.h" 
#include "x3dNurbs.h"
 
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>


NavigationInfo::NavigationInfo() {

}

void *NavigationInfo::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			//setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		//if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			//return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		//} else 		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "avatarSize")) {
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "headlight")) {
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "speed")) {
		} 
	}
	
	return(NULL);
	
}



Collision::Collision() {
	
	this->countChildren=0;
	
}

Collision *Collision::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	Collision *col;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Collision*)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
		
	col = new Collision();
	col->read(node);
		
	return col;
	
}

void Collision::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Collision)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;
	f=0;
	
	for(f=0;f<this->countChildren;f++) {
		this->children[f]->follow();
	}
	
}

void *Collision::read(DOMNode *node) {

	#ifdef DEBUG2
		cout << "\t\t\t\tReading (Collision)" << endl;	// Message about debuging
	#endif
	
	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			//strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

	for (i = 0; i < attributes->getLength(); i++) {
		//
	}

	children = node->getChildNodes ();
	
	this->countChildren = x3d::getChildren(children,this->children);
	
	return(NULL);
}



bool Collision::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (Collision)" << endl;	// Message about debuging
	#endif
	
	bool test=false;
	
	unsigned int f=0;
		
	float mori[4][4];
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<this->countChildren;f++) {
		mcopy(m,mori);
		(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Casting Ray (Collision)" << endl;	// Message about debuging
	#endif
	
	return(test);

}





























LOD::LOD() {
	
	this->countChildren=0;
	
}

LOD *LOD::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	LOD *lod;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((LOD*)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
		
	lod = new LOD();
	lod->read(node);
		
	return lod;
	
}

void LOD::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (LOD)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;
	f=0;
	
	#ifdef DEBUG2
		cout << "m " << transformMatrix[0][0] << "  " << transformMatrix[0][1] << "  " << transformMatrix[0][2] << "  " << transformMatrix[0][3] << endl;
		cout << "  " << transformMatrix[1][0] << "  " << transformMatrix[1][1] << "  " << transformMatrix[1][2] << "  " << transformMatrix[1][3] << endl;
		cout << "  " << transformMatrix[2][0] << "  " << transformMatrix[2][1] << "  " << transformMatrix[2][2] << "  " << transformMatrix[2][3] << endl;
		cout << "  " << transformMatrix[3][0] << "  " << transformMatrix[3][1] << "  " << transformMatrix[3][2] << "  " << transformMatrix[3][3] << endl << endl;
	#endif
	
	float point[4];
	point[0]=0;
	point[1]=0;
	point[2]=0;
	point[3]=1;
	multipleMatrix(transformMatrix,point);
	
	#ifdef DEBUG2
	cout << "point : " << endl;
		cout << point[0] << " , " << point[1] << " , " << point[2] << " , " << point[3] << endl  << endl  << endl;
	#endif
	
	float distance;
	distance =   sqrt( (point[0]*point[0]) + (point[1]*point[1]) + (point[2]*point[2]) );
	
	#ifdef DEBUG2
		cout << "Distance betwen object and viewer = " << distance << endl;
	#endif
	
	if(this->counterRange!=0) {

		for(f=0;f<this->counterRange;f++) {
			if(distance<this->range[f]) {
				this->children[f]->follow();
				#ifdef DEBUG2
					cout << "Drawing : " << f << endl;
				#endif
				break;
			}
		}
		if(distance>=this->range[this->counterRange-1]) {
			this->children[this->counterRange]->follow();
			#ifdef DEBUG2
				cout << "Drawing : " << this->counterRange << endl;
			#endif
		}
	
	} else {
		this->children[0]->follow();
	}
	
}

void *LOD::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter
	DOMNodeList *children;			//* variable to hold the node children
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

	#ifndef XERCES
		char *ptrptr;
		char *ctemp;				// temporary variable
	#endif
	
	this->counterRange=0;
	this->range=NULL;
	
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "range")) {
			
			#ifdef XERCES  // actualy is the xerces parser 
				
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					
					x3d::x3DValues->MFFloat[this->counterRange++] = atof(XMLString::transcode(tok->nextToken()));
				
					if(this->counterRange>=Minimum_MFFloat) {
						cerr << "Error range bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			
			#else
			
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message," \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFFloat[this->counterRange++] = atof(ctemp);
					ctemp = strtok_r(NULL," \n",&ptrptr);
					if(this->counterRange>=Minimum_MFFloat) {
						cerr << "Error range bigger than buffer" << endl;
						return(NULL);;
					}
				}
			
			#endif
			
			
			this->range = new float[this->counterRange];
			if( this->range==NULL ) 
				cerr << "Memory alocation problem at LOD" << endl;
			for(unsigned short int indexCounter=0;indexCounter < this->counterRange;indexCounter++) {
				range[indexCounter] = x3d::x3DValues->MFFloat[indexCounter];
				#ifdef DEBUG1
					cout << "Range (" << indexCounter << ") = " << range[indexCounter] << endl;
				#endif
			}
		}
	}

	if(this->counterRange==0) {
			cout << "Warning rang not defined" << endl;
	}
	
	children = node->getChildNodes ();
	
	this->countChildren = x3d::getChildren(children,this->children);
	
	return(NULL);
}



bool LOD::ray(float o[4],float d[4],float m[4][4]) {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (LOD)" << endl;	// Message about debuging
	#endif
	
	bool test=false;
	
	unsigned int f=0;
		
	float mori[4][4];
	
	//-- This rotine follow each transform node --- //
	for(f=0;f<this->countChildren;f++) {
		mcopy(m,mori);
		(this->children[f]->ray(o,d,mori)?test=true:test=test); // nada inteligente
	}
	
	mcopy(m,this->transformMatrix);	// to be used in the follow
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Casting Ray (LOD)" << endl;	// Message about debuging
	#endif
	
	return(test);

}
