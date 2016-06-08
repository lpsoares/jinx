#include "movie.h"

using namespace std;

#ifdef LINUX

Uploader::Uploader (char *file) {

	mpgfile = fopen (file, "rb"); // open the mpeg file.
	if (!mpgfile) {
		cout << "Could not open file: " << file << endl;
	}
        	
	totalframes = 0;
	
	#ifdef MPEG
		decoder = mpeg2_init (); // initializes the "handler" pointer (of type "mpeg2dec_t") with mpeg2_init(), taking a memory space for decoding.
		if (decoder == NULL) {
			cout << "Could not allocate a decoder object." << endl;
		}
    
		info = mpeg2_info (decoder);
		size = BUFFER_SIZE;
	#endif
		
	do {
		
		#ifdef MPEG
			state = mpeg2_parse (decoder); // mpeg2_parse processes the information available in "decoder". If mpeg2_parse doesn't find anything useful (returns STATE_BUFFER) then this data will be copied into mpeg2 decoder's private data area. This process repeats until something useful is found with mpeg2_parse(ruterns something other than STATE_BUFFER), or the buffer fills up.

			switch (state) {
		
				case STATE_BUFFER:
					size = fread (buffer, 1, BUFFER_SIZE, mpgfile); //  reads "BUFFER_SIZE" elements of data, each "1" byte long, from the stream pointed to by "mpegfile", storing them at the location given by "buffer", returning the number of items successfully read.
					mpeg2_buffer (decoder, buffer, buffer + size); // makes data between "buffer" and "buffer + size" available for processing.
					break;
			
				case STATE_SEQUENCE: // it's time to save all the information about the decoded stream, because the information is avaiable only during the decoding process...
					height = info->sequence->height;
					width = info->sequence->width;
					break;
			
				case STATE_SLICE: // if one of this cases occurs, means that there is a picture to be shown and the counter must be increased.
				case STATE_END:
					if (info->display_fbuf) 
						totalframes++;
					break;
			}
		
		#endif
			
    } while (size); // runs until EOF or an error occurs.
			
	rewind (mpgfile); // rewinds the file to be read again...
}

int Uploader::gWidth()
{
	return width;
}

int Uploader::gHeight()
{
	return height;
}

int Uploader::gNumberFrames()
{
	return totalframes;
}

int Uploader::gSize()
{
	return (3*width*height*totalframes*8); // memory necessary in bits because rgb is declared as uint8_t.
}

//this function takes all the pictures and put it into memory one after other in the adress pointed by "rgb".
void Uploader::UploadFrames()
{

	int j=0;

	rgb = new uint8_t [3*width*height*totalframes]; // allocate the required memory necessary to upload all the pictures.
	if (!rgb)
	{ // should not occur!!!
		cout << "Could not allocate the required memory." << endl;
		exit (-3);
	}
	size = fread (buffer, 1, BUFFER_SIZE, mpgfile);
	
	#ifdef MPEG
		mpeg2_buffer (decoder, buffer, buffer + size);
				
		#ifdef DEBUG
			cout << "# Frames................: "<< gNumberFrames() << endl;
			cout << "# Memoria utilizada (MB): "<< ((double)(gSize()))/8000000 <<endl;
			cout << "P3\n" << gWidth() << " " << gHeight()*gNumberFrames() << "\n255" << endl;
		#endif
	
		do { // process to  upload all the pictures.
			state = mpeg2_parse (decoder);
		
			switch (state) {
			
				case STATE_BUFFER:
					size = fread (buffer, 1, BUFFER_SIZE, mpgfile);
					mpeg2_buffer (decoder, buffer, buffer + size);
					break;
			
				case STATE_SEQUENCE:
					mpeg2_convert (decoder, mpeg2convert_rgb24, NULL);
					break;
			
				case STATE_SLICE:
				case STATE_END:
					if (info->display_fbuf) {
						unsigned int i;
	
						framenum++;
						buf = info->display_fbuf->buf[0];	
						for (i=0; i < (3 * (info->sequence->width * info->sequence->height)) ; i+=3) // for each picture this loop copy the data pointed by "buf" to the adress pointed by "rgb", puting all the picuters one after other in memory.
						{
							rgb[j]   = buf[i]; rgb[j+1] = buf[i+1];	rgb[j+2] = buf[i+2];
							#ifdef DEBUG
								cout << (int) *(rgb+j) << " " << (int) *(rgb+j+1) << " " << (int) *(rgb+j+2) << endl;
							#endif
							j+=3;
						}
					}
				break;
			}
    	} while (size);
	
		mpeg2_close (decoder); //frees the memory utilized by "decoder".
		fclose (mpgfile); // dissociates the named "mpegfile" from its underling file.
		
	#endif
}

uint8_t *Uploader::gFrame(int frameNumber)
{
	uint8_t *frame;
	frame = rgb + ( frameNumber * 3 * height * width );
	return(frame);
}

Uploader::~Uploader()
{
	delete rgb;	
}

#endif
