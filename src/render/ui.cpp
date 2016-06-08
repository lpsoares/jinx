

#include "render.h"

void Render::Gmenu() {
	
	char texto[256];        // Text to write on the screen
    char *letra;            // Para pegar letra a letra

	int f;
	
	int menu_definition = 16;
	
	if(this->gmenu) {

		glPushAttrib(GL_POLYGON_BIT);
		
		glDepthFunc(GL_ALWAYS); 							// this make everthing appear without carring the zbuffer
		glDisable(GL_LIGHTING);								//
		
		glPushMatrix();                 // Armazena matrix na pilha

	    glTranslatef(0,-0.2,-1.2);         // joga objetos para a frente para serem visiveis
			
		glColor4f(0,1,0,0.9);           //

       	glBegin(GL_POLYGON);            // Funcao para desenhar poligonos baseado em quadrados

       	for(f=0;f<=menu_definition;f++) {
                glVertex2f(     0.1*cos( ((float)f)*2*((float)PI)/((float)menu_definition) ) +
                                (((f>4)&&(f<12))?-0.8:0.8) ,
                                0.1*sin( ((float)f)*2*((float)PI)/((float)menu_definition) ) +
                                ((f<8?0.8:-0.5))
                                );

       	}

       	glEnd();                        // Finaliza funcao de desenhar poligonos

		glColor4f(1,1,1,0.9);               // Cor do texto
			
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_2D);							//

		my_rrr->tex->readTexture("logo_galText.bmp",false,false);				//
	
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
		glTranslatef(0,0,0.1); // joga objetos para a frente para serem visiveis
	
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(-0.7f, -0.5f);
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(0.7f, -0.5f);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(0.7f, 0.8f);
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(-0.7f, 0.8f);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);							//
	
   	    sprintf(texto, "Menu\n"); // prepara texto na variavel
   	    glRasterPos2f(0, 0.8);  // Define posicao de escrita
   	    for (letra=texto; *letra != '\n'; letra++)      // pega letra por letra
       	        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *letra); // Escreve a letra
		
       	glPopMatrix();                  // Recupera matrix da pilha
			
		glEnable(GL_LIGHTING);								//
		glDepthFunc(GL_LESS);
		
		glPopAttrib();
		
	}
		
}


void Render::Gmenu_close() {

	char texto[256];        // Texto para escrever na tela
    char *letra;            // Para pegar letra a letra

	int f;
	
	int menu_definition = 16;
	
	if(this->gmenu_close) {

		glPushAttrib(GL_POLYGON_BIT);
		
		glDepthFunc(GL_ALWAYS); 							// this make everthing appear without carring the zbuffer
		glDisable(GL_LIGHTING);								//
		
		for(int g=0;g<2;g++) {
		
			if(g) {
				glDrawBuffer(GL_BACK_LEFT);
				glLoadIdentity();
			} else {
				glDrawBuffer(GL_BACK_RIGHT);
				glLoadIdentity();		
			}

		
			glPushMatrix();                 // Armazena matrix na pilha

	        glTranslatef(0,-0.2,-1.2);         // joga objetos para a frente para serem visiveis
			
			glColor4f(0,1,0,0.9);           //

        	glBegin(GL_POLYGON);            // Funcao para desenhar poligonos baseado em quadrados

        	for(f=0;f<=menu_definition;f++) {

                glVertex2f(     0.1*cos( ((float)f)*2*((float)PI)/((float)menu_definition) ) +
                                (((f>4)&&(f<12))?-0.8:0.8) ,
                                0.1*sin( ((float)f)*2*((float)PI)/((float)menu_definition) ) +
                                ((f<8?0.8:-0.5))
                                );

        	}

        	glEnd();                        // Finaliza funcao de desenhar poligonos

			glColor4f(1,1,1,0.9);               // Cor do texto
			
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
	
			glEnable(GL_TEXTURE_2D);							//

			my_rrr->tex->readTexture("logo_saida.bmp",false,false);				//
	
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
			glTranslatef(0,0,0.1); // joga objetos para a frente para serem visiveis
	
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(-0.7f, -0.5f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(0.7f, -0.5f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(0.7f, 0.8f);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(-0.7f, 0.8f);
			glEnd();
		
			glDisable(GL_TEXTURE_2D);							//
	
    	    sprintf(texto, "Menu\n"); // prepara texto na variavel
    	    glRasterPos2f(0, 0.8);  // Define posicao de escrita
    	    for (letra=texto; *letra != '\n'; letra++)      // pega letra por letra
        	        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *letra); // Escreve a letra
			
        	glPopMatrix();                  // Recupera matrix da pilha
			
		}

		glEnable(GL_LIGHTING);								//
		glDepthFunc(GL_LESS);
		
		glPopAttrib();
		
	}
	
}



void Render::clock() {
		
	char *letra;            // Para pegar letra a letra

	char tempo[8];
	
	double periodo = 180;
	double first_menu = 20;
	
	double sec;
	
	if(this->gclock) {
	
		sec = (posRender->render.seconds - (this->regressivo + periodo ));

		#ifdef DEBUG		
			cout << " Time passed " << sec << endl;
		#endif
	
		if(sec < (-periodo+first_menu) ) {
			this->gmenu=true;
		} else {
			this->gmenu=false;
		}

		if(sec>=0) {
			sec=0;
			this->gmenu_close=true;
		}
		
		sprintf(tempo,( (((int)-sec)%60)<10?"%d:0%d":"%d:%d"),
			   ((int)-sec)/60 , ((int)-sec)%60 );
	
		glDepthFunc(GL_ALWAYS); 							// this make everthing appear without carring the zbuffer
		glDisable(GL_LIGHTING);								// turning off the lights
		
		glPushMatrix();        					        	// Pusshing the transformation matrix
		
		glLoadIdentity();									//
				
		glTranslatef(1.1,0.7,-my_rrr->dist_tela);			// joga objetos para a frente para serem visiveis
	        
		glColor4f(1,1,1,0.85);           					//
			
		gluDisk(this->obj,0,0.2,32,1);						//
	
		glTranslatef(0.0f,0.0f,0.01f);						// Move Left 1.5 Units And Into The Screen 6.0
		
		if( (-sec/periodo) >= 0.66 ) {
			glColor4f(0,1,0,1);	
			gluPartialDisk(this->obj,0.08,0.14,16,1, 0, 360 + ((sec/periodo)*360) );
		} else {
			glColor4f(0,1,0,1);	
			gluPartialDisk(this->obj,0.08,0.14,16,1, 0, 120);
		}
						
		if( (-sec/periodo) <= 0.33 ) {
			glColor4f(1,1,0,1);	
			gluPartialDisk(this->obj,0.08,0.14,16,1,120,120);
		} else if( (-sec/periodo) <= 0.66 )  {
			glColor4f(1,1,0,1);	
			gluPartialDisk(this->obj,0.08,0.14,16,1,120, 240 + ((sec/periodo)*360) );
		}
			
		if( (-sec/periodo) <= 0.33 ) {
			glColor4f(1,0,0,1);	
			gluPartialDisk(this->obj,0.08,0.14,16,1,240, 120 + ((sec/periodo)*360) );
		}
	
		glColor4f(1,1,1,1);	
		
		glRasterPos2f(-0.06,-0.02);
	
		for (letra=tempo; *letra != '\0'; letra++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *letra);
		}

		glPopMatrix();                  // Recupera matrix da pilha
			
		glEnable(GL_LIGHTING);								//
		glDepthFunc(GL_LESS);
		
	}
	
}
