

#include "texture.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void Tex::readPpm(Image *image, FILE *file) {

	#ifdef DEBUG
		cout << "Reading a PPM file" << endl;
	#endif
	
	//struct stat st;
	
	char line[256];
	int maxval;			// maximum value of a pixel
	unsigned int i_ppm;
	unsigned int int_tmp;
	
	bool binary=true;
	
	//int w,h;
		
	//size = (int)sqrt((float)st.st_size / 3);

	rewind(file);
		
	fgets(line, 256, file);
	
	/*
	if (strncmp(line, "P6", 2)) {
		
		if (strncmp(line, "P3", 2)) {
			cerr << "ppm read: not a ppm file" << endl;
		}
		
		else {
			binary = false;
		}
		
	}*/
	
	if (!strncmp(line, "P3", 2)) {
		binary = false;
	}
	
	fgets(line, 256, file);
	while (line[0] == '#')
		fgets(line, 256, file);
	
	sscanf(line, "%d %d", &image->sizeY, &image->sizeX); // read width and height
		
	//image->sizeX=h;
	//image->sizeY=w;
		
	#ifdef DEBUG
		cout << "Width : " << image->sizeY << " Height : " << image->sizeX << endl; 
	#endif
		
	fgets(line, 256, file);
	sscanf(line, "%d", &maxval);
		
	image->data = new char[(image->sizeY) * (image->sizeX) * 3];
			
	if (!(image->data)) {
		cerr << "Some fatal error" << endl;
	}
		
	if (binary) {
			
		#ifdef DEBUG
			cout << " Binary Image " << endl;
		#endif
			
		fread(image->data, 3 * (image->sizeY) * (image->sizeX), 1, file);
		
	} else {
			
		#ifdef DEBUG
			cout << " ASCII Image " << endl;
		#endif
			
		for (i_ppm = 0; i_ppm < (image->sizeY) * (image->sizeX) * 3; i_ppm++) {
			fscanf(file, "%d", &int_tmp);
			image->data[i_ppm] = int_tmp;
		}
	}
	
}
