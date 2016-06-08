
#include "texture.h"


static unsigned int getint(FILE *fp) {
	int c, c1, c2, c3;
	/* get 4 bytes*/ 
	c = getc(fp);
	c1 = getc(fp);
	c2 = getc(fp);
	c3 = getc(fp);
return ((unsigned int) c) +
(((unsigned int) c1) << 8) +
(((unsigned int) c2) << 16) +
(((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp) {
  int c, c1;
  /*get 2 bytes*/
  c = getc(fp);
  c1 = getc(fp);
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

void Tex::readBmp(Image *image, FILE *file) {
	
	// para bmp
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.
	
	image->sizeX = getint(file); 	// reading the width

	//if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
	//	cout << "Error reading width from " << filename << endl;
	//	return 0;
	//}
		
	#ifdef DEBUG 
		cout << "Width of " << filename << " : " << image->sizeX << endl;
	#endif
    
	image->sizeY = getint(file); // read the height 
	// ---------- TERMINAR -------------- //
	//cout << "size" << image->sizeY << endl;
	//cout << "sqrt" << ( log10(image->sizeY)/log10(2) ) << endl;
		
	//if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
	//printf("Error reading height from %s.\n", filename);
	//return 0;
	//}
	
	#ifdef DEBUG
	cout << "Height of " << filename << " : " << image->sizeY << endl;
	#endif
    
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;

	// read the planes
	planes = getshort(file);
	//   if ((fread(&planes, 2, 1, file)) != 1) {	
	//	printf("Error reading planes from %s.\n", filename);
	//	return 0;
	//   }
	if (planes != 1) {
		cout << "Planes from " << " is not " << planes << endl; ;
		//return 0;
	}

	// read the bpp
	bpp = getshort(file);
		
	//if ((i = fread(&bpp, 2, 1, file)) != 1) {
	//printf("Error reading bpp from %s.\n", filename);
	//return 0;
	//}
		
	if (bpp != 24) {
		cerr << "Bpp from " << " is not 24: " << bpp << endl;
		//return 0;
	}

	fseek(file, 24, SEEK_CUR); // seek the rest of the bitmap header

	image->data = new char[size];
	if (image->data == NULL) {
		cerr << "Error allocating memory for color-corrected image data" << endl;
		//return 0;	
	}

	if ((i = fread(image->data, size, 1, file)) != 1) { // read the data
		cerr << "Error reading image data from " << endl;
		//return 0;
	}

	for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
	}
	
	
}
