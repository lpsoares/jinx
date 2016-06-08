
/* Need to do */

#include "render.h"
#include "calibration.h"

void Render::calibrationScreen() {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 0.1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT );
	glDepthFunc(GL_ALWAYS);                // this make everthing appear without carring the zbuffer
	glDisable(GL_LIGHTING);                //
	glTranslatef(0,0,-1.0);                 // to not display over the camera

	glLineWidth(calibrationLineWidth);
		
	unsigned short int f;  // just a counter
	unsigned short int g;  // just a counter
	
	
	if(this->calibrationPatern==0) { // this pattern is a 50% dark
		for(f=0;f<32;f+=2) {
			for(g=0;g<4;g++) {
				mask[(f*4)+g]=0xAA;
			}
			for(g=0;g<4;g++) {
				mask[((f+1)*4)+g]=0x55;
			}
		}
	} else if(this->calibrationPatern==1) { // this is vertical line
		for(f=0;f<32*4;f++) {
			mask[f]=0xAA;
		}
	} else if(this->calibrationPatern==2) { // this is horizontal lines
		for(f=0;f<32;f+=2) {
			for(g=0;g<4;g++) {
				mask[(f*4)+g]=0xFF;
			}
			for(g=0;g<4;g++) {
				mask[((f+1)*4)+g]=0x00;
			}
		}
	}
	
	glClearColor(
		my_rrr->calibrationClearColor[0]*my_rrr->calibrationClearColorPercent/100,
		my_rrr->calibrationClearColor[1]*my_rrr->calibrationClearColorPercent/100,
		my_rrr->calibrationClearColor[2]*my_rrr->calibrationClearColorPercent/100,
			0.0f);		// Clear the background with black
	
	if(!degrade) { // draws a degrade in each edge
		
		glColor3f(my_rrr->calibrationFrontColor[0]*my_rrr->calibrationFrontColorPercent/100,
				  my_rrr->calibrationFrontColor[1]*my_rrr->calibrationFrontColorPercent/100,
				  my_rrr->calibrationFrontColor[2]*my_rrr->calibrationFrontColorPercent/100);
		
		// draws the grid
		glBegin(GL_LINES);
		for(f=0;f<=my_rrr->calibrationDivisions;f++) {
			
			glVertex3f(-1.0,(((float)f)-((float)(my_rrr->calibrationDivisions)/2.0))/(((float)(my_rrr->calibrationDivisions)/2.0)+0.001),0.0f);
			glVertex3f( 1.0,(((float)f)-((float)(my_rrr->calibrationDivisions)/2.0))/(((float)(my_rrr->calibrationDivisions)/2.0)+0.001),0.0f);
				
			glVertex3f((((float)f)-((float)(my_rrr->calibrationDivisions)/2.0))/(((float)(my_rrr->calibrationDivisions)/2.0)+0.001),-1.0,0.0f);
			glVertex3f((((float)f)-((float)(my_rrr->calibrationDivisions)/2.0))/(((float)(my_rrr->calibrationDivisions)/2.0)+0.001), 1.0,0.0f);
						
		}
		glEnd();		

		// draws a circle that fill all the screen
		glBegin(GL_LINE_STRIP);
		for(f=0;f<=360;f++) {
			glVertex3f( cos((float)(f)*((2*PI)/360)), sin((float)(f)*((2*PI)/360)), 0.0 );
		}
		glEnd();
	
	} else { // draws a degrade in each edge
		    
		if(calibrationDegradeDirection==4) {
			glEnable(GL_POLYGON_STIPPLE); 
			glPolygonStipple(mask);
		}
		
		glBegin(GL_QUADS);
			if(calibrationDegradeDirection==0 || calibrationDegradeDirection==3) 
				{glColor3f( my_rrr->calibrationClearColor[0]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[1]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[2]*my_rrr->calibrationClearColorPercent/100);}
				else
				{glColor3f(my_rrr->calibrationFrontColor[0]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[1]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[2]*my_rrr->calibrationFrontColorPercent/100);}
			glVertex3f( 1.0f,-1.0f, 0.0f);
		
			if(calibrationDegradeDirection==2 || calibrationDegradeDirection==3) 
				{glColor3f( my_rrr->calibrationClearColor[0]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[1]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[2]*my_rrr->calibrationClearColorPercent/100);}
				else 
				{glColor3f(my_rrr->calibrationFrontColor[0]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[1]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[2]*my_rrr->calibrationFrontColorPercent/100);}
			glVertex3f( 1.0f, 1.0, 0.0);
							
			if(calibrationDegradeDirection==1 || calibrationDegradeDirection==2) 
				{glColor3f( my_rrr->calibrationClearColor[0]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[1]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[2]*my_rrr->calibrationClearColorPercent/100);}
				else
				{glColor3f(my_rrr->calibrationFrontColor[0]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[1]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[2]*my_rrr->calibrationFrontColorPercent/100);}
				glVertex3f(-1.0f, 1.0f, 0.0f);
						
			if(calibrationDegradeDirection==0 || calibrationDegradeDirection==1) 
				{glColor3f( my_rrr->calibrationClearColor[0]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[1]*my_rrr->calibrationClearColorPercent/100,
						    my_rrr->calibrationClearColor[2]*my_rrr->calibrationClearColorPercent/100);}
				else
				{glColor3f(my_rrr->calibrationFrontColor[0]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[1]*my_rrr->calibrationFrontColorPercent/100,
						   my_rrr->calibrationFrontColor[2]*my_rrr->calibrationFrontColorPercent/100);}
				glVertex3f(-1.0f,-1.0f, 0.0f);
			
		glEnd();
		
		if(calibrationDegradeDirection==4) {
			glDisable(GL_POLYGON_STIPPLE); 
		}		
		
	}
	
	// draw color bars
	if(calibrationColorBars) {
		
		
		glColor3f(1.0f,0.0f,0.0f); // Red
		glBegin(GL_QUADS);
		glVertex3f(-(1.0f/3.0f),-0.8f, 0.0f);
		glVertex3f(-(1.0f/3.0f), 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-0.8f, 0.0f);
		glEnd();
		
		
		
		glColor3f(0.0f,1.0f,0.0f); // Green
		glBegin(GL_QUADS);
		glVertex3f( (1.0f/3.0f),-0.8f, 0.0f);
		glVertex3f( (1.0f/3.0f), 1.0f, 0.0f);
		glVertex3f(-(1.0f/3.0f), 1.0f, 0.0f);
		glVertex3f(-(1.0f/3.0f),-0.8f, 0.0f);
		glEnd();
		
		
		
		glColor3f(0.0f,0.0f,1.0f); // Blue
		glBegin(GL_QUADS);
		glVertex3f( 1.0f,-0.8f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glVertex3f( (1.0f/3.0f), 1.0f, 0.0f);
		glVertex3f( (1.0f/3.0f),-0.8f, 0.0f);
		glEnd();
		
		
		
		glColor3f(1.0f,1.0f,1.0f); // White
		glBegin(GL_QUADS);
		glVertex3f( 0.0f,-1.0f, 0.0f);
		glVertex3f( 0.0f,-0.8f, 0.0f);
		glVertex3f(-1.0f,-0.8f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();
		
		
		glColor3f(0.0f,0.0f,0.0f); // Black
		glBegin(GL_QUADS);
		glVertex3f( 1.0f,-1.0f, 0.0f);
		glVertex3f( 1.0f,-0.8f, 0.0f);
		glVertex3f( 0.0f,-0.8f, 0.0f);
		glVertex3f( 0.0f,-1.0f, 0.0f);
		glEnd();
		
		
		
		
	}

			
	glDepthFunc(GL_LESS);                // this make everthing appear without carring the zbuffer
	glEnable(GL_LIGHTING);               // 
	
}
