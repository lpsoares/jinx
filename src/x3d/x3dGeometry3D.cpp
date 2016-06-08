
// It was necessary in the orad (RedHat 8.0)
#define __EMX__

// OpenGL Include's
#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

#ifdef GLEW
	//--- GLSL
	#include <GL/glew.h>
#endif


#include <GL/gl.h>
#include <GL/glext.h>

// Jinx Include's
#include "x3d.h"
#include "x3dGeometry3D.h"

#include "../math.h"
#include "../Vector.h"
#include "../render/render.h"
#include "x3dTexturing.h"

// Xerces Include's
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>

#include <GL/glx.h>

#define gp(p) glXGetProcAddressARB((const GLubyte *) p)

static PFNGLGENBUFFERSARBPROC glGenBuffersARB_ptr;
static PFNGLBINDBUFFERARBPROC glBindBufferARB_ptr;
static PFNGLBUFFERDATAARBPROC glBufferDataARB_ptr;


// Constructor
Box::Box() {
	
	this->size = new SFVec3f[3];	// alloc size
	*this->size[0] = 1.0;			// default value
	*this->size[1] = 1.0;			// default value
	*this->size[2] = 1.0;			// default value

	this->solid = new SFBool;		// alloc solid
	*this->solid=true;				// default value

	this->cast=false;				// to ray tracing
	
}

// Destructor
Box::~Box() {

	delete[] this->size;	// cleaning memory for size
	delete this->solid;		// cleaning memory for solid
	
}

// ray casting fucntion of Box
bool Box::ray(float o2[4], float d2[4], float m2[4][4]) {
	
	#ifdef DEBUG
		cout << "Box Ray routine " << endl;
	#endif
		
	float o[4];
	float d[4];

	o[0] = o2[0];
	o[1] = o2[1];
	o[2] = o2[2];
	o[3] = o2[3];

	d[0] = d2[0];
	d[1] = d2[1];
	d[2] = d2[2];
	d[3] = d2[3];

	float matrix[4][4];

	float matrix2[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	
	float m3[4][4];

	scalematrix(m3, *this->size[0], *this->size[1], *this->size[2]);	multipleMatrix(m2,m3);
	mcopy(m3,m2);
		
	matrix2[0 ]=m2[0][0];	matrix2[1 ]=m2[0][1];	matrix2[2 ]=m2[0][2];	matrix2[3 ]=m2[0][3];
	matrix2[4 ]=m2[1][0];	matrix2[5 ]=m2[1][1];	matrix2[6 ]=m2[1][2];	matrix2[7 ]=m2[1][3];
	matrix2[8 ]=m2[2][0];	matrix2[9 ]=m2[2][1];	matrix2[10]=m2[2][2];	matrix2[11]=m2[2][3];
	matrix2[12]=m2[3][0];	matrix2[13]=m2[3][1];	matrix2[14]=m2[3][2];	matrix2[15]=m2[3][3];
	
	float matrix3[16];
		
	int ok = m4_inverse( matrix3, matrix2 );
	if(!ok) {
		this->cast=false;
		return false; 	// nao encontrou
	}
	matrix[0][0]=matrix3[ 0];	matrix[0][1]=matrix3[ 1];	matrix[0][2]=matrix3[ 2];	matrix[0][3]=matrix3[ 3];
	matrix[1][0]=matrix3[ 4];	matrix[1][1]=matrix3[ 5];	matrix[1][2]=matrix3[ 6];	matrix[1][3]=matrix3[ 7];
	matrix[2][0]=matrix3[ 8];	matrix[2][1]=matrix3[ 9];	matrix[2][2]=matrix3[10];	matrix[2][3]=matrix3[11];
	matrix[3][0]=matrix3[12];	matrix[3][1]=matrix3[13];	matrix[3][2]=matrix3[14];	matrix[3][3]=matrix3[15];
	
	multipleMatrix(matrix,d);
	multipleMatrix(matrix,o);
	
	float delta, raiz, t;
  
	Vector u = Vector(d[0],d[1],d[2]);				// valor de u da equacao
	u.normalize();
	Vector v = Vector(-o[0],-o[1],-o[2]);	// valor de v da equacao
	//Vector v = Vector(center - R.P);

	float  tmp = (v - (u*v)*u).length();
	delta = 1*1 - (tmp*tmp);
  
	if (delta < 0) { // para nao calcular raiz negativa
		this->cast=false;
		return false; 	// nao encontrou
	}
  
	raiz = sqrt(delta); // raiz da equacao
  
	if ((t = (u*v) - raiz) > SMALL) { // caso o primeiro ja seja maior que 0
		this->cast=true;
		return true;
	} else if ((t = (u*v) + raiz) > SMALL) { // senao t é o segundo
		this->cast=true;
		return true;
	}
  
	this->cast=false;
	return false; // nao encontro

}

// 
Box *Box::get(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	Box *box;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Box *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	box = new Box();
	box->read(node);
	
	return box;
	
}

// reading function of Box
void *Box::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (Box) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	attributes = node->getAttributes();
	for(i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode (attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "size")) {
			*this->size[0] = atof(strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ,"))/2;
			*this->size[1] = atof(strtok(NULL," ,"))/2;
			*this->size[2] = atof(strtok(NULL," ,"))/2;
			if( (*this->size[0]<0) || (*this->size[1]<0) || (*this->size[2]<0) ) {
				cout << "Warning value of CoordIndex outside the range" << endl;
			}
			
			if( (*this->size[0]==0) && (*this->size[1]==0) && (*this->size[2]==0) ) {
				cout << "Warning box dimension equals zero " << endl;
			}
		} else 
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"solid")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->solid=true; 
			} else {
				*this->solid=false;
			}
		} else 
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
		     (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
				cout << "Box member not recognized : ";
				cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}
	}

	return(NULL);
	
}

void Box::follow() {}

void Box::draw() {


	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Box)" << endl;	// Message about debuging
	#endif
	
	if(*this->solid) { 			//define backface culling
		glEnable(GL_CULL_FACE);
	}
	
	glPushMatrix();
	
	glScalef(*this->size[0],*this->size[1],*this->size[2]);

	glBegin(GL_QUADS);
	
	// Front Face
	glNormal3f( 0.0, 0.0, 1.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f( 1.0,-1.0, 1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f( 1.0, 1.0, 1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-1.0,-1.0, 1.0);

	// Back Face
	glNormal3f( 0.0, 0.0,-1.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-1.0,-1.0,-1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(-1.0, 1.0,-1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f( 1.0, 1.0,-1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f( 1.0,-1.0,-1.0);
	
	// Right Face
	glNormal3f( 1.0, 0.0, 0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f( 1.0,-1.0,-1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f( 1.0, 1.0,-1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f( 1.0, 1.0, 1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f( 1.0,-1.0, 1.0);

	// Left Face
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-1.0,-1.0, 1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-1.0, 1.0,-1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-1.0,-1.0,-1.0);
	
	// Top Face
	glNormal3f( 0.0, 1.0, 0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f( 1.0, 1.0, 1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f( 1.0, 1.0,-1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-1.0, 1.0,-1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	
	// bottom Face
	glNormal3f( 0.0,-1.0, 0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f( 1.0,-1.0,-1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f( 1.0,-1.0, 1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-1.0,-1.0, 1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-1.0,-1.0,-1.0);
	
	glEnd();

	if( (cast) && (Render::my_rrr->wandRay) ) {
		
		glPushAttrib(GL_ENABLE_BIT);
		
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LINE_STIPPLE);
		
		glColor3f(1,0,0);
		
		glutWireCube(2.1);
		
		glPopAttrib();
		
		//cout << "Casting " << DEF << endl;
		
		
	}

	glPopMatrix();
	
	if(*this->solid) { // define backface culling (backing)
		glDisable(GL_CULL_FACE);
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Box)" << endl;	// Message about debuging
	#endif
	
	
}

// constructor
Cone::Cone() {
		
	//SFNode  [in,out] metadata     NULL [X3DMetadataObject]
	
	this->bottom       = new SFBool;
	*this->bottom      = true;
	
	this->bottomRadius = new SFFloat;
	*this->bottomRadius = 1;
	
	this->height       = new SFFloat;
	*this->height      = 2;
	
	this->side         = new SFBool;
	*this->side         = true;
	
	this->solid        = new SFBool;
	*this->solid        = true;

	this->cast=false;
	
}

// destructor
Cone::~Cone() {

	delete this->bottom;
	delete this->bottomRadius;
	delete this->height;
	delete this->side;
	delete this->solid;

}

// ray casting fucntion of Cone
bool Cone::ray(float o2[4], float d2[4], float m2[4][4]) {

	#ifdef DEBUG
		cout << "Cone Ray routine " << endl;
	#endif
		
	float o[4];
	float d[4];

	o[0] = o2[0];
	o[1] = o2[1];
	o[2] = o2[2];
	o[3] = o2[3];

	d[0] = d2[0];
	d[1] = d2[1];
	d[2] = d2[2];
	d[3] = d2[3];
	
	float matrix[4][4];

	float matrix2[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
		
	matrix2[0 ]=m2[0][0];	matrix2[1 ]=m2[0][1];	matrix2[2 ]=m2[0][2];	matrix2[3 ]=m2[0][3];
	matrix2[4 ]=m2[1][0];	matrix2[5 ]=m2[1][1];	matrix2[6 ]=m2[1][2];	matrix2[7 ]=m2[1][3];
	matrix2[8 ]=m2[2][0];	matrix2[9 ]=m2[2][1];	matrix2[10]=m2[2][2];	matrix2[11]=m2[2][3];
	matrix2[12]=m2[3][0];	matrix2[13]=m2[3][1];	matrix2[14]=m2[3][2];	matrix2[15]=m2[3][3];
		
	float matrix3[16];
	
	int ok = m4_inverse( matrix3, matrix2 );
	if(!ok) {
		this->cast=false;
		return false; 	// nao encontrou
	}
	
	matrix[0][0]=matrix3[ 0];	matrix[0][1]=matrix3[ 1];	matrix[0][2]=matrix3[ 2];	matrix[0][3]=matrix3[ 3];
	matrix[1][0]=matrix3[ 4];	matrix[1][1]=matrix3[ 5];	matrix[1][2]=matrix3[ 6];	matrix[1][3]=matrix3[ 7];
	matrix[2][0]=matrix3[ 8];	matrix[2][1]=matrix3[ 9];	matrix[2][2]=matrix3[10];	matrix[2][3]=matrix3[11];
	matrix[3][0]=matrix3[12];	matrix[3][1]=matrix3[13];	matrix[3][2]=matrix3[14];	matrix[3][3]=matrix3[15];
	
	multipleMatrix(matrix,d);
	multipleMatrix(matrix,o);
	
	float delta, raiz, t;
  
	Vector u = Vector(d[0],d[1],d[2]);				// valor de u da equacao
	u.normalize();
	Vector v = Vector(-o[0],-o[1],-o[2]);	// valor de v da equacao
	//Vector v = Vector(center - R.P);

	float  tmp = (v - (u*v)*u).length();
	delta = (*this->height/2)*(*this->height/2) - (tmp*tmp);
  
	if (delta < 0) { // para nao calcular raiz negativa
		this->cast=false;
		return false; 	// nao encontrou
	}
  
	raiz = sqrt(delta); // raiz da equacao
  
	if ((t = (u*v) - raiz) > SMALL) { // caso o primeiro ja seja maior que 0
		this->cast=true;
		return true;
	} else if ((t = (u*v) + raiz) > SMALL) { // senao t é o segundo
		this->cast=true;
		return true;
	}
  
	this->cast=false;
	return false; // nao encontro

	
}


Cone *Cone::get(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Cone *cone;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Cone *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	cone = new Cone();
	cone->read(node);
	
	return cone;
	
}

// reading function of Cone
void *Cone::read(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode (attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "bottomRadius")) {
			*this->bottomRadius = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else 
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "height")) {
			*this->height = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
 		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "side")) {
			if ( !strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()) , "TRUE")) {
				*this->side=true; 
			} else {
				*this->side=false;
			}
		} else
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"bottom")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->bottom=true; 
			} else {
				*this->bottom=false;
			}
		} else 
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"solid")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				*this->solid=true; 
			} else {
				*this->solid=false;
			}
		} else 
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Cone member not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}

	}
	
	return(NULL);
	
}


void Cone::follow() {}

void Cone::draw() {
	
	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Cone)" << endl;	// Message about debuging
	#endif
	
	float radius1;	// this is based on the cos
	float radius2;	// this is based on the sin
	
	int i;
	
	float f=0;
	
	if(*this->solid) { 
		glEnable(GL_CULL_FACE);
	}
	
	if(*this->side) {
		
		for(i=0; i<=QUALITY; i++) {
		
			glBegin(GL_TRIANGLE_STRIP);
			
			f+=2*PI/QUALITY;
		
			radius1=*this->bottomRadius*cos(f);
			radius2=*this->bottomRadius*sin(f);

			glNormal3f( (radius1/(*this->bottomRadius) ) ,0, (radius2/(*this->bottomRadius) ));
			glTexCoord2f(f/(2*PI),0.0);
			glVertex3f(radius1, -(*this->height/2), radius2);
		
			glNormal3f( 0 ,1, 0);
			glTexCoord2f(f/(2*PI),1.0);
			glVertex3f(0,  (*this->height/2), 0);
		
			radius1=*this->bottomRadius*cos(f+2*PI/QUALITY);
			radius2=*this->bottomRadius*sin(f+2*PI/QUALITY);
		
			glNormal3f( (radius1/(*this->bottomRadius) ) ,0, (radius2/ (*this->bottomRadius) ));
			glTexCoord2f((f+2*PI/QUALITY)/(2*PI),0.0);
			glVertex3f(radius1, -(*this->height/2), radius2 );
		
			glEnd();	
		
		}
		
	}
	
	if(*this->bottom) {  			// this is the bottom cap
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f( 0.0f ,-1.0f, 0.0f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.0f, -(*this->height/2), 0.0f);
		
		f=0;
			
		for(i=0; i<=QUALITY; i++) {
						
			f+=2.0*PI/QUALITY;
				
			radius1=*this->bottomRadius*cos(f);
			radius2=*this->bottomRadius*sin(f);

			glNormal3f( 0.0f ,-1.0f, 0.0f);
			
			glTexCoord2f( (((radius1/(*this->bottomRadius))/2)+0.5) , (((radius2/(*this->bottomRadius))/2)+0.5) );
			glVertex3f(radius1, -(*this->height/2), radius2);
		}
		
		glEnd();
	}

	if( (this->cast) && (Render::my_rrr->wandRay) ) {
		glDisable(GL_LIGHTING);
		glColor3f(1.0f,0.0f,0.0f);
		glutWireCube(*this->height);
		glEnable(GL_LIGHTING);
	}

	
	if(*this->solid) {
		glDisable(GL_CULL_FACE);
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Cone)" << endl;	// Message about debuging
	#endif
	
}


Cylinder::Cylinder() {

	this->bottom=true;
	this->side=true;
	this->top=true;
	
	this->radius=1;
	this->height=2;
	
	this->cast=false;

}

bool Cylinder::ray(float o2[4], float d2[4], float m2[4][4]) {

	#ifdef DEBUG
		cout << "Cylinder Ray routine " << endl;
	#endif
	
	float o[4];
	float d[4];
	
	o[0] = o2[0];
	o[1] = o2[1];
	o[2] = o2[2];
	o[3] = o2[3];

	d[0] = d2[0];
	d[1] = d2[1];
	d[2] = d2[2];
	d[3] = d2[3];
	
	float matrix[4][4];

	float matrix2[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
		
	matrix2[0 ]=m2[0][0];	matrix2[1 ]=m2[0][1];	matrix2[2 ]=m2[0][2];	matrix2[3 ]=m2[0][3];
	matrix2[4 ]=m2[1][0];	matrix2[5 ]=m2[1][1];	matrix2[6 ]=m2[1][2];	matrix2[7 ]=m2[1][3];
	matrix2[8 ]=m2[2][0];	matrix2[9 ]=m2[2][1];	matrix2[10]=m2[2][2];	matrix2[11]=m2[2][3];
	matrix2[12]=m2[3][0];	matrix2[13]=m2[3][1];	matrix2[14]=m2[3][2];	matrix2[15]=m2[3][3];
		
	float matrix3[16];
	
	int ok = m4_inverse( matrix3, matrix2 );
	if(!ok) {
		this->cast=false;
		return false; 	// nao encontrou
	}
	
	matrix[0][0]=matrix3[ 0];	matrix[0][1]=matrix3[ 1];	matrix[0][2]=matrix3[ 2];	matrix[0][3]=matrix3[ 3];
	matrix[1][0]=matrix3[ 4];	matrix[1][1]=matrix3[ 5];	matrix[1][2]=matrix3[ 6];	matrix[1][3]=matrix3[ 7];
	matrix[2][0]=matrix3[ 8];	matrix[2][1]=matrix3[ 9];	matrix[2][2]=matrix3[10];	matrix[2][3]=matrix3[11];
	matrix[3][0]=matrix3[12];	matrix[3][1]=matrix3[13];	matrix[3][2]=matrix3[14];	matrix[3][3]=matrix3[15];
	
	multipleMatrix(matrix,d);
	multipleMatrix(matrix,o);
	
	//glPushMatrix();
	//glLineWidth(3);
	//glColor3f(0.0f,1.0f,0.0f);
	//glBegin(GL_LINES);
	//glVertex3f(o[0],o[1],o[2]);
	//glVertex3f((10*d[0])+o[0],(10*d[1])+o[1],(10*d[2])+o[2]);
	//glEnd();
	//glPopMatrix();
			
	float delta, raiz, t;
  
	Vector u = Vector(d[0],d[1],d[2]);				// valor de u da equacao
	u.normalize();
	Vector v = Vector(-o[0],-o[1],-o[2]);	// valor de v da equacao
	//Vector v = Vector(center - R.P);

	float  tmp = (v - (u*v)*u).length();
	delta = (this->height/2)*(this->height/2) - (tmp*tmp);
  
	if (delta < 0) { // para nao calcular raiz negativa
		this->cast=false;
		return false; 	// nao encontrou
	}
  
	raiz = sqrt(delta); // raiz da equacao
  
	if ((t = (u*v) - raiz) > SMALL) { // caso o primeiro ja seja maior que 0
		this->cast=true;
		return true;
	} else if ((t = (u*v) + raiz) > SMALL) { // senao t é o segundo
		this->cast=true;
		return true;
	}

	this->cast=false;
	return false; // nao encontro


	
}

Cylinder *Cylinder::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Cylinder *cylinder;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Cylinder *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	cylinder = new Cylinder();
	cylinder->read(node);
	
	return cylinder;
	
}

void *Cylinder::read(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode (attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "radius")) {
			this->radius = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "height")) {
			this->height = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"bottom")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				this->bottom=true; 
			} else {
				this->bottom=false;
			}
		} else
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"side")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				this->side=true; 
			} else {
				this->side=false;
			}
		} else
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"top")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				this->top=true;
			} else {
				this->top=false;
			}
		} else 
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Cylinder memeber not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}

	}
	
	return(NULL);
	
}

void Cylinder::follow() {}

void Cylinder::draw() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Cylinder)" << endl;	// Message about debuging
	#endif
	
	double radius1;
	double radius2;
	
	float f=0;
	
	unsigned short int i;
	
	if(this->side) {
		
		glBegin(GL_TRIANGLE_STRIP);
			
		for(i=0; i<=QUALITY; i++) {
					
			f+=2*PI/QUALITY;
		
			radius1=radius*cos(f);
			radius2=radius*sin(f);

			glNormal3d( (radius1/radius) ,0, (radius2/radius));
			glTexCoord2d(f/(2*PI),0.0);
			glVertex3d(radius1, -(this->height/2), radius2);

			glTexCoord2d(f/(2*PI),1.0);
			glVertex3d(radius1,  (this->height/2), radius2);
		
		}
		
		glEnd();
		
	}

	if(this->bottom) {
		glPushMatrix();
		glTranslated(0,-(this->height/2),0);
		glRotated(90,1,0,0);
		glutSolidCone(radius,0,QUALITY,1);
		glPopMatrix();
	}
	
	if(this->top) {
		glPushMatrix();
		glTranslated(0,(this->height/2),0);
		glRotated(-90,1,0,0);
		glutSolidCone(radius,0,QUALITY,1);
		glPopMatrix();
	}
	
	if( (this->cast) && (Render::my_rrr->wandRay) ) {
		glDisable(GL_LIGHTING);
		glColor3d(1,0,0);
		glutWireCube(height);
		glEnable(GL_LIGHTING);
	}

	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Cylinder)" << endl;	// Message about debuging
	#endif
	
}

ElevationGrid::ElevationGrid() {

	this->solid=true;
	this->xDimension=0;
	this->xSpacing=1.0;
	this->zDimension=0;
	this->zSpacing=1.0;
	this->colorPerVertex=true;
	this->creaseAngle=0;
	
	
	this->firstPass=true;
	this->values=NULL;
	
}

bool ElevationGrid::ray(float o2[4], float d2[4], float m2[4][4]) {
	return false;
}


ElevationGrid *ElevationGrid::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	ElevationGrid *elevationGrid;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((ElevationGrid *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	elevationGrid = new ElevationGrid();
	elevationGrid->read(node);
	
	return elevationGrid;
	
}


void *ElevationGrid::read(DOMNode *node) {	

	int f,g,h,y; // variables to counter
	
	unsigned int i;  // variable to counter

	DOMNamedNodeMap *attributes;
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode (attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "xDimension")) {
			xDimension = atoi(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "zDimension")) { 
			zDimension = atoi(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "xSpacing")) {
			xSpacing = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "zSpacing")) {
			zSpacing = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()),"solid")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				solid=true; 
			} else {
				solid=false;
			}
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "colorPerVertex")) {
		if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				colorPerVertex=1; 
			} else {
				colorPerVertex=1;
			}
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "creaseAngle")) {
			creaseAngle = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "height") || 
					!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "set_height")) {
			// this is done after
		} else 
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Elevation Grid member not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}

	}
	
	coord = new GLdouble[xDimension*zDimension*3];
	normal = new GLdouble[xDimension*zDimension*3];
	texture = new GLdouble[xDimension*zDimension*2];
	if( (coord==NULL) || (normal==NULL) || (texture==NULL) ) cerr << "Memory alocation problem at elevation grid" << endl;
	
	
	#ifndef XERCES  // actualy is the xerces parser 			
		char *ptrptr;
		char *ctemp;
	#endif
	
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "height")) {
			
			
			#ifdef XERCES  // actualy is the xerces parser 
				
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				for(f=0;f<xDimension*zDimension;f++) {
					
					if(tok->hasMoreTokens()) {
						
						coord[(f*3)+1]=atof(XMLString::transcode(tok->nextToken()));
						#ifdef DEBUG
							cout << "VALOR COORD : " << coord[(f*3)+1] << endl;
						#endif
						
					} else {
						cout << "the elevation grid presents a dimension bigger than the number of points informed" << endl;
					}
					
				}	
			
				delete tok;
							
			#else
			
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				coord[1] = atof(strtok_r(x3d::message,", \n,",&ptrptr));
				for(f=1;f<xDimension*zDimension;f++) {
					ctemp = strtok_r(NULL,", \n,",&ptrptr);
					if(ctemp!=NULL) {
						coord[(f*3)+1]=atof(ctemp); // the height id direct armanazed in the coord
						#ifdef DEBUG
							cout << "VALOR COORD : " << coord[(f*3)+1] << endl;
						#endif
					} else {
						cout << "the elevation grid presents a dimension bigger than the number of points informed" << endl;
					}
				}
			
			#endif
		}
	}
	
	for(f=0; f<zDimension ;f++) {
		for(g=0; g<xDimension ;g++) {
			coord[(3*(g+(f*xDimension)))]=g*xSpacing;
			coord[(3*(g+(f*xDimension)))+2]=f*zSpacing;
			
			texture[(2*(g+(f*xDimension)))]=((double)g)/(xDimension-1);
			texture[(2*(g+(f*xDimension)))+1]=((double)f)/(zDimension-1);
		}
	}
	
	#ifdef DEBUG
		for(f=0; f<sizeZ*xDimension*3; f++)
			cout << "in " << f << " value " << coord[f] << endl;
	#endif
		
	// This creates the stripe zig-zag		
	for(y=0,h=0; y<zDimension-1 ; y++) {
		for(f=0,g=xDimension ; f<xDimension ;h+=2) {
			coordIndex[h]=(y*xDimension)+(f++);
			#ifdef DEBUG
				cout << "index " << h << " value " << coordIndex[h] << endl;
			#endif
			
			coordIndex[h+1]=(y*xDimension)+(g++);
			#ifdef DEBUG
				cout << "index " << h+1 << " value " << coordIndex[h+1] << endl;
			#endif
		}
	}
	
	// Calculando as normais de forma errada 
	
	// deve estar instavel
	
	double tempn[3];
	
	double temp1[3];
	double temp2[3];
	double temp3[3];
	
	double vec1[3];
	double vec2[3];
	
	for(g=0; g < (2*(xDimension-1)*zDimension);g = g + (2*xDimension) ) {
		
		temp1[0]=coord[ 3*coordIndex[g]];
		temp1[1]=coord[(3*coordIndex[g])+1];
		temp1[2]=coord[(3*coordIndex[g])+2];
		
		temp2[0]=coord[ 3*coordIndex[g+1]];
		temp2[1]=coord[(3*coordIndex[g+1])+1];
		temp2[2]=coord[(3*coordIndex[g+1])+2];
		
		temp3[0]=coord[ 3*coordIndex[g+2]];
		temp3[1]=coord[(3*coordIndex[g+2])+1];
		temp3[2]=coord[(3*coordIndex[g+2])+2];
		
		vec1[0]=temp2[0]-temp1[0];
		vec1[1]=temp2[1]-temp1[1];
		vec1[2]=temp2[2]-temp1[2];
		
		vec2[0]=temp3[0]-temp2[0];
		vec2[1]=temp3[1]-temp2[1];
		vec2[2]=temp3[2]-temp2[2];
		
		crossprod(vec1,vec2,tempn);
		normalize(tempn);

		normal[ 3*coordIndex[g]] = tempn[0];
		normal[(3*coordIndex[g])+1] = tempn[1];
		normal[(3*coordIndex[g])+2] = tempn[2];
		
		for(f=0;f<(2*xDimension)-2;f++) {
			
			temp1[0]=coord[ 3*coordIndex[g+f]];
			temp1[1]=coord[(3*coordIndex[g+f])+1];
			temp1[2]=coord[(3*coordIndex[g+f])+2];
		
			temp2[0]=coord[ 3*coordIndex[g+f+1]];
			temp2[1]=coord[(3*coordIndex[g+f+1])+1];
			temp2[2]=coord[(3*coordIndex[g+f+1])+2];
		
			temp3[0]=coord[ 3*coordIndex[g+f+2]];
			temp3[1]=coord[(3*coordIndex[g+f+2])+1];
			temp3[2]=coord[(3*coordIndex[g+f+2])+2];
		
			vec1[0]=temp2[0]-temp1[0];
			vec1[1]=temp2[1]-temp1[1];
			vec1[2]=temp2[2]-temp1[2];
		
			vec2[0]=temp3[0]-temp2[0];
			vec2[1]=temp3[1]-temp2[1];
			vec2[2]=temp3[2]-temp2[2];
		
			crossprod(vec1,vec2,tempn);
			normalize(tempn);

			normal[ 3*coordIndex[g+f+1]] = tempn[0];
			normal[(3*coordIndex[g+f+1])+1] = tempn[1];
			normal[(3*coordIndex[g+f+1])+2] = tempn[2];
			
		}
		
		normal[ 3*coordIndex[g+f+2]] = tempn[0];
		normal[(3*coordIndex[g+f+2])+1] = tempn[1];
		normal[(3*coordIndex[g+f+2])+2] = tempn[2];
		

	}
		
	return(NULL);
	
}

void ElevationGrid::follow() {}

// Draws a Elevetaion Grid object
void ElevationGrid::draw() {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (ElevationGrid)" << endl;	// Message about debuging
	#endif

	unsigned short int g;	// variables to counter
	
	if(this->solid) { } //define backface culling

	// This routine is not good enouth, but moves the data to a memory in stripe
	if(firstPass) {
	
		this->firstPass=false;
		
		this->values = new float[(8*xDimension*zDimension)];
			
		for(g=0;g<(xDimension*zDimension);g=g++) {
		
			this->values[(g*8)+5]=(float)this->coord[(g*3)+0];
			this->values[(g*8)+6]=(float)this->coord[(g*3)+1];
			this->values[(g*8)+7]=(float)this->coord[(g*3)+2];
			
			this->values[(g*8)+2]=(float)this->normal[(g*3)+0];
			this->values[(g*8)+3]=(float)this->normal[(g*3)+1];
			this->values[(g*8)+4]=(float)this->normal[(g*3)+2];
		
			this->values[(g*8)+0]=(float)this->texture[(g*2)+0];
			this->values[(g*8)+1]=(float)this->texture[(g*2)+1];

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
		glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, 8*xDimension*zDimension*sizeof(float), this->values, GL_STATIC_DRAW_ARB);
			
	} else {
	
		#ifdef DEBUG3
		cout << "Binding Buffer" << endl;
		#endif
		glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->m_nVBOs);
		
		#ifdef DEBUG3
		cout << "Selecting format" << endl;
		#endif
		glInterleavedArrays(GL_T2F_N3F_V3F,0, (char *) NULL );
		
		for(g=0; g < (2*(xDimension-1)*zDimension);g = g + (2*xDimension) ) {
			glDrawElements(GL_QUAD_STRIP, (2*xDimension), GL_UNSIGNED_INT, &coordIndex[g]);
		}
		
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (ElevationGrid)" << endl;	// Message about debuging
	#endif
	
}

ElevationGrid::~ElevationGrid() {
	
	delete[] this->coord;		//temp
	delete[] this->texture;		//temp

}



Extrusion::Extrusion() {

	this->convex=true;
	this->solid=true;
	
	this->csPoints    = 0;	// reseting the counter for CrossSection points
	this->scalePoints = 0;	// reseting the counter for scale points
	this->spinePoints = 0;	// reseting the counter for spine points
	this->defScale    = false;	// define if it is going to scale or not (if read)
	
	this->firstPass=true;
	
}


bool Extrusion::ray(float o2[4], float d2[4], float m2[4][4]) {
	return false;
}

Extrusion *Extrusion::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	Extrusion *extrusion;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Extrusion *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	extrusion = new Extrusion();
	extrusion->read(node);
	
	return extrusion;
	
}

void *Extrusion::read(DOMNode *node) {
	
	
	#ifdef DEBUG1
		cout << "Reading (Extrusion)" << endl;	// Message about debuging
	#endif

	
	int f,g,h,y; 					// variables to counter
	
	unsigned int i; 				// variable to counter
	char *ctemp;					// temporary variable
	DOMNamedNodeMap *attributes;	//

	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {	
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode (attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "crossSection") ||
			 !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_crossSection")) {
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ,\n");
			while (ctemp != NULL) {
				crossSection[0][csPoints] = atof(ctemp);
				ctemp = strtok(NULL," ,\n");
				crossSection[1][csPoints++] = atof(ctemp);
				
				#ifdef DEBUG2
					cout << "Points : " << crossSection[0][csPoints-1] << " , " << crossSection[1][csPoints-1] << endl;
				#endif
				
				ctemp = strtok(NULL," ,");
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "orientation") ||
			 !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "set_orientation")) {
			// IMPLEMENTAR
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "scale") ||
			 !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_scale")) {
			defScale=true;
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ,\n");
			while (ctemp != NULL) {
				scale[0][scalePoints] = atof(ctemp);
				ctemp = strtok(NULL," ,\n");
				scale[1][scalePoints++] = atof(ctemp);
				
				#ifdef DEBUG2
					cout << "Points : " << scale[0][scalePoints-1] << endl;
				#endif
				
				ctemp = strtok(NULL," ");
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "spine") ||
			 !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "set_spine") ) {
			ctemp = strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ,\n");
			while (ctemp != NULL) {
				spine[0][spinePoints] = atof(ctemp);
				ctemp = strtok(NULL," ,\n");
				spine[1][spinePoints] = atof(ctemp);
				ctemp = strtok(NULL," ,\n");
				spine[2][spinePoints++] = atof(ctemp);
				ctemp = strtok(NULL," ,\n");
				
				#ifdef DEBUG2
					cout << "Spine " << spinePoints-1 << " : " << spine[0][spinePoints-1] << " , " << spine[1][spinePoints-1] << " , " << spine[2][spinePoints-1] << endl;
				#endif
				
			}
		} else 
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "beginCap")) {
			// IMPLEMENTAR
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "ccw")) {
			// IMPLEMENTAR
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "convex")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				convex=true; 
			} else {
				convex=false;
			}
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "creaseAngle")) {
			// IMPLEMENTAR
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "endCap")) {
			// IMPLEMENTAR
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "solid")) {
			if (!strcasecmp(XMLString::transcode (attributes->item(i)->getNodeValue()),"TRUE")) {
				solid=1; 
			} else {
				solid=0;
			}	
		} else
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Extrusion member not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}
	}
	
	
	#ifdef DEBUG2
		cout << "Creating Dataset" << endl;	// Message about debuging
	#endif

	
	coord = new GLdouble[csPoints*spinePoints*3];
	texture = new GLdouble[csPoints*spinePoints*2];
	coordIndex = new unsigned int [((2*csPoints)*(spinePoints-1))];
	if( (coord==NULL) || (texture==NULL) || (coordIndex==NULL) ) cerr << "Memory alocation problem at extrusion" << endl;

		for(f=0;f<spinePoints;f++) {

		for(g=0;g<csPoints;g++) {
		
			coord[(3*(g+(f*csPoints)))+1]=spine[1][f];
			
			if(defScale) {
				coord[(3*(g+(f*csPoints)))]=crossSection[0][g]*scale[0][f];
				coord[(3*(g+(f*csPoints)))+2]=crossSection[1][g]*scale[1][f];
			} else {
				coord[(3*(g+(f*csPoints)))]=crossSection[0][g];
				coord[(3*(g+(f*csPoints)))+2]=crossSection[1][g];
			}
				
			#ifdef DEBUG
				cout << "Point " << (3*(g+(f*csPoints))) << " :" << crossSection[0][g] 
							 << ",  " << (3*(g+(f*csPoints)))+1 << " :" << spine[1][f] 
							 << ",  " << (3*(g+(f*csPoints)))+2 << " :" << crossSection[1][g] << endl;
			#endif
			
			texture[(2*(g+(f*csPoints)))]=(((double)g)/(csPoints-1));
			texture[(2*(g+(f*csPoints)))+1]=(((double)f)/(spinePoints-1));
			
		}
	}
	
	#ifdef DEBUG2
		cout << endl << endl << "Draw points index " << endl;
		cout << "SpinePints " << spinePoints << endl;
		cout << "csPoints " << csPoints << endl << endl;
	#endif
	
	for(y=0, h=0; y<spinePoints-1 ; y++) {
		
		for(f=0, g=csPoints ; f<csPoints ;h+=2) {
			
			coordIndex[h]=(y*csPoints)+(f++);
			coordIndex[h+1]=(y*csPoints)+(g++);
			
			#ifdef DEBUG
				cout << "Index : " << h << " = " << coordIndex[h] << endl;
				cout << "Index : " << h+1 << " = " << coordIndex[h+1] << endl;
			#endif
			
		}
	}
	
	return(NULL);
	
}

void Extrusion::follow() {}

void Extrusion::draw() {

	#ifdef DEBUG1
		cout << "\t\t\t\t -Drawing- Following (Extrusion)" << endl;	// Message about debuging
	#endif
	
	unsigned short int g;
	
	#ifdef DEBUG1
		cout << "\t\t\t\t Drawing Elements of Extrusion" << endl;	// Message about debuging
	#endif
	
	
	if(firstPass) {
	
		this->firstPass=false;
		
		this->values = new float[(5*csPoints*spinePoints)];
	
		for(g=0;g<(csPoints*spinePoints);g=g++) {
		
			this->values[(g*5)+0]=coord[(g*3)+0];
			this->values[(g*5)+1]=coord[(g*3)+1];
			this->values[(g*5)+2]=coord[(g*3)+2];
		
			this->values[(g*5)+3]=texture[(g*2)+0];
			this->values[(g*5)+4]=texture[(g*2)+1];
			
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
		glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, csPoints*spinePoints*5*sizeof(float), this->values, GL_STATIC_DRAW_ARB);
			
	} else {
	
		#ifdef DEBUG3
		cout << "Binding Buffer" << endl;
		#endif
		glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->m_nVBOs);
		
		#ifdef DEBUG3
		cout << "Selecting format" << endl;
		#endif
		glInterleavedArrays(GL_T2F_V3F,0, (char *) NULL );
		
		for(g=0; g < (2*(csPoints)*(spinePoints-1));g = g + (2*csPoints) ) {
			glDrawElements(GL_QUAD_STRIP, (2*csPoints), GL_UNSIGNED_INT, &coordIndex[g]);
		}
		
	}
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Extrusion)" << endl;	// Message about debuging
	#endif
	
}

Extrusion::~Extrusion() {

	delete[] coord;
	delete[] texture;
	delete[] coordIndex;

}

IndexedFaceSet::IndexedFaceSet() {
	
	this->counterCoord=0;			// counter to fill the variable coord
	this->counterTextureCoord=0;	// counter to fill the variable texture
	this->counterNormalCoord=0;		// counter to fill the variable normal
	this->counterColorCoord=0;		// counter to fill the variable color
	
	this->dynamic=false;
	
	this->textureUse=false;
	this->normalUse=false;
	this->colorUse=false;
	
	this->solid=true;
		
	this->ccw=1;
	this->convex=1;

	coordIndex = NULL;
	texCoordIndex = NULL;
	normalIndex = NULL;
	colorIndex = NULL;
		
	coordinate=NULL;
	textureCoordinate=NULL;
	normal=NULL;
	color=NULL;
	
	this->cast=false;				// to ray tracing
	
	this->firstPass=true;
	
}


IndexedFaceSet::~IndexedFaceSet() {
	
	delete[] coordIndex;	//temp
	delete[] texCoordIndex;	//temp
	delete[] normalIndex;	//temp
	delete[] colorIndex;	//temp

	
	if(color!=NULL) {
		delete color;
	}
	
	if(coordinate!=NULL) {
		delete coordinate;
	}
	
	if(textureCoordinate!=NULL) {
		delete textureCoordinate;
	}
	
	if(normal!=NULL) {
		delete normal;
	}

}

bool IndexedFaceSet::ray(float o2[4], float d2[4], float m2[4][4]) {

	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (IndexedFaceSet)" << endl;	// Message about debuging
	#endif

	return false;
}

void IndexedFaceSet::follow() {}

void IndexedFaceSet::draw() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (IndexedFaceSet)" << endl;	// Message about debuging
	#endif

	#define CKV_MAX 128
			
	GLdouble kv[CKV_MAX][11];

	unsigned int g;						// counter 

	//if(dynamic && this->counterCoord) {
	if(firstPass) {

		if(coordinate!=NULL) {
			coordinate->follow();
		}

		if(textureCoordinate!=NULL) {
			textureCoordinate->follow();
		}

		if(normal!=NULL) {
			normal->follow();
		}

		if(color!=NULL) {
			color->follow();
		}

		
			unsigned int h;
			int j;
			unsigned int k;
		
			#ifdef DEBUG1
				cout << "counterCoord"         << counterCoord << endl;
				cout << "counterTextureCoord" << counterTextureCoord << endl;
				cout << "counterNormalCoord"  << counterNormalCoord << endl;
			#endif
					
			unsigned int ckv;
			
			g=0; //reseting g that is the couter that follows the couter of coordIndex (couterCoord)
						
			// seting the fisrt vector to know the maximun and minimun values for all poligon
			maxMin[0][0]=maxMin[0][1]=(float)this->coordinate->value->get((3*coordIndex[0])+0);
			maxMin[1][0]=maxMin[1][1]=(float)this->coordinate->value->get((3*coordIndex[0])+1);
			maxMin[2][0]=maxMin[2][1]=(float)this->coordinate->value->get((3*coordIndex[0])+2);

			if(colorUse && textureUse) {
				x3d::my_x3d->tesselator->format = GL_T2F_C4F_N3F_V3F;
			} else if(colorUse) {
				x3d::my_x3d->tesselator->format = GL_C4F_N3F_V3F;
			} else if(textureUse){
				x3d::my_x3d->tesselator->format = GL_T2F_N3F_V3F;
			} else {
				x3d::my_x3d->tesselator->format = GL_N3F_V3F;
			}
		
			int each_face=0;
			
			x3d::my_x3d->tesselator->start();
			
			//ckv=0;		//reseting ckv
			//gluTessBeginPolygon(x3d::my_x3d->tesselator->tess, x3d::my_x3d->tesselator);		// delimit a polygon description
			
			while(g<this->counterCoord) { 			// do while couter g smaller than couter of coordIndex
				
				h=g;								// the beggining of the couter based in the last position of the g
				j=0;								// reseting the counter of the number of vertices of this polygon (usualy a triangle)
				
				each_face++;	// the proble is that it starts with 1 instead of 0
				
				while(this->coordIndex[h]!=-1) {  	// search until the end of the polygon
					
					h++;							// more one vertice
					j++;							// more one vertice
					
				}
				
				gluTessBeginPolygon(x3d::my_x3d->tesselator->tess, x3d::my_x3d->tesselator);		// delimit a polygon description
				gluTessBeginContour(x3d::my_x3d->tesselator->tess);			// delimit a contour description
				
				
				#ifdef DEBUG3
					cout << endl << endl << "Starting Poligon" << endl;
				#endif
				
				ckv=0;		//reseting ckv
				
				for(k=g;k<h;k++) {					// k is the counter that follows g that is the counter of counterCoord, it is going until the end of the polygon
				
					#ifdef DEBUG4
						cout << endl << "Counter k : " << k << " , ckv : " << ckv << endl;
						cout << "Vertices" << endl;
						cout << "Coord 1 ( " << (3*coordIndex[k])   <<" ) : " << this->coordinate->value->get((3*coordIndex[k])  ) << endl;
						cout << "Coord 2 ( " << (3*coordIndex[k])+1 <<" ) : " << this->coordinate->value->get((3*coordIndex[k])+1) << endl;
						cout << "Coord 2 ( " << (3*coordIndex[k])+2 <<" ) : " << this->coordinate->value->get((3*coordIndex[k])+2) << endl;
					#endif
					
					kv[ckv][0]=(double)this->coordinate->value->get((3*coordIndex[k])  );
					kv[ckv][1]=(double)this->coordinate->value->get((3*coordIndex[k])+1);
					kv[ckv][2]=(double)this->coordinate->value->get((3*coordIndex[k])+2);
					
					if(kv[ckv][0]>maxMin[0][0]) { maxMin[0][0] = kv[ckv][0]; }
					if(kv[ckv][0]<maxMin[0][1]) { maxMin[0][1] = kv[ckv][0]; }
					if(kv[ckv][1]>maxMin[1][0]) { maxMin[1][0] = kv[ckv][1]; }
					if(kv[ckv][1]<maxMin[1][1]) { maxMin[1][1] = kv[ckv][1]; }
					if(kv[ckv][2]>maxMin[2][0]) { maxMin[2][0] = kv[ckv][2]; }
					if(kv[ckv][2]<maxMin[2][1]) { maxMin[2][1] = kv[ckv][2]; }
					
					if( (textureUse) && (k<counterTextureCoord) ) {
						#ifdef DEBUG4
							cout << "TexCoord 1 ( " << (2*texCoordIndex[k])   <<" ) : " << this->textureCoordinate->value->get((2*texCoordIndex[k])  ) << endl;
							cout << "TexCoord 2 ( " << (2*texCoordIndex[k])+1 <<" ) : " << this->textureCoordinate->value->get((2*texCoordIndex[k])+1) << endl;
						#endif
						if(k>=counterTextureCoord) {
							//#ifdef DEBUG1
								cerr << "Error k bigger than counterTextureCoord" << endl; 
							//#endif
						} else {
							kv[ckv][3]=(double)this->textureCoordinate->value->get((2*texCoordIndex[k])  );
							kv[ckv][4]=(double)this->textureCoordinate->value->get((2*texCoordIndex[k])+1);
						}
					} else {
						kv[ckv][3]=0.0f;
						kv[ckv][4]=0.0f;
					}
					
					if( (normalUse) && (k<counterNormalCoord) ) {
						
						#ifdef DEBUG4
							cout << "normalCoord 1 ( " << 3*normalIndex[k]   <<" ) : " << this->normal->value->get((3*normalIndex[k])  ) << endl;
							cout << "normalCoord 2 ( " << 3*normalIndex[k]+1 <<" ) : " << this->normal->value->get((3*normalIndex[k])+1) << endl;
							cout << "normalCoord 3 ( " << 3*normalIndex[k]+2 <<" ) : " << this->normal->value->get((3*normalIndex[k])+2) << endl;
						#endif
					
						if(k>=counterNormalCoord) {
							//#ifdef DEBUG1
								cerr << "Error k bigger than counterNormalCoord" << endl;
							//#endif
						} else {
							kv[ckv][5]=(double)this->normal->value->get((3*normalIndex[k])  );
							kv[ckv][6]=(double)this->normal->value->get((3*normalIndex[k])+1);
							kv[ckv][7]=(double)this->normal->value->get((3*normalIndex[k])+2);
						}
					} else {
						
						#ifdef DEBUG4
							cout << "No normals defined" << endl;
						#endif
						
						double v1[3],v2[3],v3[3];
						
						v1[0]=(double)this->coordinate->value->get((3*coordIndex[g])  );
						v1[1]=(double)this->coordinate->value->get((3*coordIndex[g])+1);
						v1[2]=(double)this->coordinate->value->get((3*coordIndex[g])+2);
						
						v2[0]=(double)this->coordinate->value->get((3*coordIndex[g+1])  );
						v2[1]=(double)this->coordinate->value->get((3*coordIndex[g+1])+1);
						v2[2]=(double)this->coordinate->value->get((3*coordIndex[g+1])+2);
						
						v3[0]=(double)this->coordinate->value->get((3*coordIndex[g+2])  );
						v3[1]=(double)this->coordinate->value->get((3*coordIndex[g+2])+1);
						v3[2]=(double)this->coordinate->value->get((3*coordIndex[g+2])+2);
						
						
						double v1_v2[3]; 	// vector v1 - v2
						double v2_v3[3];	// vector v2 - v3
						
						v1_v2[0] = v1[0] - v2[0]; 
						v1_v2[1] = v1[1] - v2[1]; 
						v1_v2[2] = v1[2] - v2[2]; 
						
						
						v2_v3[0] = v2[0] - v3[0]; 
						v2_v3[1] = v2[1] - v3[1]; 
						v2_v3[2] = v2[2] - v3[2]; 
						
						double resp[3];
						
						crossprod(v1_v2,v2_v3,resp);   // executing cross product to get normal
						
						kv[ckv][5]=resp[0];
						kv[ckv][6]=resp[1];
						kv[ckv][7]=resp[2];
						
					}
					gluTessNormal(x3d::my_x3d->tesselator->tess,kv[ckv][5],kv[ckv][6],kv[ckv][7]);
				
					if( (colorUse) ) {
						#ifdef DEBUG4
							cout << "colorCoord 1 ( " << 3*colorIndex[k]   << " ) : " << this->color->value->get((3*colorIndex[k])  ) << endl;
							cout << "colorCoord 2 ( " << 3*colorIndex[k]+1 << " ) : " << this->color->value->get((3*colorIndex[k])+1) << endl;
							cout << "colorCoord 3 ( " << 3*colorIndex[k]+2 << " ) : " << this->color->value->get((3*colorIndex[k])+2) << endl;
						#endif
						if(colorPerVertex) {
							#ifdef DEBUG3
								cout << "Color per Vertex is true" << endl;
							#endif
							if(k<counterColorCoord) {
								kv[ckv][8] =this->color->value->get((3*(colorIndex[k]))  );
								kv[ckv][9] =this->color->value->get((3*(colorIndex[k]))+1);
								kv[ckv][10]=this->color->value->get((3*(colorIndex[k]))+2);
							} else {
								// Problem
							}
							
						} else {
							#ifdef DEBUG3
								cout << "Color per Vertex is false" << endl;
							#endif
							kv[ckv][8] =this->color->value->get((3*(colorIndex[each_face-1]))  );
							kv[ckv][9] =this->color->value->get((3*(colorIndex[each_face-1]))+1);
							kv[ckv][10]=this->color->value->get((3*(colorIndex[each_face-1]))+2);
						}
					} else {
						
						kv[ckv][8] = 1.0f;
						kv[ckv][9] = 1.0f;
						kv[ckv][10]= 1.0f;
						
					}
					
					#ifdef DEBUG4
						cout << "Value sent : " << endl;
						cout << " X  " << kv[ckv][0];
						cout << " Y  " << kv[ckv][1];
						cout << " Z  " << kv[ckv][2];
						cout << endl;
					#endif
					
					gluTessVertex(x3d::my_x3d->tesselator->tess, kv[ckv], kv[ckv]);
					
					ckv++; // pointing to new possition for data pointer
					
					if(ckv>CKV_MAX) { // case the polygon is bigger than alloced memory
						cerr << "Maximum munber of tesselations" << endl;
					}
					
				}
			
				gluTessEndContour(x3d::my_x3d->tesselator->tess);
		
				gluTessEndPolygon(x3d::my_x3d->tesselator->tess);
				
				g=h+1;
		
			}
		
			//gluTessEndPolygon(x3d::my_x3d->tesselator->tess);

			
			//x3d::my_x3d->tesselator->draw();
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
		
			if(colorUse && textureUse) {
				glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, x3d::my_x3d->tesselator->counter*12*sizeof(float), x3d::my_x3d->tesselator->values, GL_STATIC_DRAW_ARB);
			} else if(colorUse) {
				glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, x3d::my_x3d->tesselator->counter*10*sizeof(float), x3d::my_x3d->tesselator->values, GL_STATIC_DRAW_ARB);
			} else if(textureUse){
				glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, x3d::my_x3d->tesselator->counter*8*sizeof(float), x3d::my_x3d->tesselator->values, GL_STATIC_DRAW_ARB);
			} else {
				glBufferDataARB_ptr(GL_ARRAY_BUFFER_ARB, x3d::my_x3d->tesselator->counter*6*sizeof(float), x3d::my_x3d->tesselator->values, GL_STATIC_DRAW_ARB);
			}

	
			this->counterIndices = x3d::my_x3d->tesselator->counterIndices;

			this->type         = new GLenum[this->counterIndices];
			this->indicesSize  = new unsigned int[this->counterIndices];
			

			this->start		   = new GLuint[this->counterIndices];
			this->end		   = new GLuint[this->counterIndices];

			this->indices      = new GLuint*[this->counterIndices];
			//this->indicesID	   = new unsigned int[this->counterIndices];

			

			for(unsigned int loopCounterIndices=0;loopCounterIndices < this->counterIndices;loopCounterIndices++) {
				
				this->type[loopCounterIndices]        = x3d::my_x3d->tesselator->type[loopCounterIndices];
				this->indicesSize[loopCounterIndices] = x3d::my_x3d->tesselator->indicesSize[loopCounterIndices];
				
				this->indices[loopCounterIndices] = new GLuint[this->indicesSize[loopCounterIndices]];
			
				/*
				glGenBuffersARB_ptr( 1, &this->indicesID[loopCounterIndices] );
				
				glBindBufferARB_ptr( GL_ELEMENT_ARRAY_BUFFER_ARB,
									 this->indicesID[loopCounterIndices]);				
				
				glBufferDataARB_ptr(GL_ELEMENT_ARRAY_BUFFER_ARB, 
									x3d::my_x3d->tesselator->indicesSize[loopCounterIndices]*sizeof(unsigned int),
									x3d::my_x3d->tesselator->indices[loopCounterIndices],
									GL_STATIC_DRAW_ARB);
							
				this->start[loopCounterIndices] = this->end[loopCounterIndices] =
								x3d::my_x3d->tesselator->indices[loopCounterIndices][0];
				*/
				
				for( unsigned int loopSize=0; loopSize < this->indicesSize[loopCounterIndices] ; loopSize++ ) {
					
					this->indices[loopCounterIndices][loopSize] = x3d::my_x3d->tesselator->indices[loopCounterIndices][loopSize];
					
					if( x3d::my_x3d->tesselator->indices[loopCounterIndices][loopSize] < this->start[loopCounterIndices] ) {
						this->start[loopCounterIndices] = x3d::my_x3d->tesselator->indices[loopCounterIndices][loopSize];
					}
					
					if( x3d::my_x3d->tesselator->indices[loopCounterIndices][loopSize] > this->end[loopCounterIndices] ) {
						this->end[loopCounterIndices] = x3d::my_x3d->tesselator->indices[loopCounterIndices][loopSize];
					}
					
				}
						
			}

			this->firstPass=false;
			
			#ifdef DEBUG3
			cout << "Finishing VBO" << endl;
			#endif
		
		
	} else {
	
		#ifdef DEBUG3
		cout << "Binding Buffer" << endl;
		#endif
		glBindBufferARB_ptr(GL_ARRAY_BUFFER_ARB, this->m_nVBOs);
		
		#ifdef DEBUG3
		cout << "Selecting format" << endl;
		#endif
		
		if(colorUse && textureUse) {
			glEnable(GL_COLOR_MATERIAL);
			glInterleavedArrays(GL_T2F_C4F_N3F_V3F,0, (char *) NULL );
		} else if(colorUse) {
			glEnable(GL_COLOR_MATERIAL);
			glInterleavedArrays(GL_C4F_N3F_V3F,0, (char *) NULL );
		} else if(textureUse) {
			glInterleavedArrays(GL_T2F_N3F_V3F,0, (char *) NULL );
		} else {
			glInterleavedArrays(GL_N3F_V3F,0, (char *) NULL );
		}
		
		for(unsigned int loopCounterIndices=0;loopCounterIndices < this->counterIndices;loopCounterIndices++) {

			glDrawRangeElements( this->type[loopCounterIndices], 
								 this->start[loopCounterIndices], 
								 this->end[loopCounterIndices], 
						    	 this->indicesSize[loopCounterIndices],
								 GL_UNSIGNED_INT,
								 this->indices[loopCounterIndices] );
		
		/*		
			glBindBufferARB_ptr( GL_ELEMENT_ARRAY_BUFFER_ARB, this->indicesID[loopCounterIndices]);
			glDrawRangeElements( this->type[loopCounterIndices], 
								 this->start[loopCounterIndices], 
								 this->end[loopCounterIndices], 
						    	 this->indicesSize[loopCounterIndices],
								 GL_UNSIGNED_INT,
								 (char *) NULL );
		*/	
			
		}
		
		
		
		//glCallList(list);
		
	}
	
	if( (cast) && (Render::my_rrr->wandRay) ) {

		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);

		glBegin(GL_LINE_LOOP);
		// Front Face
		glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][0]);	glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][0]);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		// Back Face
		glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][1]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][1]);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		// Right Face
		glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][1]);	glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][0]);	glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][0]);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		// Left Face
		glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][1]);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		// Top Face
		glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][0]);	glVertex3f(maxMin[0][0],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][1]);	glVertex3f(maxMin[0][1],maxMin[1][0],maxMin[2][0]);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		// bottom Face
		glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][1]);	glVertex3f(maxMin[0][0],maxMin[1][1],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][0]);	glVertex3f(maxMin[0][1],maxMin[1][1],maxMin[2][1]);
		glEnd();

		glEnable(GL_LIGHTING);

	}
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (IndexedFaceSet)" << endl;	// Message about debuging
	#endif
	
}



IndexedFaceSet *IndexedFaceSet::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
		
	IndexedFaceSet *indexedFaceSet;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((IndexedFaceSet *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	indexedFaceSet = new IndexedFaceSet();
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(indexedFaceSet->DEF,DEF2);
		setLink(indexedFaceSet->DEF,indexedFaceSet);
	}
	
	indexedFaceSet->read(node);

	return indexedFaceSet;
	
}


void *IndexedFaceSet::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "Reading (IndexedFaceSet) " << endl;  
	#endif
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	DOMNodeList *children;			// variable to hold the node children
	
	#ifndef XERCES
		char *ptrptr;
		char *ctemp;				// temporary variable
	#endif

	// place for routing

	attributes = node->getAttributes();	
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "coordIndex") ||
			!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_coordIndex") ) {
			
			#ifdef XERCES  // actualy is the xerces parser 
				
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFInt32[this->counterCoord++] = atoi(XMLString::transcode(tok->nextToken()));
				
					if(x3d::x3DValues->MFInt32[this->counterCoord] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					if(this->counterCoord>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			#else
				
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
				
					x3d::x3DValues->MFInt32[this->counterCoord++] = atoi(ctemp);
				
					if(x3d::x3DValues->MFInt32[this->counterCoord] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					if(this->counterCoord>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
				}
			
			#endif
			
			
			if(x3d::x3DValues->MFInt32[this->counterCoord-1]!=-1) {
				x3d::x3DValues->MFInt32[this->counterCoord++]=-1;
			}
			this->coordIndex = new int[this->counterCoord];
			this->set_coordIndex = this->coordIndex;				// just a copy
			if( coordIndex==NULL ) 
				cerr << "Memory alocation problem at indexedFaceSet" << endl;
			for(unsigned int indexCounter=0;indexCounter < this->counterCoord;indexCounter++) {
				coordIndex[indexCounter] = x3d::x3DValues->MFInt32[indexCounter];
			}
		} else
		if (!strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "colorIndex") ||
			 !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "set_colorIndex") ) {
			
			#ifdef XERCES  // actualy is the xerces parser 				 
			
				 XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFInt32[this->counterColorCoord++] = atoi(XMLString::transcode(tok->nextToken()));
				
					if(x3d::x3DValues->MFInt32[this->counterColorCoord] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					if(this->counterColorCoord>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			#else
				
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFInt32[this->counterColorCoord++] = atoi(ctemp);
					if(x3d::x3DValues->MFInt32[this->counterColorCoord] < -1) {
						cout << "Warning value of ColorIndex outside the range" << endl;
					}
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					if(this->counterColorCoord>=Minimum_MFInt32) {
						cerr << "Error colorIndex bigger than buffer" << endl;
						return(NULL);;
					}
				}
				
			#endif
				
				
			if(x3d::x3DValues->MFInt32[this->counterColorCoord-1]!=-1) {
				x3d::x3DValues->MFInt32[this->counterColorCoord++]=-1;
			}
			this->colorIndex = new int[this->counterColorCoord];
			this->set_colorIndex = this->colorIndex;				// just a copy
			if( colorIndex==NULL ) 
				cerr << "Memory alocation problem at indexedFaceSet" << endl;
			for(unsigned int indexCounter=0;indexCounter < this->counterColorCoord;indexCounter++) {
				colorIndex[indexCounter] = x3d::x3DValues->MFInt32[indexCounter];
			}
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "texCoordIndex") ||
			 !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "set_texCoordIndex") ) {
				
			#ifdef XERCES  // actualy is the xerces parser 				 
				 
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFInt32[this->counterTextureCoord++] = atoi(XMLString::transcode(tok->nextToken()));
				
					if(x3d::x3DValues->MFInt32[this->counterTextureCoord] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					if(this->counterTextureCoord>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			#else
				
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				this->counterTextureCoord=0;		// counter to fill the variable tex coord
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					#ifdef DEBUG
						cout << " CoordIndex ( " << this->counterTextureCoord << " ) : " << ctemp << endl;
					#endif
					x3d::x3DValues->MFInt32[this->counterTextureCoord++] = atoi(ctemp);
					if(x3d::x3DValues->MFInt32[this->counterTextureCoord] < -1) {
						cout << "Warning value of texCoordIndex outside the range" << endl;
					}
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					if(this->counterTextureCoord>=Minimum_MFInt32) {
						cerr << "Error texCoordIndex bigger than buffer" << endl;
						return(NULL);;
					}
				}
				
			#endif
			
			if(x3d::x3DValues->MFInt32[this->counterTextureCoord-1]!=-1) {
				x3d::x3DValues->MFInt32[this->counterTextureCoord++]=-1;
			}
			this->texCoordIndex = new int[this->counterTextureCoord];
			this->set_texCoordIndex = this->texCoordIndex;				// just a copy
			if( texCoordIndex==NULL ) 
				cerr << "Memory alocation problem at indexedFaceSet" << endl;
			for(unsigned int indexCounter=0;indexCounter < this->counterTextureCoord;indexCounter++) {
				texCoordIndex[indexCounter] = x3d::x3DValues->MFInt32[indexCounter];
			}
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "normalIndex") ||
			 !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "set_normalIndex") ) {
				
			#ifdef XERCES  // actualy is the xerces parser 
				 
				XMLStringTokenizer *tok = new XMLStringTokenizer( attributes->item(i)->getNodeValue() , XMLString::transcode(", \t\n\r\f") );
			
				while(tok->hasMoreTokens()) {
					x3d::x3DValues->MFInt32[this->counterNormalCoord++] = atoi(XMLString::transcode(tok->nextToken()));
				
					if(x3d::x3DValues->MFInt32[this->counterNormalCoord] < -1) {
						cout << "Warning value of CoordIndex outside the range" << endl;
					}
					if(this->counterNormalCoord>=Minimum_MFInt32) {
						cerr << "Error coordIndex bigger than buffer" << endl;
						return(NULL);;
					}
			
				}	
			
				delete tok;
				
			#else
				
				if(!XMLString::transcode(attributes->item(i)->getNodeValue(),x3d::message,Size_X3D_Message)) {
					cout << " Message exced the maximun size " << endl;
				}
				ctemp = strtok_r(x3d::message,", \n",&ptrptr);
				while (ctemp != NULL) {
					x3d::x3DValues->MFInt32[this->counterNormalCoord++] = atoi(ctemp);
					if(x3d::x3DValues->MFInt32[this->counterNormalCoord] < -1) {
						cout << "Warning value of normalIndex outside the range" << endl;
					}
					ctemp = strtok_r(NULL,", \n",&ptrptr);
					if(this->counterNormalCoord>=Minimum_MFInt32) {
						cerr << "Error normalIndex bigger than buffer" << endl;
						return(NULL);;
					}
				}
				
			#endif
			
			if(x3d::x3DValues->MFInt32[this->counterNormalCoord-1]!=-1) {
				x3d::x3DValues->MFInt32[this->counterNormalCoord++]=-1;
			}
			this->normalIndex = new int[this->counterNormalCoord];
			this->set_normalIndex = this->normalIndex;				// just a copy
			if( normalIndex==NULL ) 
				cerr << "Memory alocation problem at indexedFaceSet" << endl;
			for(unsigned int indexCounter=0;indexCounter < this->counterNormalCoord;indexCounter++) {
				normalIndex[indexCounter] = x3d::x3DValues->MFInt32[indexCounter];
			}
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"colorPerVertex")) {
			// isto indica se vai ser aplicado por vertice (TRUE) ou face (FALSE)
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"normalPerVertex")) {
			// isto indica se vai ser aplicado por vertice (TRUE) ou face (FALSE)
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"set_colorIndex")) {
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"convex")) {
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"creaseAngle")) {
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"ccw")) {
			// IMPLEMENTAR
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"solid")) {
			if (!strcasecmp(XMLString::transcode(attributes->item(i)->getNodeValue()),"TRUE")) {
				this->solid=true;
			} else {
				this->solid=false;
			}
		} else
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"USE")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"DEF")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Indexed Face Set member not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}
	}

	
	if(!counterCoord) {
		cout << "\033[31m" ;
		cerr << " coordinate index not defined in the IndexedFaceSet" << endl;
		cout << "\033[30m" ;
	}
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			#ifdef DEBUG1
				cout << "Looking for: " << XMLString::transcode(children->item(i)->getNodeName()) << endl;  
			#endif
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Coordinate")) {
				this->coordinate = Coordinate::get(children->item(i));
			} else 
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"TextureCoordinate")) {
				this->textureCoordinate = TextureCoordinate::get(children->item(i));
				textureUse=true;
				if(this->counterTextureCoord==0) {
					cout << "Warning no texture coord index, trying to use coord Index" << endl;
					texCoordIndex = coordIndex;
				}
			} else 
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Normal")) {
				this->normal = Normal::get(children->item(i));
				normalUse=true;
				if(this->counterNormalCoord==0) {
					cout << "Warning no texture coord index, trying to use coord Index" << endl;
					normalIndex = coordIndex;
				}
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Color")) {
				this->color = Color::get(children->item(i));
				colorUse=true;
				if(this->counterColorCoord==0) {
					cout << "Warning no texture coord index, trying to use coord Index" << endl;
					colorIndex = coordIndex;
				}
			} else
			if ( (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(children->item(i)->getNodeName()),"#comment")) ) {
				cout << "Indexed Face Set child not recognized : ";
				cout << XMLString::transcode(children->item(i)->getNodeName()) << endl;
			}
		}
	}
	
	// Isto esta ficando obsolte, substituir
	for(unsigned int ii=0;ii<routeStruct::routeCount;ii++) {
		if (!strcmp (x3d::my_x3d->routeTable[ii]->toNode,DEF)) {
			dynamic=true;
		}
	}
/*
	if(!dynamic) {
	
		#ifdef DEBUG1
			cout << "Case not dynamic hold in memory" << endl;  
		#endif
		
		dynamic=true;
	
		#ifdef DEBUG1
			cout << "Creating OpenGL list" << endl;  
		#endif
		//this->list = glGenLists(1);
		if(this->list==0) {
			//cerr << "Error display list was not generated" << endl;
		}
		
		//glNewList(this->list, GL_COMPILE);
		
		#ifdef DEBUG1
			cout << "Fisrt Draw" << endl;  
		#endif
		//this->draw();
		
		#ifdef DEBUG1
			//cout << "Ending list" << endl;  
		#endif
		//glEndList();
		dynamic=false;
		
	}
	*/
	#ifdef DEBUG1
		cout << "Finishing Reading (IndexedFaceSet) " << endl;  
	#endif
	
	return(NULL);
	
}

Sphere::Sphere() {

	this->radius=1;
	this->solid=true;
	
	this->cast=false;
	
}

void Sphere::follow() {}

void Sphere::draw() {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Sphere)" << endl;	// Message about debuging
	#endif
	
	/*
	float c_x=0.0f;	// center
	float c_y=0.0f;
	float c_z=0.0f;
	*/
	
	short int i = 0;	// inner counter
	short int j = 0;	// outer counter

	double theta1 = 0.0f;
	double theta2 = 0.0f;
	double theta3 = 0.0f;

	float e_x = 0.0f;
	float e_y = 0.0f;
	float e_z = 0.0f;

	float p_x = 0.0f;
	float p_y = 0.0f;
	float p_z = 0.0f;

	if(this->solid) {
		glEnable(GL_CULL_FACE);
	}

	for( j = 0; j < QUALITY/2; ++j ) {
	
		theta1 = j * 2 * PI / QUALITY - (PI/2);
		theta2 = (j + 1) * 2 * PI / QUALITY - (PI/2);

		glBegin(GL_QUAD_STRIP);

		for( i = QUALITY; i >= 0; i-- ) {			// inverting order because poligon orientation
			
			theta3 = i * 2 * PI / QUALITY;

			e_x = cos(theta2) * cos(theta3);
			e_y = sin(theta2);
			e_z = cos(theta2) * sin(theta3);
			/*
			p_x = c_x + radius * e_x;
			p_y = c_y + radius * e_y;
			p_z = c_z + radius * e_z;
			*/
			p_x = radius * e_x;
			p_y = radius * e_y;
			p_z = radius * e_z;
			
			
			glNormal3f( e_x, e_y, e_z );										// setting normal
			glTexCoord2f( 1-(i/(float)QUALITY), 2*(j+1)/(float)QUALITY );		// setting texture
			glVertex3f( p_x, p_y, p_z );										// ploting point

			e_x = cos(theta1) * cos(theta3);
			e_y = sin(theta1);
			e_z = cos(theta1) * sin(theta3);
			
			/*
			p_x = c_x + radius * e_x;
			p_y = c_y + radius * e_y;
			p_z = c_z + radius * e_z;
			*/
			p_x = radius * e_x;
			p_y = radius * e_y;
			p_z = radius * e_z;
						
			glNormal3f( e_x, e_y, e_z );										// setting normal
			glTexCoord2f( 1-(i/(float)QUALITY), 2*j/(float)QUALITY );			// setting texture
			glVertex3f( p_x, p_y, p_z );										// ploting point

		}

		glEnd();
	
	}
	
	if( (cast) && (Render::my_rrr->wandRay) ) {
		glDisable(GL_LIGHTING);
		glColor3d(1,0,0);
		glutWireCube(2*radius);
		glEnable(GL_LIGHTING);
	}

	if(this->solid) {
		glDisable(GL_CULL_FACE);
	}

	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Sphere)" << endl;	// Message about debuging
	#endif
}
	
bool Sphere::ray(float o2[4], float d2[4], float m2[4][4]) {
	
	#ifdef DEBUG2
		cout << "Ray routine " << endl;
	#endif
	
	//if(Render::my_rrr->wandRay) {
	
	float o[4];
	float d[4];
	
	o[0] = o2[0];
	o[1] = o2[1];
	o[2] = o2[2];
	o[3] = o2[3];

	d[0] = d2[0];
	d[1] = d2[1];
	d[2] = d2[2];
	d[3] = d2[3];

	float matrix[4][4];
	
	float matrix2[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	//glGetFloatv(GL_MODELVIEW_MATRIX,matrix2);

	matrix2[0 ]=m2[0][0];	matrix2[1 ]=m2[0][1];	matrix2[2 ]=m2[0][2];	matrix2[3 ]=m2[0][3];
	matrix2[4 ]=m2[1][0];	matrix2[5 ]=m2[1][1];	matrix2[6 ]=m2[1][2];	matrix2[7 ]=m2[1][3];
	matrix2[8 ]=m2[2][0];	matrix2[9 ]=m2[2][1];	matrix2[10]=m2[2][2];	matrix2[11]=m2[2][3];
	matrix2[12]=m2[3][0];	matrix2[13]=m2[3][1];	matrix2[14]=m2[3][2];	matrix2[15]=m2[3][3];
		
	float matrix3[16];
	
	int ok = m4_inverse( matrix3, matrix2 );
	if(!ok) {
		this->cast=false;
		return false; 	// nao encontrou
	}
	
	matrix[0][0]=matrix3[ 0];	matrix[0][1]=matrix3[ 1];	matrix[0][2]=matrix3[ 2];	matrix[0][3]=matrix3[ 3];
	matrix[1][0]=matrix3[ 4];	matrix[1][1]=matrix3[ 5];	matrix[1][2]=matrix3[ 6];	matrix[1][3]=matrix3[ 7];
	matrix[2][0]=matrix3[ 8];	matrix[2][1]=matrix3[ 9];	matrix[2][2]=matrix3[10];	matrix[2][3]=matrix3[11];
	matrix[3][0]=matrix3[12];	matrix[3][1]=matrix3[13];	matrix[3][2]=matrix3[14];	matrix[3][3]=matrix3[15];
	
	multipleMatrix(matrix,d);
	multipleMatrix(matrix,o);

	//glPushMatrix();
	//glLineWidth(3);
	//glColor3f(0.0f,1.0f,0.0f);
	//glBegin(GL_LINES);
	//glVertex3f(o[0],o[1],o[2]);
	//glVertex3f((10*d[0])+o[0],(10*d[1])+o[1],(10*d[2])+o[2]);
	//glEnd();
	//glPopMatrix();
			
	float delta, raiz, t;
 
	Vector u = Vector(d[0],d[1],d[2]);				// valor de u da equacao
	u.normalize();
	Vector v = Vector(-o[0],-o[1],-o[2]);	// valor de v da equacao
	
	float  tmp = (v - (u*v)*u).length();
	delta = this->radius*this->radius - (tmp*tmp);
  
	if (delta < 0) { // para nao calcular raiz negativa
		this->cast=false;
		return false; 	// nao encontrou
	}
  
	raiz = sqrt(delta); // raiz da equacao
  
	if ((t = (u*v) - raiz) > SMALL) { // caso o primeiro ja seja maior que 0
		this->cast=true;
		return true;
	} else if ((t = (u*v) + raiz) > SMALL) { // senao t é o segundo
		this->cast=true;
		return true;
	}
	
	t=t; // somente para nao emitir warnings
	
	this->cast=false;
	return false; // nao encontro

}


Sphere *Sphere::get(DOMNode *node) {

	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Sphere *sphere;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Sphere *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	sphere = new Sphere();
	sphere->read(node);
	
	return sphere;
	
}

void *Sphere::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	#ifdef DEBUG
	cout << "Sphere radius = " << radius << endl;
	#endif
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode(attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "radius")) {
			radius = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
			if(radius<0) { cerr << "Error sphere radius should be greater than zero" << endl; }
		} else
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Sphere member not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}
	}
	
	return(NULL);

}

Text::Text() {
	tstring=NULL; 			//* temporary string variable to decompose leters
}

bool Text::ray(float o2[4], float d2[4], float m2[4][4]) {
	return false;
}

void x3d::createLetters() {
	
	GLuint baseText=0;
	
	baseText = glGenLists(1); // here we must put the number of characteres (usualy 128)
	if(baseText==0) {
			cerr << "Error display list was not generated" << endl;
	}

	//cout << "Creating text" << endl;
	
	glListBase(baseText);
	
	glNewList(baseText+'a', GL_COMPILE);
		Char_a();
		glTranslated(1,0,0);
	glEndList();		
	glNewList(baseText+' ', GL_COMPILE);
		glTranslated(1,0,0);
	glEndList();
	

}

void Text::follow() {}

void Text::draw() {
	
	glPushMatrix();
	
	GLsizei len = (GLsizei) strlen(this->tstring);
	glCallLists(len, GL_BYTE, (GLbyte *)this->tstring);
	
	/*
	for(unsigned int f=0 ; f < sizeof(this->tstring) ; f++ )
		switch(tstring[f]) {
			case 'a':
				x3d::my_x3d->Char_a();
				glTranslated(1,0,0);
				break;
			
			default:
				break;	
		}
	*/
	
	glPopMatrix();
	
}


Text *Text::get(DOMNode *node) {
	Text *text = new Text();
	Text *text2;
	
	text2 = (Text *)text->read(node);
	
	if(text2!=NULL) {
		delete text;
		return text2;
	} 
	
	return text;
	
}

void *Text::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			//strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "string")) {
			this->tstring = XMLString::transcode (attributes->item(i)->getNodeValue());
		} else
		if ( (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#text")) && 
			 (strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"#comment")) ) {
			cout << "Text memeber not recognized : ";
			cout << XMLString::transcode(attributes->item(i)->getNodeName()) << endl;
		}
	}

	return(NULL);
	
}
