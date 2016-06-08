
#include <sys/types.h>
#include <sys/stat.h>

#include "texture.h"
#include "../engine.h"
#include "movie.h"
#include "../www.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <strings.h>

/* Constructor */
Tex::Tex() {
	
	#ifdef DEBUG
		cout << " constructor of Tex " << endl;
	#endif
	
	checker = true;
	this->size=20.0;
	this->textfile=0;
	
}

/* Destructor */
Tex::~Tex() {
	
	int f;

	#ifdef DEBUG
		cout << " Destructing tex" << endl << endl;
	#endif
	
	for(f=0;f<this->textfile;f++) {
		glDeleteTextures(1, (GLuint*) &this->texture2[textfile]);	// generate on texture
		//delete[] this->image1[f]->data;
		//delete this->image1[f];
	}
	
}

/* Load Images */
int Tex::ImageLoad(char *filename, Image *image) {

	FILE *file;				// holds the texture

	WWW *www = NULL; 		// to open WWW files
	www = new WWW();		// creating WWW reader
	if(www==NULL) {
		cerr << "Error alocating memory for WWW" << endl;
		return(0);	
	}

	#ifdef DEBUG
		cout << "file : " << filename << " - path " << Engine::getSource() << endl;
	#endif
	
	www->getfile(filename, Engine::getSource());
	// O stat pode retornar os valores de tempo para saber a necessidade do download

	struct stat st;
	char *newname = filename;

	if (stat(www->complete(filename), &st)) {
		newname = strrchr(www->complete(filename), '\\')+1;
		if (!(newname-1) || stat(newname, &st)) {
			cerr << "File not reachable : " << filename << endl;
			return(0);
		}
	}
	
    // make sure the file is there.
	if ((file = fopen(www->complete(filename), "rb"))==NULL) {
		cerr << "Not able to read the file : " << filename;
		if( !(st.st_mode&S_IROTH) ) {
			cout << " - Possible problem, no rights (Others has no permition to read the file)";
		}
		cout << endl;
		return(0);
    }
    
	delete www;					// destroy the www reader
	
	char type[20];
	fread(type,1,18,file); 		// seek through the bmp header, up to the width and height
		
	char stemp[2];              // special routine to identify rbg files
	#ifdef LINUX				// classical little endian and big endian problem
		stemp[1]=type[0];
		stemp[0]=type[1];
	#else
		stemp[0]=type[0];
		stemp[1]=type[1];
	#endif
	
	if(!strncmp(type,"BM6",3)) {	// BMP FILES
	
		readBmp(image,file);
		
	} else if( (!strncmp(type,"P6",2)) || (!strncmp(type,"P3",2))) {    // PPM FILES
		
		readPpm(image,file);
		
	} else if( (*((short int *)stemp)) == 474 ) { // RGB FILES (SGI FORMAT)

		readRgb(image,file);
	
	} else if( (type[0]==((char)0xff)) && (type[1]==((char)0xd8)) ) { // Jpeg

		readJpg(image,file);

	} else {
		
		cerr << "Image format not recognized : " << filename << endl;
		
	}

	
	#ifdef DEBUG3
	cout << " Image Data (" << filename << ") : " << endl;
	#ifdef DEBUG4
	for(int f=0;f<image->sizeY;f++) {
		for(int g=0;g<image->sizeX;g++) {
			if(image->alpha) {
				cout << (unsigned int)((unsigned char)image->data[ (4*((image->sizeX*f)+g))   ]) << ",";
				cout << (unsigned int)((unsigned char)image->data[ (4*((image->sizeX*f)+g))+1 ]) << ",";
				cout << (unsigned int)((unsigned char)image->data[ (4*((image->sizeX*f)+g))+2 ]) << ",";
				cout << (unsigned int)((unsigned char)image->data[ (4*((image->sizeX*f)+g))+3 ]);
			} else {
				cout << (unsigned int)((unsigned char)image->data[ (3*((image->sizeX*f)+g))   ]) << ",";
				cout << (unsigned int)((unsigned char)image->data[ (3*((image->sizeX*f)+g))+1 ]) << ",";
				cout << (unsigned int)((unsigned char)image->data[ (3*((image->sizeX*f)+g))+2 ]);
			}
			cout << " ";
		}
		cout << endl;
	}
	#endif
	#endif
		
	
	
	fclose(file);		// close file descriptor
	
	rescale(image);		// rescale the image if it is not pot2, necessary to opengl
	
    return 1;
	
}


/* Load Movies */
int Tex::MovieLoad(char *filename, Image *image) {
	
	struct stat st;
	char *newname = filename;
	
	WWW *www; // to open WWW files
	www = new WWW();
	
	// get www
	#ifdef DEBUG
		cout << " path " << Engine::my_Engine->source << endl;
	#endif
	
	www->getfile(filename, Engine::getSource());
	
	if (stat(www->complete(filename), &st)) {
		newname = strrchr(www->complete(filename), '\\')+1;
		if (!(newname-1) || stat(newname, &st)) {
			cerr << "Not able to read file : " << filename << endl;
			return 0;
		}
	}
		
	if( true ) { // MPEG FILE
	
		#ifdef LINUX
		Uploader *object = NULL;
		object = new Uploader(www->complete(filename));
		if(object==NULL) {
			cerr << "Error on Mpeg movie" << endl;
		}
		
		object->UploadFrames(); // load frames
	
		image->sizeY=object->gHeight();
		image->sizeX=object->gWidth();
		
		image->data = new char[ image->sizeY * image->sizeX * 3 ];
	
		#ifdef DEBUG		
			cout <<"Altura : " << object->gHeight() << " pixels\n";
			cout <<"Largura: " << object->gWidth()  << " pixels\n";
			cout <<"Frames : " << object->gNumberFrames()  << "\n";
			cout <<"Mem. necessaria p/ frame: " << object->gSize() << " bytes\n";
		#endif	
		
		char *tmp =	(char *) object->gFrame(2);
		
		for(unsigned int f=0; f< image->sizeY*image->sizeX*3 ; f++) {
			image->data[f] = tmp[f]; 
		}
		
		delete object;
		#endif
		
	} 

	delete www;	// destroy the www reader
	
	rescale(image);

    return 1;
	
}
    





/*   aaa  */
void Tex::LoadGLMovieTextures(char* name, int frameNumber) {	
        
    // allocate space for texture
	this->image1[this->textfile] = new Image;
    if (this->image1[this->textfile] == NULL) {
		cerr << "Error allocating space for image" << endl;
    }
	
	this->image1[this->textfile]->alpha = false;
	
    if (!MovieLoad(name, image1[this->textfile])) {
		cerr << "Error on texture" << endl;
    }        

    // Create Texture	
    glGenTextures(1, (GLuint*) &this->texture2[textfile]);	// generate on texture, just for allocation
    glBindTexture(GL_TEXTURE_2D, this->texture2[textfile]);   // 2d texture (x and y size)

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// way that the bytes are organized
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// adjuste the texture for larger areas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// adjuste the texture for larger areas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// adjuste the texture for larger areas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //specifies how texture values are interpreted when a fragment is textured.
	
	if(!image1[textfile]->alpha) {
    	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				 GL_RGB, GL_UNSIGNED_BYTE, image1[textfile]->data);
	} else {
		// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    	glTexImage2D(GL_TEXTURE_2D, 0, 4, image1[textfile]->sizeX, image1[textfile]->sizeY, 0, 
				 GL_RGBA, GL_UNSIGNED_BYTE, image1[textfile]->data);
	}
}


void Tex::readMovieTexture(char* name, int frameNumber) {
	
	bool find=false;
	
	for(int f=0;f<textfile;f++) {
		if(!strcmp(name,this->textname[f])) {
			glBindTexture(GL_TEXTURE_2D, this->texture2[f]);
			find=true;
		} 
	}
	
	if(!find) {
		strcpy(this->textname[this->textfile],name);
		LoadGLMovieTextures(name,frameNumber);
		this->textfile++;
	} 
	
}


void Tex::priority() {
	//void glPrioritizeTextures( GLsizei n,
    //                              const GLuint *textures,
    //                              const GLclampf *priorities )

	//glPrioritizeTextures();
}
