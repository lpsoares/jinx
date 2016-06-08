
#include "texture.h"

#include <jpeglib.h>

#ifdef LINUX


typedef void (*JpegErrorExit)(j_common_ptr);

void my_error_exit(j_common_ptr cinfo) {

	cout << "Some error in Jpeg descompression" << endl;
	
	// cinfo->err really points to a my_error_mgr struct, so coerce pointer 
	//my_error_ptr myerr = (my_error_ptr) cinfo->err;

	// Always display the message. 
	// We could postpone this until after returning, if we chose. 
	(*cinfo->err->output_message) (cinfo);

	// Return control to the setjmp point 
	//longjmp(myerr->setjmp_buffer, 1);
	
}

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	//jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

#endif



void Tex::readJpg(Image *image, FILE *file) {

	#ifdef DEBUG1
		cout << "\t\t\t\t\t\t\tReading a Jpeg image " << endl;
	#endif
	
	#ifdef LINUX
		
	rewind(file);
		
	struct jpeg_decompress_struct cinfo;
		
	struct my_error_mgr jerr;
		
	int bytes_per_line, i, row;
		
	#define BUFFER_HEIGHT 5

	unsigned char *pixels=NULL;
	unsigned char *buffer[BUFFER_HEIGHT];

    memset(&cinfo, 0, sizeof(cinfo)); //Initialize the JPEG decompression object with default error handling.

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = reinterpret_cast<JpegErrorExit>(my_error_exit);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file); // Specify data source for decompression
      
	if(JPEG_SUSPENDED==jpeg_read_header(&cinfo, TRUE)) { // Read file header, set default decompression parameters
		cerr << "Problems with Jpeg header" << endl;
	}

	#ifdef DEBUG3
	
	cout << " jpeg : " << endl;
	cout << " size " << cinfo.image_width << " , " << cinfo.image_height;
	cout << " number of components " << cinfo.num_components << endl;
	cout << " Color Space " << cinfo.jpeg_color_space << endl;
	
	cout << " Out Color Space" << cinfo.out_color_space; /* colorspace for output */
	cout << "Scale number " << cinfo.scale_num << " " << cinfo.scale_denom; /* fraction by which to scale image */
	cout << "Out Gama " << cinfo.output_gamma;          /* image gamma wanted in output */
	cout << "Buffered Image " << cinfo.buffered_image;       /* TRUE=multiple output passes */
	cout << "Raw data " << cinfo.raw_data_out;         /* TRUE=downsampled data wanted */
	cout << "DCT " << cinfo.dct_method;      /* IDCT algorithm selector */
	cout << "Fancy " << cinfo.do_fancy_upsampling;  /* TRUE=apply fancy upsampling */
	cout << "Block " << cinfo.do_block_smoothing;   /* TRUE=apply interblock smoothing */
	cout << "Quantize " << cinfo.quantize_colors;      /* TRUE=colormapped output wanted */

	cout << endl;

	cout << "Data precision " << cinfo.data_precision;
	cout << "saw_JFIF_marker" << cinfo.saw_JFIF_marker;      /* TRUE iff a JFIF APP0 marker was found */
  
  	if(cinfo.saw_JFIF_marker) {
  
		cout << "JFIF_major_version" << cinfo.JFIF_major_version;     /* JFIF version number */
		cout << "JFIF_minor_version" << cinfo.JFIF_minor_version;
		cout << "density_unit" << cinfo.density_unit;           /* JFIF code for pixel size units */
		cout << "X_density" << cinfo.X_density;             /* Horizontal pixel density */
		cout << "Y_density" << cinfo.Y_density;             /* Vertical pixel density */
		cout << "saw_Adobe_marker" << cinfo.saw_Adobe_marker;     /* TRUE iff an Adobe APP14 marker was found */
		cout << "Adobe_transform" << cinfo.Adobe_transform;        /* Color transform code from Adobe marker */
	
	}
	
  	cout << "CCIR601 " << cinfo.CCIR601_sampling;     /* TRUE=first samples are cosited */

	cout << "max_h_samp_factor" << cinfo.max_h_samp_factor;        /* largest h_samp_factor */
	cout << "max_v_samp_factor" << cinfo.max_v_samp_factor;        /* largest v_samp_factor */
	cout << "min_DCT_scaled_size" << cinfo.min_DCT_scaled_size;      /* smallest DCT_scaled_size of any component */
	cout << "total_iMCU_rows" << cinfo.total_iMCU_rows; 
	
	cout << endl << endl;

	#endif
	
	if(cinfo.saw_Adobe_marker) {
		cout << "Warning there is a image could contain certain parameters that the reader is not able to read" << endl;
	}
		
	(void) jpeg_start_decompress(&cinfo); // Start decompressor

	bytes_per_line = cinfo.num_components * cinfo.image_width; // Allocate image buffer
	pixels = new unsigned char[(bytes_per_line  * cinfo.image_height)];
		
	if (!pixels) {
		cerr << "No pixels found in Jpeg file" << endl;
	}

	row = 0;
	  
	while (cinfo.output_scanline < cinfo.output_height) { // Process data
		for (i=0; i<BUFFER_HEIGHT; ++i) {
			// read in upside down because opengl says the
			// texture origin is lower left
			int rrow = cinfo.output_height - row - 1;
			buffer[i] = &pixels[bytes_per_line * (rrow - i)];
		}
		row += jpeg_read_scanlines(&cinfo, buffer, BUFFER_HEIGHT);
   	}

	image->data = (char *) buffer[1];
	  
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
	  
	image->sizeX = cinfo.image_width; 	// reading the width
	image->sizeY = cinfo.image_height; 	// reading the width
	//  nc = cinfo.num_components;
	  
	#endif
	
	#ifdef DEBUG1
		cout << "\t\t\t\t\t\t\tJpeg image reading concluded" << endl;
	#endif
	
}
