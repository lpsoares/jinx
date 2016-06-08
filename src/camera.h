
#ifndef __camera_h
#define __camera_h


#include "ClientSocket.h"
#include "texture/texture.h"
#include "ezj.h"

using namespace std;

class Camera {
	public:
	
	Camera(Tex *ttex);
	~Camera();

	void draw(int eye);

#ifdef LINUX
	Tex* tex;
	
	ClientSocket *client_socket;
	
	unsigned char *buffer;
	//unsigned char *imagebuffer;
	
	int id;
	
	
	
	
  unsigned char *imagebuffer;
  unsigned char *imagebuffer2;
  unsigned char *imagebufferRGB1;
  unsigned char *imagebufferRGB2;
  
  //EZJ

  BMP_BUFFER dest;
  JPEG_BUFFER src;

#endif
  
	
};





#endif
