#ifndef __x3dGeometry3D_h
#define __x3dGeometry3D_h


#include "x3dRendering.h"
#include "x3dFields.h"

#include <xercesc/dom/DOMNode.hpp>

// Includes do OpenGL
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

using namespace std;

class X3DGeometryNode;
class Box : public X3DGeometryNode {
	
	public:
	
	Box();		// constructor
	~Box();		// destructor

	SFNode  *metadata;		//
	SFVec3f *size;			// the size of the box
	SFBool  *solid;			//
	
	void *read(DOMNode *node);
	void draw();
	void follow();
	static Box *get(DOMNode *node);
	bool ray(float o2[4], float d2[4], float m2[4][4]);

	bool cast;
	
};

 
 class Cone : public X3DGeometryNode {
	
	public:
	
	Cone();		// constructor
	~Cone();	// destructor
	
	SFNode  *metadata;		//
	SFBool  *bottom;			// define if shows the bottom
	SFFloat *bottomRadius;	// 
	SFFloat *height;			//
	SFBool  *side;			// define if shows the side
	SFBool  *solid;			//
	
	static Cone *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);

	bool cast;
		 
};

/* Class to cylinder */
class Cylinder : public X3DGeometryNode {
	
	public:
	
	Cylinder();

	SFBool  bottom;		// define if shows the bottom
	SFBool  side;		// define if shows the side
	SFBool  top;		// define if shows the top
	SFFloat radius;		//
	SFFloat height;		//
	SFBool  solid;		//
	
	static Cylinder *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	bool cast;
	
};

class ElevationGrid : public X3DComposedGeometryNode {
	
	public:
	
	ElevationGrid();
	virtual ~ElevationGrid();

	MFFloat set_height;
	//SFNode  color;
	//SFNode  metadata;
	//SFNode  normal;
	//SFNode  texCoord;
	SFBool  ccw;
	SFBool  colorPerVertex;
	SFFloat creaseAngle;
	MFFloat height;
	SFBool  normalPerVertex;
	SFBool  solid;
	SFInt32 xDimension;
	SFFloat xSpacing;
	SFInt32 zDimension;
	SFFloat zSpacing;
	
	static ElevationGrid *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	double* coord;
	double* normal;
	double* texture;
	
	unsigned int coordIndex[16384];

	float *values;
	bool firstPass;
	unsigned int m_nVBOs;


};


class Extrusion : public X3DComposedGeometryNode {
	
	public:
	
	Extrusion();
	virtual ~Extrusion();

	SFBool convex;
	SFBool solid;
	double crossSection[2][2048];
	double scale[2][2048];
	double spine[3][2048];
	
	static Extrusion *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	bool defScale;					// to define the use of scale or not
	unsigned short int scalePoints;		// number of scale points
	unsigned short int csPoints;		// number of crossSection points
	unsigned short int spinePoints;		// number of spine points
	
	unsigned int *coordIndex;
	double 		 *coord;
	double 		 *texture;

	float *values;
	bool firstPass;
	unsigned int m_nVBOs;


};

//#define JINX_DOUBLE in the case of using double instead float

class datavector;
class Coordinate;
class TextureCoordinate;
class Normal;
class Color;
class X3DComposedGeometryNode;
class IndexedFaceSet : public X3DComposedGeometryNode {
	
	public:
	
	IndexedFaceSet();
	virtual ~IndexedFaceSet();

	MFInt32 set_colorIndex;
	MFInt32 set_coordIndex;
	MFInt32 set_normalIndex;
	MFInt32 set_texCoordIndex;
	//SFNode  color;
		Color *color;
	//SFNode  coord;
		Coordinate *coordinate;
	//SFNode  metadata;
	//SFNode  normal;
		Normal *normal;
	//SFNode  texCoord;
		TextureCoordinate *textureCoordinate;
	SFBool  ccw;
	MFInt32 colorIndex;
	SFBool  colorPerVertex;
	SFBool  convex;
	MFInt32 coordIndex;
	SFFloat creaseAngle;
	MFInt32 normalIndex;
	SFBool  normalPerVertex;
	SFBool  solid;
	MFInt32 texCoordIndex;
	
	static IndexedFaceSet *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	bool dynamic;			// case the mesh is dynamic or not
	unsigned int list;		// hold the display list GLuint

	float maxMin[3][2];		// hold the maximun and minumun value of the vertexes in xyz
	bool cast;				// if the ray is casting or not
	
	unsigned int counterCoord;			// counter to fill the variable coord
	unsigned int counterTextureCoord;	// counter to fill the variable tex coord
	unsigned int counterNormalCoord;	// counter to fill the variable normal coord
	unsigned int counterColorCoord;		// counter to fill the variable color coord
	
	bool textureUse;
	bool normalUse;
	bool colorUse;



	GLenum *type;
	GLuint **indices;			// the points to make the polygon (pieces)
	unsigned int *indicesSize; // holt the number of values

	bool firstPass;
	unsigned int counterIndices;		// the polygons (pieces)
	float *values;  // to hold vertexes
	
	unsigned int m_nVBOs;
	unsigned int *indicesID;
	
	GLuint *start;
	GLuint *end;


};

class Text : public X3DGeometryNode {
	
	public:
	
	Text();

	//SFNode   fontStyle;
	MFFloat  length;
	SFFloat  maxExtent;
	//SFNode   metadata;
	//MFString string;
	char* tstring; 			//* temporary string variable to decompose leters
	SFBool   solid;
		
	static Text *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
};


class Sphere : public X3DGeometryNode {
	
	public:
	
	Sphere();

	static Sphere *get(DOMNode *node);
	
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	bool cast;
	
	//SFNode  [in,out] metadata NULL [X3DMetadataObject]
  	SFFloat radius;
  	SFBool  solid;
	
	
};



#endif
