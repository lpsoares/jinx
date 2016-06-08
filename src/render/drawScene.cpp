
/* Need to do */

#include "render.h"


/* Case a openGL function specified */
void Render::OpenGL(void (*nfunc)(void)) {
	this->func = nfunc; 	// passing the opengl function to this class
	this->opengl=true;			// if true, means that was used a opengl routine instead a X3D file
}


void Render::DrawGLScene() {
	
	#ifdef DEBUG2
		cout << endl << "Starting to Draw Scene" << endl;	// Message about debuging
	#endif
	
	// repensar no uso
	//int count=0;							// used to calculate the number of polygons (not yet used)
	
	my_rrr->sendInput=1;					// Used to control the commands send as input
	
	my_rrr->eye_separation=0.016;
	my_rrr->dist_tela=1.5;
	
	//char error_message[MPI_MAX_ERROR_STRING];
	//int message_length;
	MPI_Status status;
	int error_code = MPI_SUCCESS;
	MPI_Wait(&my_rrr->request, &status); // Get the posRender->render
	error_code = MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	
	//-- defining the head rotation of the observerver --//
	my_rrr->HeadRotX=-my_rrr->posRender->render.head_rotat[0];		// observer head rotation x
	my_rrr->HeadRotY=-my_rrr->posRender->render.head_rotat[1];		// observer head rotation y
	my_rrr->HeadRotZ=-my_rrr->posRender->render.head_rotat[2];		// observer head rotation z

	my_rrr->HeadTransX=-my_rrr->posRender->render.head_trans[0];		// observer head rotation x
	my_rrr->HeadTransY=-my_rrr->posRender->render.head_trans[1];		// observer head rotation y
	my_rrr->HeadTransZ=-my_rrr->posRender->render.head_trans[2];		// observer head rotation z

	// setting time if necessary
	if(!my_rrr->regressivo) {
		my_rrr->regressivo = my_rrr->posRender->render.seconds;
	}
	
	// this rotine was inventer because some projector invert the eyes in the operation
	if(my_rrr->invertEyes) {   				
		my_rrr->eye_separation = -my_rrr->eye_separation;
	}
	#ifdef DEBUG2
		cout << "Managing Visual Groups" << endl;	// Message about debuging
	#endif
	VisualGroup();				// This enables to make groups in X3D visible or invisible
	
	#ifdef DEBUG2
		cout << "Drawing in the Frame Buffer" << endl;	// Message about debuging
	#endif
	my_rrr->drawFrameBuffer();	// This draws the images to the frame buffer
	
	// A ordem desses dois esta errada, corrigir isso
	
	#ifdef DEBUG2
		cout << "Casting rays in scene" << endl;	// Message about debuging
	#endif
	RayTracing();				// This cast a ray to select an object
	
	
	#ifdef DEBUG2
		cout << "Data Locking" << endl;	// Message about debuging
	#endif
	DataLock();					// This assure that all data is the same in all computer
	

	glFinish();					// does not return until the effects of all previously called GL commands are complete.
	
	#ifdef DEBUG2
		cout << "Frame Locking" << endl;	// Message about debugingglutSwapBuffers
	#endif
	FrameLock();				// the frame lock routine
	
	
	#ifdef DEBUG2
		cout << "Swapping buffers" << endl;	// Message about debuging
	#endif
	glutSwapBuffers();			// glut command to swap the buffers


	#ifdef DEBUG2
		cout << "Null information for joystick" << endl;	// Message about debuging
	#endif
	if(my_rrr->joystickMode==0) { // joystick mode 0
		float data[6] = {0,0,0,0,0,0};
		if(my_rrr->sendInput) {
    		if(my_rrr->move>0) {
				data[2] = ((float)(my_rrr->motionY)) / 80;
				data[4] = ((float)(my_rrr->motionX)) / 250;
				Input::move(data); // used to do not lock application
				my_rrr->sendInput=0; 
			}
		}
	}

	#ifdef DEBUG2
		cout << "Finishing to Draw Secene" << endl;	// Message about debuging
	#endif
	
}

/* ray tracing to select objects */
void Render::RayTracing() {
	
	float matrix[4][4]; // matrix for multipli operations
	
	midentity(matrix); // creating the identity matrix
	
	float m2[4][4];	// temporary matrix to hold the homogeneus operations
	
	//-- defining the position of the observerver --//
	matrix[3][0]=my_rrr->posRender->render.pos[0];	// observer position x
	matrix[3][1]=my_rrr->posRender->render.pos[1];	// observer position y
	matrix[3][2]=my_rrr->posRender->render.pos[2];	// observer position z
	
	// This is in oposite order, because the matrix operates like this, the other multiplications is in oposite order
	rotzmatrix(m2,-my_rrr->posRender->render.rot[2]);	multipleMatrix(m2,matrix);	// rotation in x and multiply
	rotymatrix(m2,-my_rrr->posRender->render.rot[1]);	multipleMatrix(m2,matrix);	// rotation in y and multiply
	rotxmatrix(m2,-my_rrr->posRender->render.rot[0]);	multipleMatrix(m2,matrix);	// rotation in z and multiply

	// This is used with the wand to detect a ray colision
	//if(my_rrr->wandRay) { // if the wand ray is on, cast the objects
		#ifdef DEBUG2
			cout << "Starting to cast rays in X3D file" << endl;
		#endif
		my_rrr->xxx->ray(matrix); // this funcions cast a ray in the objects and find with one is active or not
		my_rrr->wandClick=false;
	//}

}

void Render::VisualGroup() {

	int received;		// hold the value if a message received
	MPI_Status status;	// check the MPI status
	
	MPI_Test(&my_rrr->request_group, &received, &status);
	
	if(received) { 		// This is used to show or hide visual groups 
				
		MPI_Wait(&my_rrr->request_group, &status);
				
		MPI_Irecv(&my_rrr->groupOnOff, 1, MPI_INT, MPI_ANY_SOURCE, GROUP_ONOFF, MPI_COMM_WORLD, &my_rrr->request_group);

		if(my_rrr->opengl==false) {									// In the case of using an openGL function
			for(int h=0; h<my_rrr->xxx->menuCount; h++) {		// Do for each menu
				if(my_rrr->groupOnOff==my_rrr->xxx->Menu[h]) {
					(my_rrr->xxx->groupOnOff[h]?
						my_rrr->xxx->groupOnOff[h]=0:
						my_rrr->xxx->groupOnOff[h]=1);
					cout << "Name " << my_rrr->xxx->menuName[h] << endl;
					cout << "Value " << my_rrr->xxx->groupOnOff[h] << endl;
				}
			}
		}
	}

}



//* Function called when window is resized
void Render::ReSizeGLScene(int Width, int Height) {

	if(!(my_rrr->lll->conf->getWindow()-1) ) {	// in the case of option fullscreen or more than one view port
		glViewport(0, 0, Width, Height);		// Reset the view port
	}

}


/* this function is called to Init Graph routines */
void Render::graphInit(int *argc, char **argv) {
	
	glutInit(argc, argv);										// Used to initialize the GLUT library
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// Used to set the initial display mode 
	
	#ifdef DEBUG1
		cout << endl << "Stereo testing" << endl;
	#endif
		
	if(this->lll->conf->getStereoGlobal()==1) {
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH | GLUT_STEREO);
	}
	
	#ifdef DEBUG1
		cout << "Creating Window Text" << endl;
	#endif
	
	char computer_name[128]; 							// the variable that receives the computer name
	gethostname(computer_name,128);						// setting the computer name in the variable
		
	char top_window[128];								// string for the hearder on the window
	sprintf(top_window,"%s : %d",computer_name,my_rank);// setting the name and rank
		
	this->window = glutCreateWindow(top_window);		// Creates the Windows with help of glut
		
	if(lll->conf->getFullScreen(0) || (lll->conf->getWindow()-1) ) { 					// in the case of option fullscreen or more than one view port
		glutFullScreen();																// set the display to appear in full screen
		glutSetCursor(GLUT_CURSOR_NONE);												// In fulls screen is interesting to hide the cursor
		#ifdef DEBUG
			cout << " Full Screen" << endl;
			cout << "Size " << glutGet(GLUT_SCREEN_WIDTH) << " , " << glutGet(GLUT_SCREEN_HEIGHT) << endl;
		#endif
		if( !(lll->conf->getWindow()-1) ) {
			lll->conf->setWidth(glutGet(GLUT_SCREEN_WIDTH));
			lll->conf->setHeight(glutGet(GLUT_SCREEN_HEIGHT));
		}
	} else { 																			// in case of one window and no fullscreen
		
		int posx=lll->conf->getPositionX(0);
		int posy=lll->conf->getPositionY(0);
		
		int width=lll->conf->getWidth(0);
		int height=lll->conf->getHeight(0);
		
		glutInitWindowSize(width,height); 	// Configure the window size
		glutInitWindowPosition(posx,posy);	// Configure the window position
		glutReshapeWindow(width,height);	// Configure the window size	
		glutPositionWindow(posx,posy);		// Configure the window position
		
		#ifdef DEBUG
			cout << " Windows Position : " << lll->conf->getPositionX(0) << " , "  << lll->conf->getPositionY(0) << endl;
		#endif
		
	}

	if ( (this->lll->conf->getStereoGlobal()==4) ) {
		cout << "Creating Autostereo Image" << endl;
		leftImage = new GLubyte[lll->conf->getWidth(0)*lll->conf->getHeight(0)*3];
		rightImage = new GLubyte[lll->conf->getWidth(0)*lll->conf->getHeight(0)*3];
		autoStereoImage = new GLubyte[lll->conf->getWidth(0)*lll->conf->getHeight(0)*3];
	}
	
	#ifdef DEBUG1
	cout << "Preparing Camera" << endl;
	#endif	
	//this->cam = new Camera(tex);		// Display a camera, could be stereo
	
	x3d::x3DValues = new X3DValues();

	menu();														// Call the function that set the menus
	
	unsigned short int f;	// counter
	
	x3d::my_x3d->menuCount=0;
	for(f=0;f<groupOnOff_MAX;f++) {
		x3d::my_x3d->groupOnOff[f]=true; //turn on all groups
	}
	
	x3d::my_x3d->light=GL_LIGHT0;
	for(f=GL_LIGHT0;f < (GL_MAX_LIGHTS+GL_LIGHT0);f++) {
		#ifdef DEBUG
			cout << "Reseting lights" << endl;
		#endif
		glDisable(f);
	}
	
	#ifdef DEBUG1
	cout << "Serializing" << endl;
	#endif
	this->xxx->x3dbase = this->xxx->serialize();
	
	if(x3d::my_x3d->light==GL_LIGHT0) {
		
		cout << "No lights definded, creating head light" << endl;
		
		/*  --- All parameter here are default --- */
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
		
		float direction[3]={0.0f,0.0f,-1.0f};
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	
		//float ambient[4]={1,1,1,1};
		float ambient[4]={0.5f,0.5f,0.5f,1.0f};
		float diffuse[4]={0.7f,0.7f,0.7f,1.0f};
		float specular[4]={0.3f,0.3f,0.3f,1.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		
		//glLightfv(GL_LIGHT0, GL_POSITION, location);
		
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	
	#ifdef DEBUG1
	cout << "Creting Eye" << endl;
	#endif
	this->eye = new Eye();				// creates the eye
	if(eye==NULL) {
		cerr << "Error on eye" << endl;
	}
	
	#ifdef DEBUG1
		cout << "Creting LookAt" << endl;
	#endif
	this->lookat = new LookAt();		// creates the LookAt
	if(lookat==NULL) {
		cerr << "Error on lookat" << endl;
	}

	#ifdef DEBUG1
		cout << "Creting Viewport" << endl;
	#endif
	this->viewport = new Viewport();	// creates the Viewport
	if(viewport==NULL) {
		cerr << "Error on viewport" << endl;
	}
	
	#ifdef DEBUG1
		cout << "Setting GLUT" << endl;
	#endif
	
	glutDisplayFunc(Render::DrawGLScene);	// Register the function called whenener necessary a redraw.
	glutIdleFunc(Render::DrawGLScene);		// Register the function called when idle.
	glutReshapeFunc(Render::ReSizeGLScene); // Register the function called when the window is resized.
	glutKeyboardFunc(Render::keyPressed); 	// Register the function called when a key in the keyboard is pressed.
	glutMouseFunc(Render::Mouse);			// Register the function called when a mouse button is pressed.
	glutMotionFunc(Render::Motion);			// Register the function called when a mouse motion is done.
	glutSpecialFunc(Render::Special);		// Register the function called when a special keys is pressed.
	//glutIdleFunc(Render::Idle); 			// IDLE 
	
	InitGL(); // Initialize our window. 

	
}
	
	
void Render::loop() {
	
	#ifdef DEBUG1
		cout << "Starting OpenGL Loop" << endl;
	#endif
	
	glutMainLoop();												// Start event processing engine 
	
	//glutDestroyMenu(mainMenu);								// Destroy the menus
}

void Render::draw(int eye) {

	
	#ifdef DEBUG2
		cout << "\t\t\tDrawing objetcs" << endl;	// Message about debuging
	#endif

	if(!calibration) {
	
		glPushMatrix();

		if(this->posRender->render.wonoff) {
			#ifdef DEBUG2
				cout << "\t\t\tDrawing wand" << endl;	// Message about debuging
			#endif
			this->drawWand();
		}

		#ifdef DEBUG2
			cout << "\t\t\tAdjusting user position" << endl;	// Message about debuging
		#endif		
		glRotatef(-this->posRender->render.rot[0],1.0f,0.0f,0.0f);	// Rotate the world to the user position
		glRotatef(-this->posRender->render.rot[1],0.0f,1.0f,0.0f);	// Rotate the world to the user position
		glRotatef(-this->posRender->render.rot[2],0.0f,0.0f,1.0f);	// Rotate the world to the user position
		glTranslatef(this->posRender->render.pos[0],this->posRender->render.pos[1],this->posRender->render.pos[2]);	// Translate the world to the user position

		#ifdef DEBUG2
			cout << "User Rotation/Position eye(" << eye << ")" << endl;
			cout << "RX=" <<  this->posRender->render.rot[0] << " ";
			cout << "RY=" <<  this->posRender->render.rot[1] << " ";
			cout << "RZ=" <<  this->posRender->render.rot[2] << " ";
			cout << "   ";
			cout << "TX=" <<  this->posRender->render.pos[0] << " ";
			cout << "TY=" <<  this->posRender->render.pos[1] << " ";
			cout << "TZ=" <<  this->posRender->render.pos[2];
			cout << endl;
		#endif
		
		
		// For Head Light
		if(x3d::my_x3d->light==GL_LIGHT0) {
			float location[3]={0.0f,0.0f,0.0f};
			location[0]=this->posRender->render.pos[0];
			location[1]=this->posRender->render.pos[1];
			location[2]=this->posRender->render.pos[2];
			float direction[3]={0.0f,0.0f,-1.0f};
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
			glLightfv(GL_LIGHT0, GL_POSITION, location);
		}
				
		if(this->grid) {
			#ifdef DEBUG2
				cout << "\t\t\tDrawing grid" << endl;	// Message about debuging
			#endif
			this->tex->gridPlane();						// Draw a grid
		}
		
		if(this->camera) {
			#ifdef DEBUG2
				cout << "\t\t\tDrawing virtual camera" << endl;	// Message about debuging
			#endif
			this->cam->draw( (eye==3?1:0) );			// Draw a camera view (in the case the right view )
		}
	
		if(this->opengl==false) {
			#ifdef DEBUG2
				cout << "\t\t\tDrawing Scene Graph" << endl;	// Message about debuging
			#endif
			this->xxx->draw(this->posRender,eye);	// the x3d draw routine
			#ifdef DEBUG2
				cout << "\t\t\tFinishing Scene Graph" << endl;	// Message about debuging
			#endif
		} else {
			this->func();
		}

		glPopMatrix();	
		
	} else {
		
		 calibrationScreen();
		
	}
	
}
