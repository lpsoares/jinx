#ifndef __x3dTexturing_h
#define __x3dTexturing_h

#include "x3d.h"
#include "x3dFields.h"

using namespace std;

#include <xercesc/dom/DOMNode.hpp>



class X3DTextureCoordinateNode : public X3DNode {
	public:
};
	
class datavector;
class TextureCoordinate : public X3DTextureCoordinateNode { // corrigir
	
	public:
	
	TextureCoordinate();
	virtual ~TextureCoordinate();

	void *read(DOMNode *node);
	void follow();
	static TextureCoordinate *get(DOMNode *node);
		
	bool ami(char *type) { return( !strcmp(type,"TextureCoordinate") ); }
	
	unsigned int f;						// counter to fill the variable coord
	
	datavector *value;
	
};


/*TextureTransform defines a 2D transformation that is applied to texture coordinates*/
class TextureTransform : public X3DNode { // corrigir
	
	public:
	
	TextureTransform();
	
	void *read(DOMNode *node);
	void follow();
	static TextureTransform *get(DOMNode *node);
	
	SFVec2f center;
	//SFNode  [in,out] metadata    NULL [X3DMetadataObject]
	SFFloat rotation;
	SFVec2f scale;
	SFVec2f translation;
	
	bool ami(char *type) { return( !strcmp(type,"TextureTransform") ); }
	
};






/*ImageTexture defines a texture map by specifying an image file and parameters for mapping*/
class ImageTexture : public X3DNode { // corrigir
	
	public:
	
	ImageTexture();

	void *read(DOMNode *node);
	void follow();
	static ImageTexture *get(DOMNode *node);
	
	char url[16][256];
	SFBool repeatS; 
  	SFBool repeatT;
	
	bool ami(char *type) { return( !strcmp(type,"ImageTexture") ); }
	
	unsigned int couter_url;
	
};



class MovieTexture : public X3DNode { // corrigir
	
	public:
	
	MovieTexture();

	static MovieTexture *get(DOMNode *node);
		
	void *read(DOMNode *node);
	void follow();
	
	char *url1;
	char *url2;

	char *url;
	
	int startFrame;
	int stopFrame;
	
	bool ami(char *type) { return( !strcmp(type,"MovieTexture") ); }
	
	
	int temp; // par animacao remover
	
	
};









// INVENTADA


/*ImageTexture defines a texture map by specifying an image file and parameters for mapping*/
class CubeMapping : public X3DNode { // corrigir
	
	public:
	
	CubeMapping();

	void *read(DOMNode *node);
	void follow();
	static CubeMapping *get(DOMNode *node);
	
	char urlFront[16][256];
	char urlBack[16][256];
	char urlTop[16][256];
	char urlBottom[16][256];
	char urlRight[16][256];
	char urlLeft[16][256];
	
	SFBool repeatS; 
  	SFBool repeatT;
	
	unsigned int couter_urlFront;		// in the case of many texture files in one specification
	unsigned int couter_urlBack;		// in the case of many texture files in one specification
	unsigned int couter_urlTop;		// in the case of many texture files in one specification
	unsigned int couter_urlBottom;		// in the case of many texture files in one specification
	unsigned int couter_urlLeft;		// in the case of many texture files in one specification
	unsigned int couter_urlRight;		// in the case of many texture files in one specification
	
};








#endif
