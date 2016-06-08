/* To do */

#include "SocketException.h"
#include "camera.h"

// Includes de C e C++
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>

// Includes do OpenGL
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>



#ifdef LINUX
typedef enum
{
  BAYER_PATTERN_BGGR,
  BAYER_PATTERN_GRBG,
  BAYER_PATTERN_RGGB,
  BAYER_PATTERN_GBRG
} bayer_pattern_t;

void BayerNearestNeighbor(unsigned char *src, unsigned char *dest, int sx, int sy, bayer_pattern_t type)
{
  unsigned char *outR, *outG, *outB;
  register int i,j;

  // sx and sy should be even
  switch (type)
    {
    case BAYER_PATTERN_GRBG:
    case BAYER_PATTERN_BGGR:
      outR=&dest[0];
      outG=&dest[1];
      outB=&dest[2];
      break;
    case BAYER_PATTERN_GBRG:
    case BAYER_PATTERN_RGGB:
      outR=&dest[2];
      outG=&dest[1];
      outB=&dest[0];
      break;
    default:
      outR=NULL;outG=NULL;outB=NULL;
      break;
    }

  switch (type)
    {
    case BAYER_PATTERN_GRBG: //-------------------------------------------
    case BAYER_PATTERN_GBRG:
      // copy original RGB data to output images
      for (i=0;i<sy;i+=2)
	for (j=0;j<sx;j+=2)
	  {
	    outG[(i*sx+j)*3]=src[i*sx+j];
	    outG[((i+1)*sx+(j+1))*3]=src[(i+1)*sx+(j+1)];
	    outR[(i*sx+j+1)*3]=src[i*sx+j+1];
	    outB[((i+1)*sx+j)*3]=src[(i+1)*sx+j];
	  }
      
      // R channel
      for (i=0;i<sy;i+=2)
	for (j=0;j<sx-1;j+=2)
	  {
	    outR[(i*sx+j)*3]=outR[(i*sx+j+1)*3];
	    outR[((i+1)*sx+j+1)*3]=outR[(i*sx+j+1)*3];
	    outR[((i+1)*sx+j)*3]=outR[(i*sx+j+1)*3];
	  }
      
      // B channel
      for (i=0;i<sy-1;i+=2)//every two lines
	for (j=0;j<sx-1;j+=2)
	  {
	    outB[(i*sx+j)*3]=outB[((i+1)*sx+j)*3];
	    outB[(i*sx+j+1)*3]=outB[((i+1)*sx+j)*3];
	    outB[((i+1)*sx+j+1)*3]=outB[((i+1)*sx+j)*3];
	  }
      
      // using lower direction for G channel
      
      // G channel
      for (i=0;i<sy-1;i+=2)//every two lines
	for (j=1;j<sx;j+=2)
	  outG[(i*sx+j)*3]=outG[((i+1)*sx+j)*3];
      
      for (i=1;i<sy-2;i+=2)//every two lines
	for (j=0;j<sx-1;j+=2)
	  outG[(i*sx+j)*3]=outG[((i+1)*sx+j)*3];
      
      // copy it for the next line
      for (j=0;j<sx-1;j+=2)
	outG[((sy-1)*sx+j)*3]=outG[((sy-2)*sx+j)*3];
      
      break;
    case BAYER_PATTERN_BGGR: //-------------------------------------------
    case BAYER_PATTERN_RGGB:
      // copy original data
      for (i=0;i<sy;i+=2)
	for (j=0;j<sx;j+=2)
	  {
	    outB[(i*sx+j)*3]=src[i*sx+j];
	    outR[((i+1)*sx+(j+1))*3]=src[(i+1)*sx+(j+1)];
	    outG[(i*sx+j+1)*3]=src[i*sx+j+1];
	    outG[((i+1)*sx+j)*3]=src[(i+1)*sx+j];
	  }

      // R channel
      for (i=0;i<sy;i+=2)
	for (j=0;j<sx-1;j+=2)
	  {
	    outR[(i*sx+j)*3]=outR[((i+1)*sx+j+1)*3];
	    outR[(i*sx+j+1)*3]=outR[((i+1)*sx+j+1)*3];
	    outR[((i+1)*sx+j)*3]=outR[((i+1)*sx+j+1)*3];
	  }
      
      // B channel
      for (i=0;i<sy-1;i+=2)//every two lines
	for (j=0;j<sx-1;j+=2)
	  {
	    outB[((i+1)*sx+j)*3]=outB[(i*sx+j)*3];
	    outB[(i*sx+j+1)*3]=outB[(i*sx+j)*3];
	    outB[((i+1)*sx+j+1)*3]=outB[(i*sx+j)*3];
	  }

      // using lower direction for G channel
      
      // G channel
      for (i=0;i<sy-1;i+=2)//every two lines
	for (j=0;j<sx-1;j+=2)
	  outG[(i*sx+j)*3]=outG[((i+1)*sx+j)*3];
      
      for (i=1;i<sy-2;i+=2)//every two lines
	for (j=0;j<sx-1;j+=2)
	  outG[(i*sx+j+1)*3]=outG[((i+1)*sx+j+1)*3];
      
      // copy it for the next line
      for (j=0;j<sx-1;j+=2)
	outG[((sy-1)*sx+j+1)*3]=outG[((sy-2)*sx+j+1)*3];

      break;

    default:  //-------------------------------------------
      fprintf(stderr,"Bad Bayer pattern ID\n");
      break;
    }
}

#endif

Camera::Camera(Tex *ttex) {
#ifdef LINUX
	this->tex = ttex;

	try {
		client_socket = new ClientSocket ("10.0.200.71",30000); // Start FrameReceiver Class
	} catch (SocketException& e) {
		cerr << "Was not possible to read the camera" << endl ;
        cerr << "Exception message is: " << endl ;
        cerr << e.description() << endl;
		client_socket = NULL;
    }

	//buffer = (unsigned char*)malloc(sizeof(unsigned char)*3); // Allocate memory for image buffer
	//imagebuffer = (unsigned char*)malloc(640*480*2*sizeof(int));
	//if(imagebuffer == NULL) perror("Erro alocando memoria para imagebuffer\n");
	//memset(imagebuffer,128,640*480*2);
		
	
  int WindowHeight = 480;
  int WindowWidth  = 640;
	
  imagebuffer = (unsigned char*)malloc(WindowWidth*WindowHeight);
  imagebuffer2 = (unsigned char*)malloc(WindowWidth*WindowHeight);
  imagebufferRGB1 = (unsigned char*)malloc(WindowWidth*WindowHeight*3);
  imagebufferRGB2 = (unsigned char*)malloc(WindowWidth*WindowHeight*3);
  memset(imagebuffer,128,WindowWidth*WindowHeight);
  memset(imagebuffer2,128,WindowWidth*WindowHeight);
  memset(imagebufferRGB1,128,WindowWidth*WindowHeight*3);
  memset(imagebufferRGB2,128,WindowWidth*WindowHeight*3);

	#ifdef EZJ
  		//EZJ Allocation Memory
  		src = JPEG_BUFFER_alloc();
  		dest = BMP_BUFFER_alloc();
	#endif
	
	
#endif
}

Camera::~Camera() {
}

void Camera::draw(int eye) {
#ifdef LINUX

	//#ifdef DEBUG
	cout << " Drawing camera image " << endl;
	//#endif

	if(client_socket!=NULL) {

  
  int sizeimage;
  int size;

		cout << " reading image " << endl;

		
  int teste;
	teste = client_socket->recvdata((int*)&size, sizeof(int));
  sizeimage = client_socket->recvdata((int*)imagebuffer,size);
  
		cout << " reading image1 " << endl;


	#ifdef EZJ		
	
EZJ_STATUS my_status;
		
  JPEG_BUFFER_init (src, sizeimage);
  
		
  	memcpy(src->data, imagebuffer, sizeimage);
  	// Convert to PPM
  	my_status = JPEGToPPM_mm (src, dest); //BufferPPM To BufferJPEG
 		
		cout << " reading image2 " << endl;
		
 	 memcpy(imagebuffer,dest->data+16,dest->total_size);
 	 memcpy(imagebuffer2,dest->data+16+640*480,(dest->total_size-54)/2);
   
		cout << " converting image 3" << endl;

		
  	BayerNearestNeighbor(imagebuffer, imagebufferRGB1, 640, 480, BAYER_PATTERN_RGGB);
  	BayerNearestNeighbor(imagebuffer2, imagebufferRGB2, 640, 480,BAYER_PATTERN_RGGB);

  	JPEG_BUFFER_deinit ( src );
  	BMP_BUFFER_deinit ( dest );
  
  #endif
  
  		cout << " opengl image " << endl;

		//if(eye==0) {
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_2D);
		
		glGenTextures(1,(GLuint*) &id);	// generate on texture, just for allocation
		glBindTexture(GL_TEXTURE_2D,id);   // 2d texture (x and y size)

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// way that the bytes are organized
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// adjuste the texture for larger areas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// adjuste the texture for larger areas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// adjuste the texture for larger areas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// adjuste the texture for larger areas

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //specifies how texture values are interpreted when a fragment is textured.
		
		cout << " conversão maluca image " << endl;

		
		char *data;
	
		data = (char *)malloc(1024*1024*3*sizeof(char));
		
		for(int h=0; h<160; h++) {
			for(int g=0; g<210; g++) {
				for(int f=0; f<3; f++) {
					data[(h*1024*3)+(g*3)+f]=imagebufferRGB1[(h*640*3)+(g*3)+f];
				}
			}
		}
		
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0f,1.0f,1.0f);
		glDisable(GL_COLOR_MATERIAL);

	
		glBegin(GL_QUADS);
	
		glTexCoord2f(0.0,1.0);
		glVertex3f(-1,-1,-3);
		glTexCoord2f(1.0,1.0);
		glVertex3f(1,-1,-3);
		glTexCoord2f(1.0,0.0);
		glVertex3f(1,1,-3);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-1,1,-3);
	
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glDeleteTextures(1,(GLuint*) &id);	// generate on texture, just for allocation
		
		cout << " Fim image " << endl;

		
		//cout << id << endl;
		
		//}
	}

	
#endif
}
