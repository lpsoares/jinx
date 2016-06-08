#ifndef __texture_h
#define __texture_h

#include <iostream>

using namespace std;

/* Image type - contains height, width, and data */
class Image {
	
	public:
	
	Image();
	~Image();
	
    unsigned int sizeX;
    unsigned int sizeY;
	
    char *data;
	bool alpha;
	
};

/* controls the textures */
class Tex {
	
	public:
				
	Image *image1[256];
	
	Image *image2[256];	// for cube mapping
	Image *image3[256];	// for cube mapping
	Image *image4[256];	// for cube mapping
	Image *image5[256];	// for cube mapping
	Image *image6[256];	// for cube mapping
	
	Tex();	// Constructor
	~Tex();	// Destructor

	void readBmp(Image *image, FILE *file);	// function that reads BMP files
	void readPpm(Image *image, FILE *file);	// function that reads PPM files
	void readRgb(Image *image, FILE *file);	// function that reads RGB files
	void readJpg(Image *image, FILE *file);	// function that reads JPG files

	void rescale(Image *image);				// rescale images to pot2
	
	int ImageLoad(char *filename, Image *image);
	int MovieLoad(char *filename, Image *image);
	
	void LoadGLTextures(char* name, bool repeatS, bool repeatT);
	void LoadGLMovieTextures(char* name,int frameNumber);
	
	void readTexture(char* name, bool repeatS, bool repeatT);
	void readMovieTexture(char* name, int frameNumber);
		
	void readCubeMap(char name[6][256], bool repeatS, bool repeatT);
	void LoadCubeMapTextures(char name[6][256], bool repeatS, bool repeatT);
		
	void gridPlane();	// draw a grid on the origin



	//private:

	void priority();  // function to control the priority of each texture, (( NOT IMPLEMENTED ))

	bool checker;
	float size;	// define the grid size at floort
	
	int  textfile; 				// counter for textures
	char textname[256][256];	// hold the name of each texture
	int  texture2[256];			// id of texture, used for opengl
	
};

#endif
