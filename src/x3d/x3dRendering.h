#ifndef __x3dRendering_h
#define __x3dRendering_h

#include "x3d.h"
#include "x3dGrouping.h"
#include "x3dCore.h"


using namespace std;

#include <xercesc/dom/DOMNode.hpp>

class X3DGeometricPropertyNode : public X3DNode { 
	
	public:
	
	SFNode *metadata;
	
};

class X3DColorNode: public X3DGeometricPropertyNode { 
	
	public:
	
	SFNode *metadata;
	
};

class X3DCoordinateNode : public X3DGeometricPropertyNode {
  
	SFNode *metadata;
	
};



class X3DGeometryNode : public X3DChildNode {
	
	public:

	X3DGeometryNode();
	
	SFNode *metadata;
		
	virtual void draw()=0;	// pure virtual function to draw
	
};


class X3DNormalNode : public X3DGeometricPropertyNode {
	
	public:
	
	SFNode *metadata;

};


class X3DGeometry3DNode :  public X3DGeometryNode {
	public:
};

class X3DComposedGeometryNode : public X3DGeometry3DNode {
	
	public:
		
	SFNode *color;
	SFNode *coord;
	SFNode *metadata;
	SFNode *normal;
	SFNode *texCoord;
	SFBool *ccw;
	SFBool *colorPerVertex;
	SFBool *normalPerVertex;
	SFBool *solid;
	
};



class datavector;
class Color : public X3DColorNode {
	
	public:
	
	Color();
	virtual ~Color();

	//MFColor *color;
	datavector *value;
	SFNode  *metadata;
	
	static Color *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();

	unsigned int numberOfElements;				// counter to fill the variable coord
	unsigned int getNumberOfElements() { return numberOfElements; }
	
};

class ColorRGBA : public X3DColorNode {
	
	public:
	
	MFColorRGBA *color;
	SFNode      *metadata;
	
};

class Normal : public X3DNormalNode { // corrigir esta hierarquia
	
	public:
	
	Normal();
	virtual ~Normal();

	SFNode  *metadata;
	//MFVec3f *vector;
	datavector *value;
	
	static Normal *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();

	unsigned int numberOfElements;				// counter to fill the variable coord
	unsigned int getNumberOfElements() { return numberOfElements; }
	
	
};


class Coordinate : public X3DCoordinateNode { // corrigir esta hierarquia
	
	public:

	Coordinate();		// Constructor
	virtual ~Coordinate();	// Destructor
		
	SFNode  *metadata;
  	//MFVec3f *point;
	datavector *value;
	
	static Coordinate *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();

	int numberOfElements;				// counter to fill the variable coord
	unsigned int getNumberOfElements() { return numberOfElements; }
	
};



class PointSet : public X3DGeometryNode {
	
	public:
	
	PointSet();
	virtual ~PointSet();

	//SFNode *color;
	Color *color;
	//SFNode *coord;
	Coordinate *coordinate;
	SFNode *metadata;
		
	static PointSet *get(DOMNode *node);
	void *read(DOMNode *node);
	void draw();
	void follow();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
	
	int f;						//* counter to fill the variable coord
	
	int colorNumber;
	int coordinateNumber;
	
	
};

class IndexedLineSet : public X3DGeometryNode {
	
	public:
	
	IndexedLineSet();
	virtual ~IndexedLineSet();

	MFInt32 *set_colorIndex;
	MFInt32 *set_coordIndex;
	
	//SFNode  *color;
		Color *color;
	//SFNode  *coord;
		Coordinate *coordinate;
	
	SFNode  *metadata;
	//MFInt32 *colorIndex;
	int *colorIndex;
	SFBool  *colorPerVertex;
	//MFInt32 *coordIndex;
	int *coordIndex;  //verificar
	
	
	static IndexedLineSet *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	void draw();
	bool ray(float o2[4], float d2[4], float m2[4][4]);
		
	int f;						// counter to fill the variable coord
	
	int counterColor;
	
	bool firstPass;
	
	unsigned int m_nVBOs;
	unsigned int colorVBOs;


	
};



class IndexedTriangleFanSet : public X3DComposedGeometryNode {
	
	public:
	
	MFInt32 *set_index;
	SFNode  *color;
	SFNode  *coord;
	SFNode  *metadata;
	SFNode  *normal;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *colorPerVertex;
	SFBool  *normalPerVertex;
	SFBool  *solid;
	MFInt32 *index;
	
};


class IndexedTriangleSet : public X3DComposedGeometryNode {
	
	public:
	
	MFInt32 *set_index;
	SFNode  *color;
	SFNode  *coord;
	SFNode  *metadata;
	SFNode  *normal;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *colorPerVertex;
	SFBool  *normalPerVertex;
	SFBool  *solid;
	MFInt32 *index;
	
};


class IndexedTriangleStripSet : public X3DComposedGeometryNode {

	public:
	
	MFInt32 *set_index;
	SFNode  *color;
	SFNode  *coord;
	SFFloat *creaseAngle;
	SFNode  *metadata;
	SFNode  *normal;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *normalPerVertex;
	SFBool  *solid;
	MFInt32 *index;
	
};


class LineSet : public X3DGeometryNode {
	
	public:
	
	SFNode  *color;
	SFNode  *coord;
	MFInt32 *vertexCount;
	SFNode  *metadata;
	
};


class TriangleFanSet : public X3DComposedGeometryNode {
	
	public:
	
	SFNode  *color;
	SFNode  *coord;
	MFInt32 *fanCount;
	SFNode  *metadata;
	SFNode  *normal;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *colorPerVertex;
	SFBool  *normalPerVertex;
	SFBool  *solid;
	
};


class TriangleSet : public X3DComposedGeometryNode {

	public:
	
	SFNode  *color;
	SFNode  *coord;
	SFNode  *metadata;
	SFNode  *normal;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *colorPerVertex;
	SFBool  *normalPerVertex;
	SFBool  *solid;

};



class TriangleStripSet : public X3DComposedGeometryNode {

	public:
	
	SFNode  *color;
	SFNode  *coord;
	SFNode  *metadata;
	SFNode  *normal;
	MFInt32 *stripCount;
	SFNode  *texCoord;
	SFBool  *ccw;
	SFBool  *colorPerVertex;
	SFBool  *normalPerVertex;
	SFBool  *solid;

};
	

#endif
