#include "texture.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>


/* Draw a grid at origin */
void Tex::gridPlane() {
	
	GLubyte checkImage[256][256][4];
	GLuint texName;
	
	if(checker) {
		
		int i,j;					// Counters 
		int c;						// Temporary 
		
		for (i=0;i < 256; i++) {
			for (j=0; j< 256; j++) {
				c = ((((i&0x8)==0)^((j&0x8))==0))*255;
				checkImage[i][j][0] = (GLubyte) c;
				checkImage[i][j][1] = (GLubyte) c;
				checkImage[i][j][2] = (GLubyte) c;
				checkImage[i][j][3] = (GLubyte) 255;
			}
		}
		
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		if(false) // in the case of mipmap
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
		} 
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
			gluBuild2DMipmaps( GL_TEXTURE_2D, 3, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, checkImage );
		}

		checker=false;
	}

	glBindTexture(GL_TEXTURE_2D, texName);						//
	
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);			// Draw a square at the floor 
	glTexCoord2f(0.0,0.0); glVertex3f(-this->size, -3.5, -this->size);
	glTexCoord2f(0.0,1.0); glVertex3f(-this->size, -3.5,  this->size);
	glTexCoord2f(1.0,1.0); glVertex3f( this->size, -3.5,  this->size);
	glTexCoord2f(1.0,0.0); glVertex3f( this->size, -3.5, -this->size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
}
