

#include "x3d.h"
#include "x3dTexturing.h"
#include "pthread.h"
#include "../math.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>


ImageTexture::ImageTexture() {
	
	this->couter_url=0;		// in the case of many texture files in one specification
	
	strcpy(this->url[0],"");	// default value
	this->repeatS=true; 		// default value
  	this->repeatT=true;			// default value
	
}


void ImageTexture::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (ImageTexture)" << endl;	// Message about debuging
	#endif

	glEnable(GL_TEXTURE_2D);

	#ifdef DEBUG2
		cout << "\t\t\t\t\tLoading texture name : \"" << this->url[0] <<"\"" << endl;
	#endif
	x3d::my_x3d->texture_bmp->readTexture(this->url[0],this->repeatS,this->repeatT); 	// falta passar o repeat
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (ImageTexture)" << endl;	// Message about debuging
	#endif
	
}


ImageTexture *ImageTexture::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	ImageTexture *imageTexture = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			imageTexture = (ImageTexture *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( imageTexture->ami("ImageTexture") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(imageTexture);
			
		} 
	}
		
	imageTexture = new ImageTexture();
	if(imageTexture==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(imageTexture->DEF,DEF2);
		setLink(imageTexture->DEF,imageTexture);			
	}
	
	imageTexture->read(node);
	
	return imageTexture;


}

void *ImageTexture::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
		
	attributes = node->getAttributes();
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
					cout << " Texture [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," \n");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
		} else 
		if(!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "repeatS")) {
			if (!strcasecmp( XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				this->repeatS=true; 
			} else {
				this->repeatS=false;
			}
		} else 
		if(!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "repeatT")) {
			if (!strcasecmp( XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				this->repeatT=true; 
			} else {
				this->repeatT=false;
			}
		}
	}
	
	if(!strcmp(this->url[0],"")) cout << "Url parameter not especified in image texture" << endl;

	return(NULL);

}

// Constructor
MovieTexture::MovieTexture() {
	
	url1=NULL;
	url2=NULL;
	
	startFrame=0;
	stopFrame=0;
	
	this->temp=1; // para remover
}

//
void MovieTexture::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (MovieTexture)" << endl;	// Message about debuging
	#endif
		
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	x3d::my_x3d->texture_bmp->readMovieTexture(this->url,0); 

	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (MovieTexture)" << endl;	// Message about debuging
	#endif

	
}

//
MovieTexture *MovieTexture::get(DOMNode *node) {
	
	
	unsigned int i; 					// variable to counter
	DOMNamedNodeMap *attributes;		// variable to hold the node attributes
	
	char DEF2[256];						// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	MovieTexture *movieTexture = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			movieTexture = (MovieTexture *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( movieTexture->ami("MovieTexture") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(movieTexture);
			
		} 
	}
		
	movieTexture = new MovieTexture();
	if(movieTexture==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(movieTexture->DEF,DEF2);
		setLink(movieTexture->DEF,movieTexture);			
	}
	
	movieTexture->read(node);
	
	return movieTexture;

}

void *MovieTexture::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {	
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url")) 
			url = XMLString::transcode (attributes->item(i)->getNodeValue());
		else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url")) 
			url1 = XMLString::transcode (attributes->item(i)->getNodeValue());
		else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url2")) 
			url2 = XMLString::transcode (attributes->item(i)->getNodeValue());
		else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "startFrame")) 
			startFrame = atoi(XMLString::transcode (attributes->item(i)->getNodeValue()));
		else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "stopFrame")) 
			stopFrame = atoi(XMLString::transcode (attributes->item(i)->getNodeValue()));
	}
	
	if ( url==NULL && ( url1==NULL || url2==NULL ) ) cout << "Url parameter not especified in movie texture" << endl;
	
	return(NULL);
}

// Constructor
TextureTransform::TextureTransform() {
	
	this->translation[0]=0;
	this->translation[1]=0;
	
	this->center[0]=0;
	this->center[1]=0;
	
	this->scale[0]=1;
	this->scale[1]=1;

	this->rotation=0;

}

void TextureTransform::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (TextureTransform)" << endl;	// Message about debuging
	#endif

	if(    
		
		(translation[0]!=0) ||
		(translation[1]!=0) ||
	
		(center[0]!=0) ||
		(center[1]!=0) ||
	
		(scale[0]!=1) ||
		(scale[1]!=1) ||
	
		(rotation!=0)
	
								 ) {
	
		glMatrixMode(GL_TEXTURE);
	
		glTranslatef(-center[0],-center[1],0);
	
		glScalef(scale[0],scale[1],1);
		glRotatef(rotation,0,0,1);
		glTranslatef(translation[0],translation[1],0);
	
		glTranslatef(center[0],center[1],0);
	
		glMatrixMode(GL_MODELVIEW);
	
	} else {
		
		#ifdef DEBUG1
			cout << "Nothing to do" << endl;
		#endif
		
	}
		
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (TextureTransform)" << endl;	// Message about debuging
	#endif

	
}


TextureTransform *TextureTransform::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	TextureTransform *textureTransform = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			textureTransform = (TextureTransform *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( textureTransform->ami("TextureTransform") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(textureTransform);
			
		} 
	}
		
	textureTransform = new TextureTransform();
	if(textureTransform==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(textureTransform->DEF,DEF2);
		setLink(textureTransform->DEF,textureTransform);			
	}
	
	textureTransform->read(node);
	
	return textureTransform;
}

void *TextureTransform::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes

	char *ptrptr;
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()), "center")) {
			center[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			center[1] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()), "translation")) {
			translation[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			translation[1] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()), "rotation")) {
			 rotation = 180*atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "))/PI;
		} else 
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()), "scale")) {
			scale[0] = atof(strtok_r(XMLString::transcode (attributes->item(i)->getNodeValue())," ",&ptrptr));
			scale[1] = atof(strtok_r(NULL," ",&ptrptr));
		}
		
	}
	
	return(NULL);

}

// Constructor
TextureCoordinate::TextureCoordinate() {
	
	value = NULL;
	
	f=0;						// counter to fill the variable coord

	//message=NULL;
	
}

//Destructor
TextureCoordinate::~TextureCoordinate() {
	
	delete value;
	
}

void TextureCoordinate::follow() {
	/*
	if(message!=NULL) {
		delete[] message;
		message=NULL;
	}
	*/
}


TextureCoordinate *TextureCoordinate::get(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	TextureCoordinate *textureCoordinate = NULL;					// temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			
			textureCoordinate = (TextureCoordinate *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue()));
			
			if( textureCoordinate->ami("TextureCoordinate") ) {
				cout << "USE of a different DEF type" << endl;
			}
			
			return(textureCoordinate);
			
		} 
	}
		
	textureCoordinate = new TextureCoordinate();
	if(textureCoordinate==NULL) { 	// checks if creation return null
		cerr << "Error on getting Group" << endl;
	}
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(textureCoordinate->DEF,DEF2);
		setLink(textureCoordinate->DEF,textureCoordinate);			
	}
	
	textureCoordinate->read(node);
	
	return textureCoordinate;

}


void *TextureCoordinate::read(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	value = new datavector;
	if(value==NULL) {
		cerr << "Error on TextureCoordinate" << endl;
	}
	
	#ifndef XERCES
		char *ctemp;					// temporary variable
		char *ptrptr;
	#endif
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "point")) {
				
			#ifdef XERCES  // actualy is the xerces parser 
			
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFVec3f[this->f++] = atof(XMLString::transcode(tok->nextToken()));
					#ifdef DEGUB
						cout << "Coordinate [ " << this->f-1 << " :" << x3d::x3DValues->MFVec3f[this->f-1] << endl;
					#endif
					if(this->f>=Minimum_MFVec3f) {
						cerr << "Error Texture values bigger than buffer" << endl;
						return(NULL);
					}	
				}	
			
				delete tok;
			
			#else
			
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message," \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFVec3f[this->f++] = atof(ctemp);
					ctemp = strtok_r(NULL," \n",&ptrptr);
					#ifdef DEGUB
						cout << "Coordinate [ " << f-1 << " :" << x3d::x3DValues->MFVec3f[f-1] << endl;
					#endif
					if(this->f>=Minimum_MFVec3f) {
						cerr << "Error Texture values bigger than buffer" << endl;
						return(NULL);;
					}
				}

			#endif

		}
	}

	this->value->aloc(f);
	
	unsigned int g;							// counter
	
	for(g=0;g<f;g++){
		value->data[g]=x3d::x3DValues->MFVec3f[g];
	}
	
	//delete[] tcoord;
	
	return(NULL);

}








// INVENTADA




CubeMapping::CubeMapping() {
	
	this->couter_urlFront=0;		// in the case of many texture files in one specification
	this->couter_urlBack=0;		// in the case of many texture files in one specification
	this->couter_urlTop=0;		// in the case of many texture files in one specification
	this->couter_urlBottom=0;		// in the case of many texture files in one specification
	this->couter_urlLeft=0;		// in the case of many texture files in one specification
	this->couter_urlRight=0;		// in the case of many texture files in one specification
	
	strcpy(this->urlFront[0],"");	// default value
	strcpy(this->urlBack[0],"");	// default value
	strcpy(this->urlTop[0],"");	// default value
	strcpy(this->urlBottom[0],"");	// default value
	strcpy(this->urlLeft[0],"");	// default value
	strcpy(this->urlRight[0],"");	// default value
	
	this->repeatS=true; 		// default value
  	this->repeatT=true;			// default value
	
}


void CubeMapping::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (CubeMapping Texture)" << endl;	// Message about debuging
	#endif

	
	//glEnable(GL_COLOR_MATERIAL);
	//glColor3f(1.0f,1.0f,1.0f);
	//glDisable(GL_COLOR_MATERIAL);    //verificar o que acontece
		
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\tLoading texture name : \"" << this->urlFront[0] <<"\"" << endl;
	#endif
	
	char files[6][256];
	
	strcpy(files[0],this->urlFront[0]);
	strcpy(files[1],this->urlBack[0]);
	strcpy(files[2],this->urlTop[0]);
	strcpy(files[3],this->urlBottom[0]);
	strcpy(files[4],this->urlLeft[0]);
	strcpy(files[5],this->urlRight[0]);
	
	//x3d::my_x3d->texture_bmp->readCubeMap(files,this->repeatS,this->repeatT); 	// falta passar o repeat
	x3d::my_x3d->texture_bmp->readCubeMap(files,false,false);
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (CubeMapping Texture)" << endl;	// Message about debuging
	#endif
	
}


CubeMapping *CubeMapping::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	CubeMapping *imageTexture;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((CubeMapping *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	imageTexture = new CubeMapping();
	imageTexture->read(node);
	
	return imageTexture;

}

void *CubeMapping::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
		
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlFront")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlFront[couter_urlFront],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlFront[couter_urlFront][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlFront[couter_urlFront],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlFront[couter_urlFront],"")) {
					couter_urlFront++;
				}
			}
		} else 
		
		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlBack")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlBack[couter_urlBack],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlBack[couter_urlBack][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlBack[couter_urlBack],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlBack[couter_urlBack],"")) {
					couter_urlBack++;
				}
			}
		} else 
		
		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlTop")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlTop[couter_urlTop],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlTop[couter_urlTop][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlTop[couter_urlTop],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlTop[couter_urlTop],"")) {
					couter_urlTop++;
				}
			}
		} else 
		
		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlBottom")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlBottom[couter_urlBottom],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlBottom[couter_urlBottom][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlBottom[couter_urlBottom],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlBottom[couter_urlBottom],"")) {
					couter_urlBottom++;
				}
			}
		} else 
		
		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlLeft")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlLeft[couter_urlLeft],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlLeft[couter_urlLeft][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlLeft[couter_urlLeft],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlLeft[couter_urlLeft],"")) {
					couter_urlLeft++;
				}
			}
		} else 
		
		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "urlRight")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->urlRight[couter_urlRight],tmp1+1,((int)(strlen(tmp1)))-2);
					this->urlRight[couter_urlRight][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->urlRight[couter_urlRight],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->urlRight[couter_urlRight],"")) {
					couter_urlRight++;
				}
			}
		} else 
		
		
		
		
		
		
		if(!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "repeatS")) {
			if (!strcasecmp( XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				this->repeatS=true; 
			} else {
				this->repeatS=false;
			}
		} else 
		if(!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "repeatT")) {
			if (!strcasecmp( XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				this->repeatT=true; 
			} else {
				this->repeatT=false;
			}
		}
	}
	
	if(!strcmp(this->urlFront[0],"")) cout << "Url parameter not especified in image texture" << endl;
	if(!strcmp(this->urlBack[0],"")) cout << "Url parameter not especified in image texture" << endl;
	if(!strcmp(this->urlTop[0],"")) cout << "Url parameter not especified in image texture" << endl;
	if(!strcmp(this->urlBottom[0],"")) cout << "Url parameter not especified in image texture" << endl;
	if(!strcmp(this->urlLeft[0],"")) cout << "Url parameter not especified in image texture" << endl;
	if(!strcmp(this->urlRight[0],"")) cout << "Url parameter not especified in image texture" << endl;

	return(NULL);

}
