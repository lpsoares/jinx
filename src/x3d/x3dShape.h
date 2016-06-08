#ifndef __x3dShape_h
#define __x3dShape_h

#ifdef GLEW
#include "../render/ezGLSL.h"
#endif

#include "x3dShading.h"
#include "x3dGrouping.h"
#include "x3dGeometry3D.h"
#include "x3dTexturing.h"
#include "x3dRendering.h"
#include "x3dNurbs.h"
#include "x3dGrouping2.h"


#include <xercesc/dom/DOMNode.hpp>

using namespace std;

class X3DAppearanceChildNode : public X3DNode {
	
	public:

	SFNode *metadata;
	
	virtual ~X3DAppearanceChildNode() { }
	
};

class X3DAppearanceNode : public X3DNode {
  
	public:
	
	SFNode *metadata;
	
};


class X3DShapeNode : public X3DChildNode, public X3DBoundedObject {

	public:
	
	virtual ~X3DShapeNode() { }
	
	
	SFNode  *appearance;
	SFNode  *geometry;
	SFNode  *metadata;
	SFVec3f *bboxCenter;
	SFVec3f *bboxSize;
	
	
};

class X3DMaterialNode : public X3DAppearanceChildNode {
	
	public:

	virtual ~X3DMaterialNode() { }
	
	SFNode *metadata;
	
};

class LineProperties : public X3DAppearanceChildNode { // corrigir esta hierarquia
	
	
	public:
	
	LineProperties();

	SFBool  *applied;
	SFInt32 *linetype;
	SFFloat *linewidthScaleFactor;
	SFNode  *metadata;
		
	static LineProperties *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	
	int factor;
	unsigned short pattern;
	float width;

	
};


class Material : public X3DMaterialNode {
	
	public:
	
	Material();

	SFFloat *ambientIntensity;
	SFColor *diffuseColor;
	SFColor *emissiveColor;
	SFNode  *metadata;
	SFFloat *shininess;
	SFColor *specularColor;
	SFFloat *transparency;
	
	static Material *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	
	float tmpSpecularColor[4];
	float tmpDiffuseColor[4];
	float tmpEmissiveColor[4];
		
};



class ShaderAppearance : public X3DAppearanceNode {
	
	public:
	
	ShaderAppearance();

	//-----------------------------

	#ifdef GLEW
		// TO GLSL
		//Shader and Program Objects:
		ezShaderManager shadermanager;	
		ezShaderObject* myShader; 
	#endif

	
	VertexShader *vertexShader;
	FragmentShader *fragmentShader;
		
	//-----------------------------
	
	
	SFNode *fillProperties;
	//SFNode *lineProperties;
	LineProperties *lineProperties;
	//SFNode material;
	Material *material;
	SFNode *metadata;
	//SFNode texture;
	ImageTexture *imageTexture;
	MovieTexture *movieTexture;
	//SFNode textureTransform;
	TextureTransform *textureTransform;
	
	CubeMapping *cubeMapping;
	
	static ShaderAppearance *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	
};




class Appearance : public X3DAppearanceNode {
	
	public:
	
	Appearance();

	SFNode *fillProperties;
	//SFNode *lineProperties;
	LineProperties *lineProperties;
	//SFNode material;
	Material *material;
	SFNode *metadata;
	//SFNode texture;
	ImageTexture *imageTexture;
	MovieTexture *movieTexture;
	//SFNode textureTransform;
	TextureTransform *textureTransform;
	
	CubeMapping *cubeMapping;
	
	static Appearance *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	
};



class X3DGeometryNode;
class Shape : public X3DShapeNode { // corrigir
	
	public:
	
	Shape();


	//SFNode  *appearance;
	Appearance *appearance;
	
	// CHECK IF ITS IS RIGHT
	ShaderAppearance *shaderAppearance;
	
	//SFNode  *geometry;
	X3DGeometryNode *geometry[1024];  //checar tambem
	SFNode  *metadata;
	SFVec3f *bboxCenter;
	SFVec3f *bboxSize;
	
	
	static Shape *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4], float m[4][4]);

	int countGeometry;
	
};

/*   Conflitando com o Xerces
class FillProperties : public X3DAppearanceChildNode { 

	public :
	
	SFBool  *filled;
	SFColor *hatchColor;
	SFBool  *hatched;
	SFInt32 *hatchStyle;
	SFNode  *metadata;
	
}
*/

#endif
