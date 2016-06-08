
#include "render.h"

#include "../avatars.h"

#include "Maths/Quaternion.h"
#include "Maths/MathsStruct.h"

void Render::AuxiliaryInformation() {

	// ---  Rotine that find problems in the OpenGL system ---
	GLenum glError;
	do {
		glError = glGetError();
		if(glError!=GL_NO_ERROR) {
			cerr << "OpenGL error : ";
			cerr << gluErrorString(glError) << endl;
		}
	} while(glError != GL_NO_ERROR);


	my_rrr->showPerformance(); // draw the framerate on screen and console
	
	//my_rrr->Gmenu();
	
	//my_rrr->Gmenu_close();
	
	my_rrr->clock();
	
}
          
void Render::calcPerf() {
	
	int end;
	
	if (!this->framenum) {
		
		end = glutGet(GLUT_ELAPSED_TIME);
		
		cout << "processo " << this->my_rank;
		cout << " em ";
		cout << (this->lll->conf->getStereoGlobal()?"stereo ":"mono ");
		cout <<	", tempo total: " << end;
		
		cout <<	", tempo total: " << end-this->start;
		cout << " ms , em " << FRAMES*1000.0/(float)(end-this->start);
		//cout << " com " <<  count;   //reestudar isso
		//cout << "polygons" 
		cout << endl;
					
		sprintf(my_rrr->fps, " %f FPS\n",FRAMES*1000.0/(float)(end-this->start));
					
		this->start = end;
		
		GLuint textures[256];
		for(int ff=0;ff<x3d::my_x3d->texture_bmp->textfile;ff++)
			textures[ff]=x3d::my_x3d->texture_bmp->texture2[ff];
	
		GLboolean residences[256];
	
		GLboolean testes;
		testes = glAreTexturesResident(x3d::my_x3d->texture_bmp->textfile,textures,residences);
		if(testes==GL_FALSE) {
			cout << "Warning no memory enought in hardware for texture" << endl; ;
		}

		#ifdef DEBUG
			for(int ff=0;ff<x3d::my_x3d->texture_bmp->textfile;ff++)
				cout << " " << (residences[ff]==GL_TRUE?"SIM":"NAO") ;
			cout << endl;
		#endif		
		
	}
	
	this->framenum++;
	if (this->framenum >= FRAMES) {
		this->framenum = 0;
	}

}

void Render::showPerformance() {

	float cor[4]={1.0f,1.0f,1.0f,1.0f};
	
	if(this->framerate) {	//frag to draw or not the framerate on screen
	
		glPushMatrix();
		glLoadIdentity();
		glDepthFunc(GL_ALWAYS); 							// this make everthing appear without carring the zbuffer
		glDisable(GL_LIGHTING);								//
		glDisable(GL_TEXTURE_2D);

		glColor4fv(cor);
		
		glTranslatef(0,0,-1.5);			// to not display over the camera
		
		#ifdef DEBUG
			glGetFloatv(GL_CURRENT_RASTER_COLOR,cor);
			cout << "Cor " << cor[0] << " , "  << cor[1] << " , " << cor[2] << " , " << cor[3] << endl;
		#endif
		
		glRasterPos2f(-0.95, -0.97); 	// define the position to write
		
		for (letra=this->fps; *letra != '\0'; letra++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *letra);
		}
			
		glPopMatrix();

		glEnable(GL_LIGHTING);								//
		glDepthFunc(GL_LESS);								//
	
	}
	
}

/* A  function that draws the Jinx logo in the screen  */
void Render::Logo() {

	#ifdef DEBUG1
		cout << "\tDrawing Logo" << endl;	// Message about debuging
	#endif
	
	if(!calibration) {
		
		glPushMatrix();
		
		glDepthFunc(GL_ALWAYS); 							// this make everthing appear without carring the zbuffer

		glColor3d(1,1,1);
	
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		
		glLoadIdentity();
		
		glEnable(GL_TEXTURE_2D);							//
		glDisable(GL_LIGHTING);								//

		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		
		this->tex->readTexture("jinx.rgb",false,false);				//
	
		glPushAttrib(GL_POLYGON_BIT);						//

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);			//
		
		glTranslatef(((ScreenConners[1]-1)/2)*FRUSTUM_NEAR,((ScreenConners[2]+1)/2)*FRUSTUM_NEAR,-FRUSTUM_NEAR); 							// joga objetos para a frente para serem visiveis

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(0.6*((1/1.5)*FRUSTUM_NEAR), -1.0*((1/1.5)*FRUSTUM_NEAR));
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(1.0*((1/1.5)*FRUSTUM_NEAR), -1.0*((1/1.5)*FRUSTUM_NEAR));
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(1.0*((1/1.5)*FRUSTUM_NEAR), -0.8*((1/1.5)*FRUSTUM_NEAR));
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(0.6*((1/1.5)*FRUSTUM_NEAR), -0.8*((1/1.5)*FRUSTUM_NEAR));
		glEnd();
	
		glEnable(GL_LIGHTING);								//
		glDepthFunc(GL_LESS);

		glDisable(GL_TEXTURE_2D);							//
	
		glColor3d(0.0,0.0,0.0);
	
		glPopAttrib();
	
		glPopMatrix();
		
	}
	
}




/* Display information about the openGL */
void Render::info() {
	
	#ifdef DEBUG
	
	int max;				// variable to get the max staks depth
	
	cout << "OpenGL Vendor    : " << glGetString(GL_VENDOR) << endl;
	cout << "OpenGL Renderer  : " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL Version   : " << glGetString(GL_VERSION) << endl;
	cout << "OpenGL Extensios : " << glGetString(GL_EXTENSIONS) << endl;
	cout << endl;
	cout << "GLU Version      : " << gluGetString(GLU_VERSION) << endl;
	cout << "GLU Extensions   : " << gluGetString(GLU_EXTENSIONS) << endl;
	cout << endl;
	
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&max);
	cout << "Modelview Stack Depth  : " << max << endl;
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,&max);
	cout << "Projection Stack Depth : " << max << endl;
	
	#endif
	
}

void Render::quality() {

	// todo
	
	/*
	glHint(GL_FOG_HINT,GL_FASTEST);				//this function lets the fog rendering fast or quality
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);	//this function lets the points rendering fast or quality
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);		//this function lets the lines rendering fast or quality
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);	//this function lets the polygons rendering fast or quality
	*/
	
}	


float xr=0;
float yr=0;
float zr=0;

void Render::drawWand() {
	
	#ifdef DEBUG
		cout << "Render Wand " << this->posRender->render.trans[0]<< " , " << this->posRender->render.trans[1] << " , " << this->posRender->render.trans[2] << endl;
		cout << " REN " << posRender->render.wonoff << endl;
	#endif
				
	if(this->posRender->render.buttons & SBALL_BUTTON_1) {
		this->wandType=1;
	}
	
	if(this->posRender->render.buttons & SBALL_BUTTON_2) {
		this->wandType=2;
	}
	
	if(this->posRender->render.buttons & SBALL_BUTTON_3) {
		this->wandType=3;
	}

	if(this->posRender->render.buttons & SBALL_BUTTON_4) {
		this->wandType=4;
	}
	
	if(this->posRender->render.buttons & SBALL_BUTTON_5) {
		this->wandType=5;
	}

	if(this->posRender->render.buttons & SBALL_BUTTON_6) {
		this->wandType=6;
	}
	
	if(this->posRender->render.buttons & SBALL_BUTTON_9) {
		this->wandRay=true;
	}
	
	if(this->posRender->render.buttons & SBALL_BUTTON_8) {
		this->wandRay=false;
	}

	if(this->posRender->render.buttons & SBALL_BUTTON_7) {
		this->wandClick=true;
	}
	
	glPushMatrix();
	
	#ifdef DEBUG3	
	cout << "Type : " << this->posRender->render.type << endl;
	cout << "Quaternion : " << " w=" << this->posRender->render.quaternion[0] << 
								 ", x=" << this->posRender->render.quaternion[1] << 
								 ", y=" << this->posRender->render.quaternion[2] <<
								 ", z=" << this->posRender->render.quaternion[3] << endl;
	#endif	

	
	Quaternion	quat;		// quaternion 
	Vector3D	axis;		// temp axis angle representation
	float angle;
	float ax, ay, az;


	if(this->posRender->render.type==0) {

		glTranslatef(this->posRender->render.trans[0],this->posRender->render.trans[1],this->posRender->render.trans[2]);
		glRotatef(this->posRender->render.rotat[0],1.0f,0.0f,0.0f);
		glRotatef(this->posRender->render.rotat[1],0.0f,1.0f,0.0f);
		glRotatef(this->posRender->render.rotat[2],0.0f,0.0f,1.0f);
		glScalef(0.5,0.5,0.5);
	
		#ifdef DEBUG
	
			cout << " POS: " << this->posRender->render.trans[0];
			cout << " , " << this->posRender->render.trans[1];
			cout << " , " << this->posRender->render.trans[2] << endl;
	
			cout << " ROT: " << this->posRender->render.rotat[0];
			cout << " , " << this->posRender->render.rotat[1];
			cout << " , " << this->posRender->render.rotat[2] << endl;
	
			cout << " FIN: " ;			// show the fingers status
			for(unsigned int ff=0;ff<20;ff++) {
					cout << this->posRender->render.fingers[ff] << " , ";
			}
			cout << endl;
	
		#endif

	} else { // type==1
	
		quat.set(this->posRender->render.quaternion[1],
			this->posRender->render.quaternion[2],
			this->posRender->render.quaternion[3],
			this->posRender->render.quaternion[0]);	// build the quaternion representing the quaternion change

		quat.GetAxisAngle(axis, angle);		// get angle axis representation

		axis.GetValues(ax, ay, az);		// extract values from angle axis
		angle = RADTODEG(angle);

		#ifdef DEBU3
			cout << " Angle : " << angle << endl;
			cout << " Axes : " << ax << " , "  << ay << " , " << az << endl;
		#endif

		glTranslatef(this->posRender->render.trans[0],this->posRender->render.trans[1],this->posRender->render.trans[2]);	// move camera


		/*
		glRotatef(90, 0.0f, 0.0f, 1.0f);		// multiply into matrix
		glRotatef(180, 0.0f, 1.0f, 0.0f);		// multiply into matrix
		glRotatef(90, 1.0f, 0.0f, 0.0f);		// multiply into matrix
		*/
		// Acerta a rotacao

		//glRotatef(angle, ax, ay, az);		// multiply into matrix
		glRotatef(angle, ay, az, ax);		// multiply into matrix

		// Acerat o objeto
		/*
		glRotatef(180, 1.0f, 0.0f, 0.0f);		// multiply into matrix
		glRotatef(-90, 0.0f, 0.0f, 1.0f);		// multiply into matrix
		*/
		glScalef(0.5,0.5,0.5);
	}
	
	
	
	float witheColor[4]={1,1,1,1};
	float redColor[4]={1,0,0,1};
	float greenColor[4]={0,1,0,1};
	float blueColor[4]={0,0,1,1};
		
	glDisable(GL_TEXTURE_2D);
		
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, witheColor);

	unsigned short int f;
	
	//this->wandType=1;

	#ifdef DEBUG3
		if(wandRay) cout << "Ray emiting from wand!!!" << endl;
	#endif
	
	switch(wandType) {
		case 1:
			glPushMatrix();
			glColor3f(0.8f,0.8f,0.8f);
			glutWireCone(0.5,1,32,32);
			glutWireCone(0.5,0,32,32);
			if(wandRay) {
				glLineWidth(3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColor);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,100);
				glEnd();
			}
				glPopMatrix();
			break;
		case 2:
			Avatars::wandgl();
			if(wandRay) {
				glLineWidth(3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, redColor);
				glColor3f(1,0,0);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,100);
				glEnd();
			}
			break;
		case 3:
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, witheColor);
			glutSolidSphere(0.5,32,32);
			for(f=0;f<3;f++) {
				glPushMatrix();
				glRotated(120*((double)f),0,0,1);
				glRotatef(this->opening,1,0,0);
				glTranslatef(0,0,1.4);
				glutSolidSphere(0.15,16,16);
				glScaled(0.15,0.15,1);
				glTranslated(0,0,-0.5);
				glutSolidCube(1);
				glScalef(1/0.15,1/0.15,1);
				glRotated(-90,1,0,0);
				glTranslated(0,-0.5,0.5);
				glScalef(0.15,0.15,1);
				glutSolidCube(1);
				glPopMatrix();
			}
			if(wandRay) {
				if(this->opening>40) opening-=1;
			} else {
				if(this->opening<60) opening+=1;
			}
			break;
		case 6:
			if(wandRay) {
				glLineWidth(3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
				//glColor3f(1.0f,0.0f,0.0f);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,100);
				glEnd();
				glLineWidth(1);
			}
			break;
		case 7: // used for gloves
			this->wandRay=0;									// just to not cast any ray
			Avatars::glove(this->posRender->render.fingers); 	// routine that draws the glove
			break;
		default:
			Avatars::wandgl();
			if(wandRay) {
				glLineWidth(3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, redColor);
				glColor3f(1,0,0);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,100);
				glEnd();
			}
			break;
	}
	
	glPopMatrix();
	
}
