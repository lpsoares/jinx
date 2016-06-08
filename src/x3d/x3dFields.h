

#include "x3dCore.h" // tava de saco cheio e funcionou talvez este seja o modo certo


#ifndef __x3dFields_h
#define __x3dFields_h



#include <xercesc/dom/DOMNode.hpp>


using namespace std;

// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
	XERCES_CPP_NAMESPACE_USE
#endif


class X3DImage {
	
	public:
		
	unsigned char **image;
	int width;
	int height;
	int components;
	
};


class X3DChildNode;


/* defining the fields here */
typedef bool   			SFBool;
typedef bool*  			MFBool;
typedef float  			SFColor[3];
typedef float* 			MFColor[3];
typedef float  			SFColorRGBA[4];
typedef float* 			MFColorRGBA[4];
typedef double 			SFDouble;
typedef double*			MFDouble;
typedef float  			SFFloat;
typedef float* 			MFFloat;
typedef X3DImage		SFImage;
typedef X3DImage*		MFImage;
typedef int   			SFInt32;
typedef int*   			MFInt32;
typedef X3DChildNode	SFNode;
typedef X3DChildNode	MFNode;  // corrigir
typedef float  			SFRotation[4];
typedef char   			SFString[256];
typedef char   			MFString[16][256];
typedef double 			SFTime;
typedef double* 		MFTime;
typedef double 			SFVec2d[2];
typedef double* 		MFVec2d[2];
typedef float  			SFVec2f[2];
typedef float* 			MFVec2f[2];
typedef double 			SFVec3d[3];
typedef double*			MFVec3d[3];
typedef float  			SFVec3f[3];
typedef float*			MFVec3f[3];




// Used in Scripts and Protos
class field {
	
	public:
	
	field();	// constructor
	
	void *read(DOMNode *node);
	
	char name[256];
	char type[256];
	
	unsigned short int accessType;
	/*
	AccessType
	1 - inputOnly
	2 - outputOnly
	*/
	
	void *value;		// void pointer to be alloced to hold the value
	
	
};


#endif
