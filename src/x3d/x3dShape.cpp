


#ifdef GLEW
	// To GLSL
	#include "../render/ezGLSL.h"
#endif

#include "x3dShape.h"


#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>



ShaderAppearance::ShaderAppearance() {

	#ifdef GLEW
		// TO GLSL
		this->myShader = 0; 
		//---------------
	#endif
	
	this->vertexShader=NULL;
	this->fragmentShader=NULL;
	
	this->material=NULL;
	this->imageTexture=NULL;
	this->movieTexture=NULL;
	this->textureTransform=NULL;
	this->cubeMapping=NULL;
	
}

void ShaderAppearance::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (ShaderAppearance)" << endl;	// Message about debuging
	#endif

	/*
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	*/
	
	
	//---------------------
	
	if(this->fragmentShader!=NULL) {
		fragmentShader->follow();
	}
	
	
	if(this->vertexShader!=NULL) {
		vertexShader->follow();
	}
	
	//---------------------
	
	
	
	if(this->material!=NULL) {
		material->follow();
	}



	#ifdef GLEW
	   myShader->sendUniform4f("color0",material->tmpDiffuseColor[0]*0.6,material->tmpDiffuseColor[1]*0.6,material->tmpDiffuseColor[2]*0.6,material->tmpDiffuseColor[3]);
	   myShader->sendUniform4f("color1",material->tmpDiffuseColor[0]*0.4,material->tmpDiffuseColor[1]*0.4,material->tmpDiffuseColor[2]*0.4,material->tmpDiffuseColor[3]);
	   myShader->sendUniform4f("color2",material->tmpSpecularColor[0],material->tmpSpecularColor[1],material->tmpSpecularColor[2],material->tmpSpecularColor[3]);
	#endif
	
	

	
	if(this->imageTexture!=NULL) {
		imageTexture->follow();
	}

	if(this->movieTexture!=NULL) {
		movieTexture->follow();
	}

	if(this->textureTransform!=NULL) {
		textureTransform->follow();
	}

	if(this->cubeMapping!=NULL) {
		cubeMapping->follow();
	}

	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (ShaderAppearance)" << endl;	// Message about debuging
	#endif
	
}

ShaderAppearance *ShaderAppearance::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	ShaderAppearance *appearance;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((ShaderAppearance *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	appearance = new ShaderAppearance();
	appearance->read(node);
	
	return appearance;

}

void *ShaderAppearance::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter

	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

	DOMNodeList *children;			//* variable to hold the node children
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			
			if (!strcmp(XMLString::transcode (children->item(i)->getNodeName()),"VertexShader")) {
				this->vertexShader = VertexShader::get(children->item(i));
			} else
			if (!strcmp(XMLString::transcode (children->item(i)->getNodeName()),"FragmentShader")) {
				this->fragmentShader = FragmentShader::get(children->item(i));
			} else
			
			
			if (!strcmp(XMLString::transcode (children->item(i)->getNodeName()),"Material")) {
				this->material = Material::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"ImageTexture")) {
				this->imageTexture = ImageTexture::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"MovieTexture")) {
				this->movieTexture = MovieTexture::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"TextureTransform")) {
				this->textureTransform = TextureTransform::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"LineProperties")) {
				this->lineProperties = LineProperties::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"CubeMapping")) {
				this->cubeMapping = CubeMapping::get(children->item(i));
			} 
		}
	}

	
	//----------
	
	

	#ifdef DEBUG1	
		cout << "Creating Shader" << endl;
		cout << "\tVertex   : " << this->vertexShader->getURL() << endl;
		cout << "\tFragment : " << this->fragmentShader->getURL() << endl;
	#endif
	
	
	// Initialize the "OpenGL Extension Wrangler" library
	#ifdef DEBUG1
		cout << "Initializing GLEW  ((AGAIN )) " << endl;
	#endif

#ifdef GLEW
    glewInit(); // I have to repete otherwise doesn't work

    this->myShader = shadermanager.loadfromFile(this->vertexShader->getURL(),this->fragmentShader->getURL());
	
	if (myShader == 0) {
		cout << "Can't create shader object." << endl;
	}

	//sleep(5);  // nao resolveu
	
	//----------
	
#endif
		
	#ifdef DEBUG1
		cout << "Returning (ShaderAppearance) " << endl;  
	#endif

	
	return(NULL);
	
}





Appearance::Appearance() {

	this->material=NULL;
	this->imageTexture=NULL;
	this->movieTexture=NULL;
	this->textureTransform=NULL;
	this->cubeMapping=NULL;
	
	
	
}

void Appearance::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Appearance)" << endl;	// Message about debuging
	#endif
	
	if(this->material!=NULL) {
		material->follow();
	}
	
	if(this->imageTexture!=NULL) {
		imageTexture->follow();
	}

	if(this->movieTexture!=NULL) {
		movieTexture->follow();
	}

	if(this->textureTransform!=NULL) {
		textureTransform->follow();
	}

	if(this->cubeMapping!=NULL) {
		cubeMapping->follow();
	}

	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Appearance)" << endl;	// Message about debuging
	#endif
	
}

Appearance *Appearance::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	Appearance *appearance;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Appearance *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	appearance = new Appearance();
	appearance->read(node);
	
	return appearance;

}

void *Appearance::read(DOMNode *node) {

	unsigned int i; 				//* variable to counter

	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

	DOMNodeList *children;			//* variable to hold the node children
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength (); i++) {			
			if (!strcmp(XMLString::transcode (children->item(i)->getNodeName()),"Material")) {
				this->material = Material::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"ImageTexture")) {
				this->imageTexture = ImageTexture::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"MovieTexture")) {
				this->movieTexture = MovieTexture::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"TextureTransform")) {
				this->textureTransform = TextureTransform::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"LineProperties")) {
				this->lineProperties = LineProperties::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode (children->item(i)->getNodeName()),"CubeMapping")) {
				this->cubeMapping = CubeMapping::get(children->item(i));
			} 
		}
	}

	return(NULL);
	
}

LineProperties::LineProperties() {
	//lineStyle=0;
	factor=3;
	pattern = 0x5555;
	width=2;
	
}

void LineProperties::follow() {
	glLineWidth(width);
	
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(factor,pattern);

}


LineProperties *LineProperties::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	LineProperties *lineProperties;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((LineProperties *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	lineProperties = new LineProperties();
	lineProperties->read(node);
	
	return lineProperties;

}


void *LineProperties::read(DOMNode *node) {
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}
	
	#ifdef DEBUG
	float lsizes[2];
	float lstep;
	glGetFloatv(GL_LINE_WIDTH_RANGE,lsizes);
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY,&lstep);
	cout << "Max and Min line width " << lsizes[0] << " , " << lsizes[1] << " Granularity " << lstep << endl;
	#endif

	return(NULL);
}

Material::Material() {

		
}
	

void Material::follow() {
	
	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Material)" << endl;	// Message about debuging
	#endif
	
	tmpSpecularColor[0]=*this->specularColor[0];
	tmpSpecularColor[1]=*this->specularColor[1];
	tmpSpecularColor[2]=*this->specularColor[2];
	tmpSpecularColor[3]=1.0f-*this->transparency;

	tmpDiffuseColor[0]=*this->diffuseColor[0];
	tmpDiffuseColor[1]=*this->diffuseColor[1];
	tmpDiffuseColor[2]=*this->diffuseColor[2];
	tmpDiffuseColor[3]=1.0f-*this->transparency;
	
	tmpEmissiveColor[0]=*this->emissiveColor[0];
	tmpEmissiveColor[1]=*this->emissiveColor[1];
	tmpEmissiveColor[2]=*this->emissiveColor[2];
	tmpEmissiveColor[3]=1.0f-*this->transparency;
	
	glShadeModel(GL_SMOOTH);
	
	
	// Warning you have to set all materials because the others are in cache and will affect future objects
	
	#ifdef DEBUG
		cout << " Specular Color = " ;
		cout << *this->specularColor[0] << "  " ;
		cout << *this->specularColor[1] << "  " ;
		cout << *this->specularColor[2] << endl ;
	#endif
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpSpecularColor);		// set opengl specular color
	
	#ifdef DEBUG
		cout << " Diffuse Color = " ;
		cout << *this->diffuseColor[0] << "  " ;
		cout << *this->diffuseColor[1] << "  " ;
		cout << *this->diffuseColor[2] << endl ;
	#endif
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmpDiffuseColor);		// set opengl diffuse color
	
	
	#ifdef DEBUG
		cout << " Diffuse Color = " ;
		cout << *this->emissiveColor[0] << "  " ;
		cout << *this->emissiveColor[1] << "  " ;
		cout << *this->emissiveColor[2] << endl ;
	#endif
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, tmpEmissiveColor);		// set opengl emisive color
	
	
	// NAO REMOVER, MOLECULAS PARAM DE FUNCIONAR
	// Errado mais certo agora
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(tmpDiffuseColor[0]/1,tmpDiffuseColor[1]/1,tmpDiffuseColor[2]/1,tmpDiffuseColor[3]);
	glDisable(GL_COLOR_MATERIAL);
	
	
	#ifdef DEBUG
		cout << " Shininess = " ;
		cout << *this->shininess << endl ;
	#endif
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, *this->shininess);		// set opengl shininess effect
	

	
	
	if(*this->transparency>0.01f) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3,0x0000);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glDisable(GL_LINE_STIPPLE);
		glLineStipple(3,0xFFFF);
	}
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Material)" << endl;	// Message about debuging
	#endif
	
}

Material *Material::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes

	char DEF2[256];					// temporary variable to hold the DEF name
	strcpy(DEF2,"");					// reseting the variable
	
	Material *material;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF2,XMLString::transcode (attributes->item(i)->getNodeValue()));
			break;
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Material *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}	
	
	material = new Material();
	
	if(strcmp(DEF2,"")) {
		#ifdef DEBUG3
			cout << "DEF : " << DEF2 << endl;
		#endif
		strcpy(material->DEF,DEF2);
		setLink(material->DEF,material);
	}
	
	material->read(node);
	
	return material;
	
}

void *Material::read(DOMNode *node) {
	
	this->transparency = NULL;
	
	unsigned int f;
	unsigned int f2;
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if( (!strcmp(x3d::my_x3d->routeTable[f]->toField,"transparency")) || (!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_transparency")) ) {

				if(transparency == NULL) {
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								if( ( (*x3d::my_x3d->routeTable[f2]->field2) != NULL ) ) {
									(*x3d::my_x3d->routeTable[f]->field2) = (*x3d::my_x3d->routeTable[f2]->field2);
									break;
								}
							}
						}
					}
					if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
						(*x3d::my_x3d->routeTable[f]->field2) = new SFFloat;
					}
					for(f2=0;f2<routeStruct::routeCount;f2++) {
						if ( (!strcmp (x3d::my_x3d->routeTable[f2]->fromNode,x3d::my_x3d->routeTable[f]->fromNode)) && (f!=f2)  ) {
							if(!strcmp(x3d::my_x3d->routeTable[f2]->fromField,x3d::my_x3d->routeTable[f]->fromField)) {
								(*x3d::my_x3d->routeTable[f2]->field2) = (*x3d::my_x3d->routeTable[f]->field2);
							}
						}
					}
					transparency = (SFFloat *)(*x3d::my_x3d->routeTable[f]->field2);
				} else {
					(*x3d::my_x3d->routeTable[f]->field2) = transparency;
				}

			} 
		}
	}
	#ifdef DEBUG3
		cout << DEF << "transparency address : " << transparency << endl;
	#endif
	
	this->diffuseColor = new SFColor[3];
	*this->diffuseColor[0]=0.8f;
	*this->diffuseColor[1]=0.8f;
	*this->diffuseColor[2]=0.8f;
	
	this->shininess = new SFFloat;
	*this->shininess = 0.2;
	
	this->specularColor = new SFColor[3];
	*this->specularColor[0]=0.0;
	*this->specularColor[1]=0.0;
	*this->specularColor[2]=0.0;
	
	if(this->transparency == NULL) {
		this->transparency = new SFFloat;
	}
	*this->transparency = 0.0f;

	this->emissiveColor = new SFColor[3];
	*this->emissiveColor[0]=0.0;
	*this->emissiveColor[1]=0.0;
	*this->emissiveColor[2]=0.0;
	
	
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "diffuseColor")) {
			*this->diffuseColor[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			*this->diffuseColor[1] = atof(strtok(NULL," "));
			*this->diffuseColor[2] = atof(strtok(NULL," "));
			#ifdef DEBUG
				cout << " Diffuse Color = " ;
				cout << *this->diffuseColor[0] << "  " ;
				cout << *this->diffuseColor[1] << "  " ;
				cout << *this->diffuseColor[2] << endl ;
			#endif
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "specularColor")) {
			*this->specularColor[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			*this->specularColor[1] = atof(strtok(NULL," "));
			*this->specularColor[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "emissiveColor")) {
			*this->emissiveColor[0] = atof(strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," "));
			*this->emissiveColor[1] = atof(strtok(NULL," "));
			*this->emissiveColor[2] = atof(strtok(NULL," "));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "shininess")) {
			*this->shininess = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "transparency")) {
			*this->transparency = atof(XMLString::transcode (attributes->item(i)->getNodeValue()));
			if(*this->transparency>1.0f) *this->transparency=1.0f;	// limit up
			if(*this->transparency<0.0f) *this->transparency=0.0f;	// limit down
		}
	}
	
	return(NULL);
	
}

Shape::Shape() {
	
	this->appearance=NULL;
	this->shaderAppearance=NULL;
	
	this->countGeometry=0;
	
}

bool Shape::ray(float o[4],float d[4], float m[4][4]) {

	#ifdef DEBUG2
		cout << "\t\t\t\tCasting Ray (Shape)" << endl;	// Message about debuging
	#endif
	
	
	int f;
	
	if(this->appearance!=NULL) { 
		//this->appearance->follow();
	}
	
	
	if(this->shaderAppearance!=NULL) { 
		//this->shaderAppearance->follow();
	}
		
	for(f=0;f<this->countGeometry;f++) {
		if(this->geometry[f]->ray(o,d,m)) {
			
			// Isto pode ser um problema pois não passa em todos os objetos
			return(true);
			
			
		}
	}

	
	#ifdef DEBUG2
		cout << "\t\t\t\tLeaving Casting Ray (Shape)" << endl;	// Message about debuging
	#endif
	
	
	return(false);
	
}

void Shape::follow() {

	#ifdef DEBUG2
		cout << "\t\t\t\tFollowing (Shape)" << endl;	// Message about debuging
	#endif
	
	unsigned int f;

	#ifdef DEBUG2
		cout << "\t\t\t\t\tLooking for Appearance" << endl;	// Message about debuging
	#endif
	if(this->appearance!=NULL) { 
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\tReading appearance " 
			<< " addr : " << this->appearance << endl;	// Message about debuging
		#endif
		this->appearance->follow();
	}
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\tLooking for Appearance" << endl;	// Message about debuging
	#endif
	if(this->shaderAppearance!=NULL) { 
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\tReading appearance " 
			<< " addr : " << this->appearance << endl;	// Message about debuging
		#endif
		
		
		#ifdef GLEW
			// GLSL
			#ifdef DEBUG2
				cout << "\t\t\t\t\t\tBeggining Shader" << endl;	// Message about debuging
			#endif
			this->shaderAppearance->myShader->begin();
		#endif
		this->shaderAppearance->follow();
	}
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\tLooking for Geometry" << endl;	// Message about debuging
	#endif
	for(f=0;f<this->countGeometry;f++) {
		this->geometry[f]->draw(); 
	}

	
	
	if(this->shaderAppearance!=NULL) { 
		#ifdef GLEW
			#ifdef DEBUG2
				cout << "\t\t\t\t\t\tEnding Shader" << endl;	// Message about debuging
			#endif
			// GLSL
			this->shaderAppearance->myShader->end();
		#endif
	}


	#ifdef DEBUG2
		cout << "\t\t\t\t\tLooking for Appearance" << endl;	// Message about debuging
	#endif
	if(this->appearance!=NULL) { 
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\tFinisghing appearance " << endl;	// Message about debuging
		#endif
		if(this->appearance->imageTexture!=NULL) {
			glDisable(GL_TEXTURE_2D);
		};

		if(this->appearance->cubeMapping!=NULL) {
			glDisable(GL_TEXTURE_CUBE_MAP);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
		}
	
		if(this->appearance->textureTransform!=NULL) {
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		}
		
	}

	// Por causa do quebra galho da transparencia no material
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDisable(GL_LINE_STIPPLE);
	glLineStipple(3,0xFFFF);
	

	
	#ifdef DEBUG2
		cout << "\t\t\t\tReturning (Shape)" << endl;	// Message about debuging
	#endif





	
	
}

Shape *Shape::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Shape *shape;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Shape *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	shape = new Shape();
	shape->read(node);
	
	return shape;
}

void *Shape::read(DOMNode *node) {
	
	
	#ifdef DEBUG1
		cout << "Reading (Shape) " << endl;  
	#endif
	
	unsigned int i; 			//* variable to counter

	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} 
	}

	DOMNodeList *children;			//* variable to hold the node children
	
	children = node->getChildNodes ();
	if (children != NULL) {
		
		for (i = 0; i < children->getLength(); i++) {
			
			#ifdef DEBUG1
				cout << "Looking for: " << XMLString::transcode(children->item(i)->getNodeName()) << endl;  
			#endif
			
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"ShaderAppearance")) {
				this->shaderAppearance = ShaderAppearance::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Appearance")) {
				this->appearance = Appearance::get(children->item(i));
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Box")) { 
				this->geometry[this->countGeometry] = Box::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Sphere")) { 
				this->geometry[this->countGeometry] = Sphere::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Cone")) {
				this->geometry[this->countGeometry] = Cone::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Cylinder")) { 
				this->geometry[this->countGeometry] = Cylinder::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Text")) { 
				this->geometry[this->countGeometry] = Text::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"Extrusion")) { 
				this->geometry[this->countGeometry] = Extrusion::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"IndexedFaceSet")) {
				this->geometry[this->countGeometry] = IndexedFaceSet::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"IndexedLineSet")) {
				this->geometry[this->countGeometry] = IndexedLineSet::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"ElevationGrid")) { 
				this->geometry[this->countGeometry] = ElevationGrid::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"PointSet")) {
				this->geometry[this->countGeometry] = PointSet::get(children->item(i));
				this->countGeometry++;
			} else
			if (!strcmp (XMLString::transcode(children->item(i)->getNodeName()),"NurbsCurve")) {
				this->geometry[this->countGeometry] = NurbsCurve::get(children->item(i));
				this->countGeometry++;
			} else
			if (strcmp (XMLString::transcode(children->item(i)->getNodeName()),"#text")) {
				cout << "Shape not recognized : ";
				cout << (XMLString::transcode(children->item(i)->getNodeName())) << endl;
			}
	
		}
		
	}

	return(NULL);
	
}
