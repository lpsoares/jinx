
#include "texture.h"

#include "readrgb.h"

void Tex::readRgb(Image *image, FILE *file) {

	/*
		
		An SGI RGB file consists of a header followed by offset data if the image is run length encoded and finally the image data.
Header

The header is structured as shown in the table below.

Size   Type    Name       Description   
 
      2 bytes  short   MAGIC      IRIS image file magic number
                                  This should be decimal 474
      1 byte   char    STORAGE    Storage format
                                  0 for uncompressed
                                  1 for RLE compression
      1 byte   char    BPC        Number of bytes per pixel channel 
                                  Legally 1 or 2
      2 bytes  ushort  DIMENSION  Number of dimensions
                                  Legally 1, 2, or 3
                                  1 means a single row, XSIZE long
                                  2 means a single 2D image
                                  3 means multiple 2D images
      2 bytes  ushort  XSIZE      X size in pixels 
      2 bytes  ushort  YSIZE      Y size in pixels 
      2 bytes  ushort  ZSIZE      Number of channels
                                  1 indicates greyscale
                                  3 indicates RGB
                                  4 indicates RGB and Alpha
      4 bytes  long    PIXMIN     Minimum pixel value
                                  This is the lowest pixel value in the image
      4 bytes  long    PIXMAX     Maximum pixel value
                                  This is the highest pixel value in the image
      4 bytes  char    DUMMY      Ignored
                                  Normally set to 0
     80 bytes  char    IMAGENAME  Image name
                                  Must be null terminated, therefore at most 79 bytes
      4 bytes  long    COLORMAP   Colormap ID
                                  0 - normal mode
                                  1 - dithered, 3 mits for red and green, 2 for blue, obsolete
                                  2 - index colour, obsolete
                                  3 - not an image but a colourmap
    404 bytes  char    DUMMY      Ignored
                                  Should be set to 0, makes the header 512 bytes.

*/
	
		//int rgb_width;
		//int rgb_height;
		int rgb_components;
		
		image->data = (char *) read_texture(file, &image->sizeX, &image->sizeY, &rgb_components);
					
		//image->sizeY=rgb_height;
		//image->sizeX=rgb_width;
	
		if(rgb_components==4) {
			image->alpha = true;
		}
		
		#ifdef DEBUG
			cout << " RGB : size X " << image->sizeX << " size Y " << image->sizeY << endl;
			cout << " RGB : components " << rgb_components << endl;
		#endif
		
	

}
