
// It was necessary in the orad (RedHat 8.0)
#define __EMX__

// It is necessary to activete the advanced prototypes of OpenGL and GLX (ARB Functions)
#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

// Includes do OpenGL

#ifdef GLEW
	//-- GLSL
	#include <GL/glew.h>
#endif

#include <GL/gl.h>		// default opengl header
#include <GL/glext.h>	// extensions for opengl

#include "x3d.h"
#include "x3dRendering.h"

// Xerces includes
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>


#include <GL/glx.h>
#include <GL/glut.h>


#define gp(p) glXGetProcAddressARB((const GLubyte *) p)


static PFNGLGENBUFFERSARBPROC glGenBuffersARB_ptr;
static PFNGLBINDBUFFERARBPROC glBindBufferARB_ptr;
static PFNGLBUFFERDATAARBPROC glBufferDataARB_ptr;


X3DGeometryNode::X3DGeometryNode() {
	
}

Color::Color() {
	
	value = new datavector;
	
	numberOfElements=0;						// counter to fill the variable coord

}

Color::~Color() {
	
	delete value;
	
}


void Color::follow() {
	
}

Color *Color::get(DOMNode *node) {
	Color *color = new Color();
	Color *color2;
	
	color2 = (Color *)color->read(node);
	
	if(color2!=NULL) {
		delete color;
		return color2;
	} 
	
	return color;

}


void *Color::read(DOMNode *node) {

	unsigned int i; 		// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	#ifndef XERCES
		char *ctemp;			// temporary variable	
		char *ptrptr;
	#endif
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "color")) {

			#ifdef XERCES  // actualy is the xerces parser 
			
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFColor[this->numberOfElements++] = atof(XMLString::transcode(tok->nextToken()));
					#ifdef DEGUB
						cout << "Coordinate [ " << this->numberOfElements-1 << " :" << x3d::x3DValues->MFVec3f[this->numberOfElements-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFVec3f) {
						cerr << "Error Coordinates bigger than buffer" << endl;
						return(NULL);
					}	
				}	
			
				delete tok;
				
			#else

				if(XMLString::stringLen(attributes->item(i)->getNodeValue()) >= Size_X3D_Message) {
					cout << " Message exced the maximun size " << endl;
				}
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFColor[this->numberOfElements++] = atof(ctemp);
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					#ifdef DEGUB
						cout << "Coordinate [ " << f-1 << " :" << x3d::x3DValues->MFColor[f-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFColor) {
						cerr << "Error Colors bigger than buffer" << endl;
						return(NULL);;
					}
				}

			#endif
		
		}
	}

	this->value->aloc(numberOfElements);
	
	unsigned int g;							// counter
	
	for(g=0;g<numberOfElements;g++){
		value->data[g]=x3d::x3DValues->MFColor[g];
	}
	
	return(NULL);

}


Coordinate::Coordinate() {
	
	this->value = new datavector;
	
	this->numberOfElements=0;						// counter to fill the variable coord
	
}

Coordinate::~Coordinate() {
	
	delete this->value;
	
}

void Coordinate::follow() {
	
}

Coordinate *Coordinate::get(DOMNode *node) {
	Coordinate *coordinate = new Coordinate();
	Coordinate *coordinate2;
	
	coordinate2 = (Coordinate *)coordinate->read(node);
	
	if(coordinate2!=NULL) {
		delete coordinate;
		return coordinate2;
	} 
	
	return coordinate;
	
}

void *Coordinate::read(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	#ifndef XERCES
		char *ctemp;					// temporary variable
		char *ptrptr;
	#endif
		
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "point")) {
			
			#ifdef XERCES  // actualy is the xerces parser 
			
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFVec3f[this->numberOfElements++] = atof(XMLString::transcode(tok->nextToken()));
					#ifdef DEGUB
						cout << "Coordinate [ " << this->numberOfElements-1 << " :" << x3d::x3DValues->MFVec3f[this->numberOfElements-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFVec3f) {
						cerr << "Error Coordinates bigger than buffer" << endl;
						return(NULL);
					}	
				}	
			
				delete tok;
				
			#else
				
				#ifdef DEGUB
					cout << "String Size " << XMLString::stringLen(attributes->item(i)->getNodeValue()) << endl;
				#endif
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFVec3f[this->numberOfElements++] = atof(ctemp);
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					#ifdef DEGUB
						cout << "Coordinate [ " << this->numberOfElements-1 << " :" << x3d::x3DValues->MFVec3f[this->numberOfElements-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFVec3f) {
						cerr << "Error Coordinates bigger than buffer" << endl;
						return(NULL);
					}
				}
				
			#endif
			
			
		}
	}
		
	#ifdef DEGUB1
		cout << "Number of Elements = " << this->numberOfElements << endl;
	#endif
	
	this->value->aloc(this->numberOfElements);
	
	int g;							// counter
	
	for(g=0;g<this->numberOfElements;g++){
		value->data[g]=x3d::x3DValues->MFVec3f[g];
	}
	
	//delete[] tcoord;
	
	return(NULL);

}

IndexedLineSet::IndexedLineSet() {

	coordIndex = new int[Minimum_MFVec3f];  //verificar
	if(coordIndex==NULL) cerr << "Memory alocation problem at temporary coordinate values" << endl;
	
	this->f=0;						// counter to fill the variable coord
	this->counterColor=0;				// counter to fill the variable color
	
	this->coordinate=NULL;			// 
	this->color=NULL;			// 
	
	this->colorPerVertex = new SFBool;
	
	this->firstPass=true;			// for drawing
	
}

IndexedLineSet::~IndexedLineSet() {
		
}

bool IndexedLineSet::ray(float o2[4], float d2[4], float m2[4][4]) {
	return(false);
}

void IndexedLineSet::follow() {
	
}

void IndexedLineSet::draw() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (IndexedLineSet)" << endl;	// Message about debuging
	#endif
	
	double *colorArray;
	
	if(firstPass) {
	
		if(coordinate!=NULL) {
			coordinate->follow();
		}

		if(color!=NULL) {
			color->follow();
		}

		#ifdef DEBUG3
			cout << "Setting functions" << endl;
		#endif
		glGenBuffersARB_ptr = (PFNGLGENBUFFERSARBPROC) gp("glGenBuffersARB");
		glBindBufferARB_ptr = (PFNGLBINDBUFFERARBPROC) gp("glBindBufferARB");
		glBufferDataARB_ptr = (PFNGLBUFFERDATAARBPROC) gp("glBufferDataARB");
			
		#ifdef DEBUG3
			cout << "Generating Buffer" << endl;
		#endif
		glGenBuffersARB_ptr( 1, &this->m_nVBOs );
			
		#ifdef DEBUG3
			cout << "Binding Buffer" << endl;
		#endif
		glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->m_nVBOs);
			
		#ifdef DEBUG3
			cout << "Transmiting data to buffer" << endl;
		#endif
		glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, this->coordinate->value->size*sizeof(double), this->coordinate->value->data, GL_STATIC_DRAW_ARB);
		
		
		if(color!=NULL) {
		
			if(*this->colorPerVertex) {
		
				colorArray = this->color->value->data;
			
			} else {
			
				#ifdef DEBUG1
				cout << "ColorPervertex False" << endl;
				#endif
			
				colorArray = new double[this->coordinate->value->size];
			
				int h,j; // counter
				int g=0; // counter
	
				int counterC=0;
			
				while(g<f) {
					h=g;j=0;
					while(coordIndex[h]!=-1) {
					
						colorArray[(coordIndex[h]*3)+0] = this->color->value->data[(colorIndex[counterC]*3)+0];
						colorArray[(coordIndex[h]*3)+1] = this->color->value->data[(colorIndex[counterC]*3)+1];
						colorArray[(coordIndex[h]*3)+2] = this->color->value->data[(colorIndex[counterC]*3)+2];
	
						#ifdef DEBUG1
							cout << "color [0] "  << colorArray[(coordIndex[h]*3)+0] <<
								     "  [1] "  << colorArray[(coordIndex[h]*3)+1] <<
									 "  [2] "  << colorArray[(coordIndex[h]*3)+2] << endl;
						#endif
					
						h++;j++;
					
					}
				
					g=h+1;
				
					counterC++;
				
				}
			
			}
		
		
			#ifdef DEBUG3
				cout << "Generating Buffer" << endl;
			#endif
			glGenBuffersARB_ptr( 1, &this->colorVBOs);
			
			#ifdef DEBUG3
				cout << "Binding Buffer" << endl;
			#endif
			glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->colorVBOs);
			
			#ifdef DEBUG3
				cout << "Transmiting data to buffer" << endl;
			#endif
			glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, this->coordinate->value->size*sizeof(double), colorArray, GL_STATIC_DRAW_ARB);
		
		
		}
		
		this->firstPass=false;
			
		#ifdef DEBUG3
			cout << "Finishing VBO" << endl;
		#endif
		
		
	}
		
	glEnableClientState(GL_VERTEX_ARRAY);
	#ifdef DEBUG3
		cout << "Binding Buffer" << endl;
	#endif
	glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->m_nVBOs);
	glVertexPointer(3, GL_DOUBLE, 0, (char *) NULL );
	
	if(color!=NULL) {
	
		glEnable(GL_COLOR_MATERIAL);   // to make colors appear
			
		glEnableClientState(GL_COLOR_ARRAY);
		#ifdef DEBUG3
			cout << "Binding Buffer" << endl;
		#endif
		glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->colorVBOs);
		glColorPointer(3, GL_DOUBLE, 0, (char *) NULL );
		
	} else {
		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_LIGHTING);
	}
	
	int h,j; // counter
	int g=0; // counter
	
	#ifdef DEBUG1
		cout << "Drawing Line" << endl;
	#endif
		
	while(g<f) {
		h=g;j=0;
		while(coordIndex[h]!=-1) {
			#ifdef DEBUG1
				cout << "CoordIndex " << coordIndex[h] << endl;
			#endif
			h++;j++;
		}
		#ifdef DEBUG1
			cout << "Elements " << j << endl;
		#endif
		glDrawElements(GL_LINE_STRIP, j, GL_UNSIGNED_INT, &coordIndex[g]);
		g=h+1;
	
	}
		
	glEnable(GL_LIGHTING);
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (IndexedFaceSet)" << endl;	// Message about debuging
	#endif

	
}


IndexedLineSet *IndexedLineSet::get(DOMNode *node) {
	IndexedLineSet *indexedLineSet = new IndexedLineSet();
	IndexedLineSet *indexedLineSet2;
	
	indexedLineSet2 = (IndexedLineSet *)indexedLineSet->read(node);
	
	if(indexedLineSet2!=NULL) {
		delete indexedLineSet;
		return indexedLineSet2;
	} 
	
	return indexedLineSet;

}

void *IndexedLineSet::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading IndexedLineSet" << endl;
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	DOMNodeList *children;			// variable to hold the node children
	char *ctemp;					// temporary variable

	
	#ifndef XERCES  // actualy is the xerces parser 
		char *ptrptr;
	#endif
	
	char* message;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"colorPerVertex")) {
			if (!strcasecmp(XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->colorPerVertex=true;
			} else {
				*this->colorPerVertex=false;
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "coordIndex")) {
			message = XMLString::transcode(attributes->item(i)->getNodeValue());
			ctemp = strtok(message,", \n");
			while (ctemp != NULL) {
				coordIndex[f++] = atoi(ctemp);
				ctemp = strtok(NULL,", \n");
				if(this->f>=Minimum_MFVec3f) { // corigir
					cerr << "Error IndexedLineSet bigger than buffer" << endl;
					return(NULL);;
				}				
			}
			if(coordIndex[f-1]!=-1) {
				coordIndex[f++]=-1;
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "colorIndex")) {

			#ifdef XERCES  // actualy is the xerces parser 
				
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFInt32[this->counterColor++] = atoi(XMLString::transcode(tok->nextToken()));
				
					if(x3d::x3DValues->MFInt32[this->counterColor] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					if(this->counterColor>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			
			#else
			
				if(XMLString::stringLen(attributes->item(i)->getNodeValue()) >= Size_X3D_Message) {
					cout << " Message exced the maximun size " << endl;
				}
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFInt32[this->counterColor++] = atoi(ctemp);
					if(x3d::x3DValues->MFInt32[this->counterColor] < -1) {
						cout << "Warning value of ColorIndex outside the range" << endl;
					}
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					if(this->counterColor>=Minimum_MFInt32) {
						cerr << "Error colorIndex bigger than buffer" << endl;
						return(NULL);;
					}
				}

			#endif


			if(x3d::x3DValues->MFInt32[this->counterColor-1]!=-1) {
				x3d::x3DValues->MFInt32[this->counterColor++]=-1;
			}
			this->colorIndex = new int[this->counterColor];
			//this->set_colorIndex = this->colorIndex;				// just a copy
			if( colorIndex==NULL ) 
				cerr << "Memory alocation problem at indexedFaceSet" << endl;
			for(int indexCounter=0;indexCounter < this->counterColor;indexCounter++) {
				colorIndex[indexCounter] = x3d::x3DValues->MFInt32[indexCounter];
			}
	
		}
	}

	#ifdef DEBUG1
		cout << "Reading children" << endl;
	#endif
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Coordinate")) {
				coordinate = Coordinate::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Color")) {
				color = Color::get(children->item(i));
			}
		}
	}
	
	#ifdef DEBUG1
		cout << "Leaving Reding IndexedLineSet" << endl;
	#endif
	return(NULL);
	
}



Normal::Normal() {
	
	value = new datavector;
	
	numberOfElements=0;						// counter to fill the variable coord

}

Normal::~Normal() {
	
	delete value;
	
}


void Normal::follow() {

}


Normal *Normal::get(DOMNode *node) {
	Normal *normal = new Normal();
	Normal *normal2;
	
	normal2 = (Normal *)normal->read(node);
	
	if(normal2!=NULL) {
		delete normal;
		return normal2;
	} 
	
	return normal;
	
}



void *Normal::read(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	#ifndef XERCES	
		char *ctemp;					// temporary variable
		char *ptrptr;
	#endif
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()), "vector")) {
			
			#ifdef XERCES  // actualy is the xerces parser 
			
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFVec3f[this->numberOfElements++] = atof(XMLString::transcode(tok->nextToken()));
					#ifdef DEGUB
						cout << "Coordinate [ " << this->numberOfElements-1 << " :" << x3d::x3DValues->MFVec3f[this->numberOfElements-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFVec3f) {
						cerr << "Error Coordinates bigger than buffer" << endl;
						return(NULL);
					}	
				}	
			
				delete tok;
				
			#else

				if(XMLString::stringLen(attributes->item(i)->getNodeValue()) >= Size_X3D_Message) {
					cout << " Message exced the maximun size " << endl;
				}
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFVec3f[this->numberOfElements++] = atof(ctemp);
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					#ifdef DEGUB
						cout << "Coordinate [ " << f-1 << " :" << x3d::x3DValues->MFVec3f[f-1] << endl;
					#endif
					if(this->numberOfElements>=Minimum_MFVec3f) {
						cerr << "Error Normals bigger than buffer" << endl;
						return(NULL);;
					}
				}

			#endif
		}
	}
	
	this->value->aloc(numberOfElements);
	
	unsigned int g;							// counter
	
	for(g=0;g<numberOfElements;g++){
		value->data[g]=x3d::x3DValues->MFVec3f[g];
	}
	
	return(NULL);
	
}


PointSet::PointSet() {
	
	f=0;						// counter to fill the variable coord
	
	colorNumber=0;
	coordinateNumber=0;
	
	coordinate=NULL;
	
	color=NULL;
	
}
	
PointSet::~PointSet() {
	
	
	if(coordinateNumber) {
		delete coordinate;
	}
	
	if(colorNumber) {
		delete color;
	}
	
}

bool PointSet::ray(float o2[4], float d2[4], float m2[4][4]) {
	return(false);
}

void PointSet::follow() {
}

void PointSet::draw() {
	
	if(coordinate!=NULL) {
		coordinate->follow();
	}

	if(color!=NULL) {
		color->follow();
	}
	
	glPointSize(1);
	
	glDisable(GL_TEXTURE_2D);
		
	glBegin(GL_POINTS);
	for(f=0;f<coordinateNumber;f=f+3) {
		
		if(colorNumber>3) {
			//#ifdef JINX_DOUBLE
				//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmpDiffuseColor);   // trocar em oportunidade
				glColor3d(color->value->data[f],color->value->data[f+1],color->value->data[f+2]);
			//#else
			//	glColor3f(color->value->data[f],color->value->data[f+1],color->value->data[f+2]);
			//#endif
		} else
		if(colorNumber==3) { 
			//#ifdef JINX_DOUBLE
				//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmpDiffuseColor);   // trocar em oportunidade
				glColor3d(color->value->data[0],color->value->data[1],color->value->data[2]); 
			//#else
			//	glColor3f(color->value->data[0],color->value->data[1],color->value->data[2]); 
			//#endif
		}
		//#ifdef JINX_DOUBLE
			glVertex3dv(&coordinate->value->data[f]);
		//#else
		//	glVertex3fv(&coordinate->value->data[f]);
		//#endif
	}
	glEnd();
		
}
	

PointSet *PointSet::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	PointSet *pointSet;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((PointSet *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	pointSet = new PointSet();
	pointSet->read(node);
		
	return pointSet;
	
}

void *PointSet::read(DOMNode *node) {
	unsigned int i; 				// variable to counter
	DOMNodeList *children;			// variable to hold the node children
	
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		}// else
		//if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
		//	return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		//} 
	}

	children = node->getChildNodes();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Coordinate")) {
				coordinate = Coordinate::get(children->item(i));
				coordinateNumber = coordinate->getNumberOfElements();
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Color")) {
				color = Color::get(children->item(i));
				colorNumber = color->getNumberOfElements();
			} 
		}
	}
	
	#ifdef DEBUG
		float sizes[2];
		float step;
		glGetFloatv(GL_POINT_SIZE_RANGE,sizes);
		glGetFloatv(GL_POINT_SIZE_GRANULARITY,&step);
		cout << "Max and Min point size " << sizes[0] << " , " << sizes[1] << " Granularity " << step << endl;
	#endif
	
	if(colorNumber) {
		if(colorNumber==3) {
			cout << "Three colors selected" << endl;
		} else
		if(colorNumber!=coordinateNumber) {
			cerr << "Color and coordinate not matching" << endl;
		}
	}

		
	#ifdef DEBUG
		cout << "Coordinate Number " << coordinateNumber << endl;
		cout << "Color Number " << colorNumber << endl;
	#endif

	return(NULL);
	
}
