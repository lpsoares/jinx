#ifndef UPLOADER_H
#define UPLOADER_H


#include <iostream>
#include <inttypes.h>
#include <stdio.h>

#ifdef MPEG
	#include "mpeg2dec/mpeg2.h"
	#include "mpeg2dec/mpeg2convert.h"
#endif

#define BUFFER_SIZE 4096

//#define DEBUG // uilized only for debuging. Show the stream in a format tha could be opened wiht a graphical program.

#ifdef LINUX
class Uploader {
	
	FILE * mpgfile; 
	
	uint8_t buffer [BUFFER_SIZE]; // space reserved to read the information of the stream.
	
	#ifdef MPEG
		const mpeg2_info_t * info; // a struct pointer wich will keep all the information about the stream, only during the decoding process.
		mpeg2dec_t * decoder; // a struct pointer wich will keep all the data of the stream.
	#endif
	
	size_t size; // necessary to continue the decoding.
	int state; // necessary to show in wich process the decoder is...
	
    int framenum;
	int totalframes;
	int height;
	int width;
	
	uint8_t * rgb; // pointer to upload all the pictures.
	uint8_t * buf; // pointer to upload each the pictures.
	
public:
	
	Uploader (char *file); // constructor that open the stream mpeg and count how many pictures it has.
	~Uploader (); // destructor freeing up the memory utilized by the rgb.
	
// functions created to return values like: "width", "height", "number of frames" and "memory size required to upload all the pictures".	
	int gWidth();
	int gHeight();
	int gNumberFrames();
	int gSize();
	
	void UploadFrames(); // function that put all the frames into memory...
	uint8_t *gFrame(int frameNumber); // funtion that returns the initail adress of the conponents (in rgb) of each picture.
};
#else

class Uploader {

    int framenum;
	int totalframes;
	int height;
	int width;


};

#endif

#endif
