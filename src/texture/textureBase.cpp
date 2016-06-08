
#include <iostream>

#include "texture.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>


void Tex::readTexture(char* name, bool repeatS, bool repeatT) {

	#ifdef DEBUG2
		cout << "\t\t\t\t\tStarting to get texture" << endl;
	#endif
		
	bool find=false;
	
	#ifdef DEBUG2
		cout << " Number of textures : " << this->textfile << endl;
		cout << " address " << this << endl;
	#endif
	
	for(int f=0;f<textfile;f++) 
	{
		if(!strcmp(name,this->textname[f])) {
			glBindTexture(GL_TEXTURE_2D, this->texture2[f]);
			find=true;
		} 
	}
	
	if(!find) 
	{
		#ifdef DEBUG2
			cout << " Reading a new image : " << name << "  >  " << this->textfile << endl;
		#endif
		strcpy(this->textname[this->textfile],name);
		LoadGLTextures(name,repeatS,repeatT);
		
		#ifdef DEBUG2
			cout << " Adjusting the number of textures" << endl;
		#endif
		this->textfile++;
	
	} 
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\tfinishing to get texture" << endl;
	#endif
	
}




void Tex::readCubeMap(char name[6][256], bool repeatS, bool repeatT) {

	#ifdef DEBUG2
		cout << "\t\t\t\t\tStarting to get texture" << endl;
	#endif
		
	bool find=false;
	
	#ifdef DEBUG2
		cout << " Number of textures : " << this->textfile << endl;
		cout << " address " << this << endl;
	#endif
	
	for(int f=0;f<textfile;f++) 
	{
		if(!strcmp(name[0],this->textname[f])) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture2[f]);
			find=true;
		} 
	}
	
	if(!find) 
	{
		#ifdef DEBUG2
			cout << " Reading a new image : " << name << "  >  " << this->textfile << endl;
		#endif
		strcpy(this->textname[this->textfile],name[0]);
		LoadCubeMapTextures(name,repeatS,repeatT);
		
		#ifdef DEBUG2
			cout << " Adjusting the number of textures" << endl;
		#endif
		this->textfile++;
	
	} 
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\tfinishing to get texture" << endl;
	#endif
	
}








// Load Bitmaps And Convert To Textures
void Tex::LoadCubeMapTextures(char name[6][256], bool repeatS, bool repeatT) {
        
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\tLoading a OpenGL Texture" << endl;
	#endif
	
    // allocate space for texture
	this->image1[this->textfile] = new Image;
    if (this->image1[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image1[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[0], image1[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        

	this->image2[this->textfile] = new Image;
    if (this->image2[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image2[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[1], image2[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }
	
	this->image3[this->textfile] = new Image;
    if (this->image3[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image3[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[2], image3[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        
	
	this->image4[this->textfile] = new Image;
    if (this->image4[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image4[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[3], image4[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        
	
	this->image5[this->textfile] = new Image;
    if (this->image5[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image5[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[4], image5[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        
	
	this->image6[this->textfile] = new Image;
    if (this->image6[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	this->image6[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	if (!ImageLoad(name[5], image6[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        
	
	
	
	
	
	
	
	
	
	int maxT;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxT);
	/*
	if( (image1[textfile]->sizeX>(unsigned int)maxT) || (image1[textfile]->sizeY>(unsigned int)maxT) ) {
		cerr << "Error texture " << name << "has a big size that supported by the system (" << maxT << ")" << endl;
		return;
	}
	*/
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\t\tpreparing OpenGL texture commands" << endl;
	#endif
	
	// Create Texture	
    glGenTextures(1, (GLuint*) &this->texture2[textfile]);	// generate on texture, just for allocation
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture2[textfile]);   // 2d texture (x and y size)

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// way that the bytes are organized

	if(repeatS) {
		#ifdef DEBUG2
			cout << "Repeating Texture in S" << endl;
	
		#endif
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);	// adjuste the texture for larger areas
	} else {
		#ifdef DEBUG2
			cout << "Not repeating Texture in S" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);// adjuste the texture for larger areas
	}

	if(repeatT) {
		#ifdef DEBUG2
			cout << "Repeating Texture in T" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);	// adjuste the texture for larger areas
	} else {
		#ifdef DEBUG2
			cout << "Not epeating Texture in T" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);// adjuste the texture for larger areas
	}

	
	
	
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //specifies how texture values are interpreted when a fragment is textured.
	
	
	
	// ESTA FUNCAO DEVE SER ESTUDADA PARA ENTENDER PROBLEMAS DE TEXTURIZACAO E COR ^^^
	
	if(!image1[textfile]->alpha) {
    	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
		
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\t\tPutting a texture without alpha in OpenGL texture memory" << endl;
		#endif
		
		if(true) // this is just a teste to know what happens (asa delta precisa)
		{
			// QUANDO LIGAR O MIPMAP
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// adjuste the texture for larger areas
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// adjuste the texture for larger areas
	
			
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 3, image5[textfile]->sizeX, image5[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image5[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 3, image6[textfile]->sizeX, image6[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image6[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 3, image4[textfile]->sizeX, image4[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image4[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 3, image3[textfile]->sizeX, image3[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image3[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 3, image2[textfile]->sizeX, image2[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image2[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 3, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data);
			
			
			
			#ifdef DEBUG2
				cout << "Initializing Mipmap : " << textfile << endl;
			#endif 
		/*
			if(gluBuild2DMipmaps( GL_TEXTURE_2D, 3, image1[textfile]->sizeX, image1[textfile]->sizeY,
        			GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data )) {
				cerr << "Error with Mipmap" << endl;
			}
			*/
			#ifdef DEBUG
				cout << "Error : " << error << endl;
			#endif
		} 
		else 
		{	
	
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 3, image6[textfile]->sizeX, image6[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image6[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 3, image5[textfile]->sizeX, image5[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image5[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 3, image3[textfile]->sizeX, image3[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image3[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 3, image4[textfile]->sizeX, image4[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image4[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 3, image2[textfile]->sizeX, image2[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image2[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 3, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data);

		}
		
	} 
	else 
	{
	
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\t\tPutting a texture with alpha in OpenGL texture memory" << endl;
		#endif
		
		if(image1[textfile]->sizeX <= 1024) // this is just a teste to know what happens (asa delta precisa)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);// adjuste the texture for larger areas
	
    		
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 4, image6[textfile]->sizeX, image6[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image6[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 4, image5[textfile]->sizeX, image5[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image5[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 4, image3[textfile]->sizeX, image3[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image3[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 4, image4[textfile]->sizeX, image4[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image4[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 4, image2[textfile]->sizeX, image2[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image2[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 4, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data);
			#ifdef DEBUG2
				cout << "Initializing Mipmap : " << textfile << endl;
			#endif 
		/*
			if(gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image1[textfile]->sizeX, image1[textfile]->sizeY,
        			GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data )) {
				cerr << "Error with Mipmap" << endl;
			}
		*/
			
			#ifdef DEBUG
				cout << "Error : " << error << endl;
			#endif
		} 
		else 
		{	
	
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas
	
    		
    		
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 4, image6[textfile]->sizeX, image6[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image6[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 4, image5[textfile]->sizeX, image5[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image5[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 4, image3[textfile]->sizeX, image3[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image3[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 4, image4[textfile]->sizeX, image4[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image4[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 4, image2[textfile]->sizeX, image2[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image2[textfile]->data);
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 4, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data);
			
			
		}
		
	}
	
	#ifdef DEBUG2
		cout << "Texture [" << textfile << "] : " << image1[textfile]->sizeX*image1[textfile]->sizeY*(image1[textfile]->alpha?4:3) << endl;
	#endif
	
	
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_NORMALIZE);
	
	
	// The texture is alredy on the graphic board memory it is not more necessary
	//delete this->image1[this->textfile];
	// Do not delete, the program become instable
	// Dangerous...
	
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\tLoading a OpenGL Texture finished" << endl;
	#endif
	
}







// Load Bitmaps And Convert To Textures
void Tex::LoadGLTextures(char* name, bool repeatS, bool repeatT) {	
        
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\tLoading a OpenGL Texture" << endl;
	#endif
	
    // allocate space for texture
	this->image1[this->textfile] = new Image;
    if (this->image1[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	
	this->image1[this->textfile]->alpha = false;	// usualy image has no alpha (transparecy)
	
    if (!ImageLoad(name, image1[textfile])) {
		cerr << "Error on texture : " << name << endl;
		return;
    }        

	int maxT;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxT);
	
	if( (image1[textfile]->sizeX>(unsigned int)maxT) || (image1[textfile]->sizeY>(unsigned int)maxT) ) {
		cerr << "Error texture " << name << "has a big size that supported by the system (" << maxT << ")" << endl;
		return;
	}
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\t\tpreparing OpenGL texture commands" << endl;
	#endif
	// Create Texture	
    glGenTextures(1, (GLuint*) &this->texture2[textfile]);	// generate on texture, just for allocation
    glBindTexture(GL_TEXTURE_2D, this->texture2[textfile]);   // 2d texture (x and y size)

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// way that the bytes are organized

	if(repeatS) {
		#ifdef DEBUG2
			cout << "Repeating Texture in S" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// adjuste the texture for larger areas
	} else {
		#ifdef DEBUG2
			cout << "Not repeating Texture in S" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// adjuste the texture for larger areas
	}

	if(repeatT) {
		#ifdef DEBUG2
			cout << "Repeating Texture in T" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// adjuste the texture for larger areas
	} else {
		#ifdef DEBUG2
			cout << "Not epeating Texture in T" << endl;
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);// adjuste the texture for larger areas
	}

	
	// select modulate to mix texture with color for shading
	//specifies how texture values are interpreted when a fragment is textured.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 	// not used
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);		// not used
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);		// not used
	
	
	
	// NOT USED YET
	//float TextureColor[4] = { 1.0f , 1.0f , 1.0f , 0.0f };
	//glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, TextureColor);
	
	
		
	if(!image1[textfile]->alpha) {
    	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
		
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\t\tPutting a texture without alpha in OpenGL texture memory" << endl;
		#endif
		
		if(true) // this is just a teste to know what happens (asa delta precisa)
		{

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// adjuste the texture for larger areas
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// adjuste the texture for larger areas
	
			glTexImage2D(GL_TEXTURE_2D, 0, 3, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data);
		
			#ifdef DEBUG2
				cout << "Initializing Mipmap : " << textfile << endl;
			#endif 
		
			if(gluBuild2DMipmaps( GL_TEXTURE_2D, 3, image1[textfile]->sizeX, image1[textfile]->sizeY,
        			GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data )) {
				cerr << "Error with Mipmap" << endl;
			}
			
			#ifdef DEBUG
				cout << "Error : " << error << endl;
			#endif
		} 
		else 
		{	
	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas
			glTexImage2D(GL_TEXTURE_2D, 0, 3, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data);
		}
		
	} 
	else 
	{
	
		#ifdef DEBUG2
			cout << "\t\t\t\t\t\t\tPutting a texture with alpha in OpenGL texture memory" << endl;
		#endif
		
		if(image1[textfile]->sizeX <= 1024) // this is just a teste to know what happens (asa delta precisa)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);// adjuste the texture for larger areas
	
    		glTexImage2D(GL_TEXTURE_2D, 0, 4, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data);
		
			#ifdef DEBUG2
				cout << "Initializing Mipmap : " << textfile << endl;
			#endif 
		
			if(gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image1[textfile]->sizeX, image1[textfile]->sizeY,
        			GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data )) {
				cerr << "Error with Mipmap" << endl;
			}
		
			
			#ifdef DEBUG
				cout << "Error : " << error << endl;
			#endif
		} 
		else 
		{	
	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas
	
    		glTexImage2D(GL_TEXTURE_2D, 0, 4, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data);
			
		}
		
	}
	
	#ifdef DEBUG2
		cout << "Texture [" << textfile << "] : " << image1[textfile]->sizeX*image1[textfile]->sizeY*(image1[textfile]->alpha?4:3) << endl;
	#endif
	
	
	// The texture is alredy on the graphic board memory it is not more necessary
	//delete this->image1[this->textfile];
	// Do not delete, the program become instable
	// Dangerous...
	
	
	
	
	#ifdef DEBUG2
		cout << "\t\t\t\t\t\tLoading a OpenGL Texture finished" << endl;
	#endif
	
}
