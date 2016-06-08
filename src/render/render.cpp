/* Need to do */

#include "render.h"
#include "../sphelib.h"
#include "wwwsys.h"
#include "../engine.h"


Render* Render::my_rrr; // static variable

/* Prepare a Graph communicator for Used mainly for frame-lock */
void Render::graphCommunicator() {
	
	unsigned int f;				// a temporary counter
	int proc;					// used to hold the process numbers

	MPI_Group group_world;		// temporary for create the graph_group used to frame lock
	
	MPI_Comm_size(MPI_COMM_WORLD, &this->ranks);
	if(this->ranks==1) cout << "Running in stand alone" << endl;
		
	graph_ranks = new int[(ranks-3)];
	if(graph_ranks==NULL) cerr << "Error creating graphical ranks for MPI" << endl;
	
	for(f=0, proc = GRAPH; proc < ranks ; f++, proc++) {
		graph_ranks[f] = proc;
	}
	
	MPI_Comm_group(MPI_COMM_WORLD, &group_world);
	MPI_Group_incl(group_world, this->ranks-GRAPH, graph_ranks, &graph_group);
	MPI_Comm_create(MPI_COMM_WORLD, graph_group, &graph_comm);
		
}

/* Prepare a Graph communicator for Used mainly for frame-lock */
void Render::graphCommunicatorGlobal() {
	
	int f;
	int ranks2;
	int* graph_ranks2;
	int proc;
	MPI_Group graph_group2;
	MPI_Comm graph_comm2;
	MPI_Group group_world;	
	
	MPI_Comm_size(MPI_COMM_WORLD, &ranks2);
	
	graph_ranks2 = new int[(ranks2-3)];
	if(graph_ranks2==NULL) {
		cerr << "Error creating graphical ranks for MPI" << endl;
	}
	
	for(f=0, proc = GRAPH; proc < ranks2 ; f++, proc++)	{
		graph_ranks2[f] = proc;
	}
	
	MPI_Comm_group(MPI_COMM_WORLD, &group_world);
	MPI_Group_incl(group_world, ranks2-GRAPH, graph_ranks2, &graph_group2);
	MPI_Comm_create(MPI_COMM_WORLD, graph_group2, &graph_comm2);
	
}

/* Constructor */
Render::Render(xml* temp_lll, x3d* temp_xxx, int temp_ranks) {
	
	this->graphCommunicator();
	
	MPI_Comm_rank(MPI_COMM_WORLD, &this->my_rank); // set the rank number to post use

	this->joystickMode=0;
	
	this->headPosition[0]=0;
	this->headPosition[1]=0;
	this->headPosition[2]=0;
	
	this->headRotation[0]=0;
	this->headRotation[1]=0;
	this->headRotation[2]=0;
	
	this->sendInput=0;			// Used to control the commands send as input
	
	this->lll = temp_lll;		// the xml class that holds the configuration
	this->xxx = temp_xxx;		// the x3d class that holds the model
	
	this->opengl=0;				// if false, means that it is using a X3D file

	this->obj = gluNewQuadric();
	
	this->grid=false;			// if true, draw a grid on the floor
	this->camera=false;			// if true, draw a camera view
	this->calibration=false;	// if true, draw a camera view
	this->framerate=false;		// if true, draw a camera view
	this->invertEyes=false;		// if true, invert eyes on stereoscopy
	this->gmenu=true;			//
	this->gclock=false;
	this->gmenu_close=true;

	this->degrade=false;
	this->calibrationDivisions=20;
	this->calibrationClearColor[0]=0;
	this->calibrationClearColor[1]=0;
	this->calibrationClearColor[2]=0;
	this->calibrationFrontColor[0]=1;
	this->calibrationFrontColor[1]=1;
	this->calibrationFrontColor[2]=1;
	this->calibrationDegradeDirection=0;
	this->frontBackColor=false;
	this->calibrationClearColorPercent=100;
	this->calibrationFrontColorPercent=100;
	this->calibrationPatern=0;
	this->calibrationLineWidth=1;
	this->calibrationColorBars=false;
	
	this->regressivo=0;	// to the regresive counter

	this->HeadRotX=0;			// Rotation for HeadTracking on X
	this->HeadRotY=0;			// Rotation for HeadTracking on Y
	this->HeadRotZ=0;			// Rotation for HeadTracking on Z

	this->start = 0;
	this->framenum = 1;

	this->wandRay  =false;		// define if the wand ray is on or off
	this->wandClick=false;		// define if the wand click is on or off
	this->opening  =60;			// define how many degres the fingers of the robot hand wand have
	
	this->move=0;				// case right mouse button clicked get 1, middle -1, none 0
	this->motionX=0;			// reseting the variable that define the X position of the mouse on the screen
	this->motionY=0;			// reseting the variable that define the Y position of the mouse on the screen

	this->fps = new char[128];
	if(this->fps==NULL) cerr << "Memory alocation problem at char value" << endl;
	strcpy(this->fps,"- FPS");

	#ifdef DEBUG
	this->myMPI_Time=0;			// to calculate how many time the MPI routine use
	#endif
	
 	info();						// generate information about the opengl drivers
	quality();					// sets the quality of some opengl features
	
	
	this->eye = NULL;				// creates the eye
	this->lookat = NULL;		// creates the LookAt
	this->viewport = NULL;	// creates the Viewport
	
	
	tex = new Tex();			// creating one texture object
		
	//this->cam=NULL;
	//if(this->cam==NULL) {
		//cerr << "Error creating camera view at render" << endl;
	//}
	
	
	this->posRender = new PosRender();
	
	MPI_Irecv(&this->posRender->render, 1, posRender->renderDatatype, MAIN, RENDER_MOVEMENT, MPI_COMM_WORLD, &this->request);
	
	MPI_Irecv(&this->groupOnOff, 1, MPI_INT, MPI_ANY_SOURCE, GROUP_ONOFF, MPI_COMM_WORLD, &this->request_group);
	
	Render::my_rrr = this;
}

/* Destructor */
Render::~Render() {
	delete this->graph_ranks;
	delete this->fps;
	delete this->tex;
	delete this->cam;
	delete this->posRender;
}


#ifdef GLEW

// Called at startup
void Render::initializeGLSL() {
	
    // Initialize the "OpenGL Extension Wrangler" library
	#ifdef DEBUG1
		cout << "Initializing GLEW" << endl;
	#endif
    glewInit();

    // Ensure we have the necessary OpenGL Shading Language extensions.
    if (glewGetExtension("GL_ARB_fragment_shader")      != GL_TRUE ||
        glewGetExtension("GL_ARB_vertex_shader")        != GL_TRUE ||
        glewGetExtension("GL_ARB_shader_objects")       != GL_TRUE ||
        glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
        cerr << "Driver does not support OpenGL Shading Language" << endl;
    }
	    
}

#endif



/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void Render::InitGL() {              // We call this right after our OpenGL window is created.

#ifdef GLEW	
	initializeGLSL();
#endif
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Clear the background with black
	glClearDepth(1.0);							// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);						// The type of depth test
	glEnable(GL_DEPTH_TEST);					// Enables depth testing
	glEnable(GL_NORMALIZE);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);			// 	
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	glShadeModel(GL_SMOOTH);				// The shading will be smooth
	
	sp_Init(); 
	sp_Set_Offsets(0.0,0.42,0.0);
	sp_Set_Aperture_EC(135.0,-0.38);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // important to glDrawPixels
	glPixelStorei(GL_PACK_ALIGNMENT, 1); // important to glReadPixels
	
	x3d::my_x3d->createLetters(); // opengl needs to be started to run this
	
}



// desenha tela
void Render::drawFrameBuffer() {

	#ifdef DEBUG2
		cout << "\t\tdrawing Frame Buffer" << endl;  
	#endif
	
	/*
	stereo=0  -> no stereo
	stereo=1  -> active stereo
	stereo=2  -> right eye
	stereo=3  -> left eye
	*/
	
	/*
	
	Para o futuro
	
	Two first bits:
	stereo=0  -> no stereo
	stereo=1  -> right eye
	stereo=2  -> left eye
	stereo=3  -> active stereo
	stereo=4  -> auto stereo
	
	
	atual
	stereo=0  -> no stereo
	stereo=1  -> active
	stereo=2  -> right eye
	stereo=3  -> left stereo
	stereo=4  -> auto stereo
	
	
	Third and fourth bits
	stereo=0..  -> planar projection
	stereo=1..  -> Horizonta curved projection
	stereo=2..  -> Vertical curved projection
	stereo=3..  -> spherical projection
	
	For example:
	
	00000 = planar projection without stereo
	11000 = planar projection with stereo
	00011 = spherical projection without stereo
	
	*/
		
	int window;  // for counter
	
	#ifdef DEBUG1
		cout << "\tPreparing to Draw in the Frame Buffer" << endl;	// Message about debuging
		cout << "Stereo system in : " << this->lll->conf->getStereoGlobal() << endl;
	#endif


	this->calcPerf();

	
	if(this->lll->conf->getStereoGlobal() > 0) { 				// In the case of stereo flag on

//		cout << " TA EM ESTEREO " << endl;
		
		if (this->lll->conf->getStereoGlobal()==1) 				// In the case of active stereo
		{
			glDrawBuffer(GL_BACK_RIGHT);						// Set the operations in the right buffer
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		} 
		else													// In the case of pasive stereo 
		{	
			glDrawBuffer(GL_BACK_LEFT); 						// Set the operations in the right buffer
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		}
	
				
		// Possible place for OpenMP
		for(window=0; window < this->lll->conf->window; window++ ) 
		{
			if (this->lll->conf->getStereo(window)!=3) {
				#ifdef DEBUG1
					cout << "\tDrawing right eye" << endl;	// Message about debuging
				#endif
				this->execute(2,window);
				if( (window==0 || (this->lll->conf->getStereo(window)==2)) && (Engine::my_Engine->my_rank==(Engine::my_Engine->procesSize-1)) ) {
					
					#ifdef DEBUG2
						cout << "Drawing Auxiliary information" << endl;	// Message about debuging
					#endif
					AuxiliaryInformation(); 	// information like fps, clock, etc...

					Logo();				// Jinx Logo
				}
			}
		}
		if ( (this->lll->conf->getStereoGlobal()==4) ) {
			glReadPixels(0,0,lll->conf->getWidth(0),lll->conf->getHeight(0),GL_RGB,GL_UNSIGNED_BYTE,this->rightImage);
		}
		
		
		
		
		// Muito estranho testar novamente
		if ( (this->lll->conf->getStereoGlobal()==1) || (this->lll->conf->getStereoGlobal()==3) || (this->lll->conf->getStereoGlobal()==4) ) 
		{
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
		}
		
		// Possible place for OpenMP
		for(int window=0; window < this->lll->conf->window; window++ ) 
		{
			if (this->lll->conf->getStereo(window)!=2) 
			{
				#ifdef DEBUG1
					cout << "\tDrawing left eye" << endl;	// Message about debuging
				#endif
				
			
				this->execute(3,window);
				

				if( ((window==0) || (this->lll->conf->getStereo(window)==3)) && (Engine::my_Engine->my_rank==(Engine::my_Engine->procesSize-1)) ) {

					#ifdef DEBUG2
						cout << "Drawing Auxiliary information" << endl;	// Message about debuging
					#endif
					AuxiliaryInformation(); 	// information like fps, clock, etc...

					Logo();
				}
			}
		} 
		if ( (this->lll->conf->getStereoGlobal()==4) ) {
			glReadPixels(0,0,lll->conf->getWidth(0),lll->conf->getHeight(0),GL_RGB,GL_UNSIGNED_BYTE,this->leftImage);
		}
		
		
		if( this->lll->conf->getStereoGlobal()==4 ) {
			
			int i,j,even;

			#ifdef DEBUG1
				cout << "\tDrawing auto estereo eye" << endl;	// Message about debuging
			#endif
	
 			for(i = 0; i < lll->conf->getHeight(0); i++) {
				for(j = 0,even = 0; j < lll->conf->getWidth(0)*3; j+=3,even++) {
					if(even%2) {
						autoStereoImage[i*lll->conf->getWidth(0)*3+j]   = leftImage[i*lll->conf->getWidth(0)*3+j];
						autoStereoImage[i*lll->conf->getWidth(0)*3+j+1] = rightImage[i*lll->conf->getWidth(0)*3+j+1];
						autoStereoImage[i*lll->conf->getWidth(0)*3+j+2] = leftImage[i*lll->conf->getWidth(0)*3+j+2];
					} else {
						autoStereoImage[i*lll->conf->getWidth(0)*3+j]   = rightImage[i*lll->conf->getWidth(0)*3+j];
						autoStereoImage[i*lll->conf->getWidth(0)*3+j+1] = leftImage[i*lll->conf->getWidth(0)*3+j+1];
						autoStereoImage[i*lll->conf->getWidth(0)*3+j+2] = rightImage[i*lll->conf->getWidth(0)*3+j+2];
					}

				}
			}

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, 1.0, 0.1, 10);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear( GL_COLOR_BUFFER_BIT );
			glDepthFunc(GL_ALWAYS);                // this make everthing appear without carring the zbuffer
			glDisable(GL_LIGHTING);                //
			glTranslatef(0,0,-1.0);                 // to not display over the camera
			glRasterPos2i(-1,-1); // there is a linear proportion betwen these values
			glDrawPixels(lll->conf->getWidth(0),lll->conf->getHeight(0),GL_RGB,GL_UNSIGNED_BYTE,autoStereoImage);
			glDepthFunc(GL_LESS);                // this make everthing appear without carring the zbuffer
			glEnable(GL_LIGHTING);                //
		}
		
		
		
	}
	
	else { //* in the case of no stereo flag
	
		#ifdef DEBUG3
			cout << "No stereo flag" << endl;	
		#endif
		
		glDrawBuffer(GL_BACK_LEFT);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
		
		// Possible place for OpenMP
		
		window=0;
		do {
			#ifdef DEBUG1
				cout << "\tDrawing middle eye" << endl;	// Message about debuging
			#endif
			this->execute(0,window);
			if(window==0 && (Engine::my_Engine->my_rank==(Engine::my_Engine->procesSize-1))) {

				#ifdef DEBUG2
					cout << "Drawing Auxiliary information" << endl;	// Message about debuging
				#endif
				AuxiliaryInformation(); 	// information like fps, clock, etc...

				Logo();
			}
			window++;  //means no windows defined or computer not found in list
		} while(window < this->lll->conf->window);
		
	}
	
	
}

void Render::selectFrameBuffer(int window, int mode) {
	
	if ((this->lll->conf->getStereoGlobal()==1) && (mode==2)) 				// In the case of active stereo
	{
		glDrawBuffer(GL_BACK_RIGHT);						// Set the operations in the right buffer
	}
	else 
	{
		glDrawBuffer(GL_BACK_LEFT);							// Set the operations in the right buffer	
	}
		
	
}

void Render::execute(int mode, int window) {
		
	#ifdef DEBUG2
		cout << "Executing Graphic system" << endl;  
	#endif
	
	float eyeSeparation=0;
	
	#ifdef DEBUG2	
		cout << " Eye Separation = " << this->eye_separation << endl;
	#endif
	
	if(mode==3) {
		
		eyeSeparation = + this->eye_separation;
		
	} 
	else if(mode==2) 
	{
		
		eyeSeparation = - this->eye_separation;
		
	} 
	
		
	
	
	
	#ifdef DEBUG1
		cout << "\t\t" << "Projection " << window << " : " << this->lll->conf->getProjectionView(window) << endl;
	#endif
	
	
	
	
	if(this->lll->conf->getProjectionView(window)!=3)
	{

		#ifdef DEBUG1
			cout << "\t\tSetting Camera" << endl;	// Message about debuging
		#endif
		this->setCamera(window);
		
		
		// Important for dist eye, then stereo
		#ifdef DEBUG1
			cout << "\t\tSetting Head Tracking (Position)" << endl;	// Message about debuging
		#endif
		this->headTracking();			// calculate the head tracking change for values in HMD
		
		
		
		
		#ifdef DEBUG1
			cout << "\t\tGetting Screen Parameter" << endl;	// Message about debuging
		#endif
		this->getScreenParameter(this->ScreenConners,&this->ScreenPlanarDepth,window);
	
		#ifdef DEBUG1
			cout << "\t\tCalcing projection" << endl;	// Message about debuging
		#endif
		this->calcDistEye(this->distEye,eyeSeparation);
		this->calcViewport(this->viewport,window);				
		this->calcFruntum(this->eye,this->ScreenConners,this->headPosition,this->distEye,this->ScreenPlanarDepth);
		this->calcLookAt(this->lookat,this->headPosition,this->distEye,this->cameraDir,this->cameraUp);
		this->selectFrameBuffer(window,mode);
		this->finishCamera(this->viewport,this->eye,this->lookat); // Set the glFrustum and gluLookAt
		
	
		
		
		#ifdef DEBUG1
			cout << "\t\tDrawing in the viewport" << endl;	// Message about debuging
		#endif
		
		// This routine rotate the object, in other words rotate by the center of the object
		bool rotateObject=false;
		if(rotateObject) {
			static float rx=0;
			static float ry=0;
			static float rz=0;
			glTranslatef(0.0,0.0,-20.0);
			glRotatef(rx-=(my_rrr->HeadRotX*0.1),1.0,0.0,0.0);
			glRotatef(ry-=(my_rrr->HeadRotY*0.1),0.0,1.0,0.0);
			glRotatef(rz+=(my_rrr->HeadRotZ*0.1),0.0,0.0,1.0);
			glTranslatef(0.0,0.0,20.0);
		}
		
		this->draw(mode);
	
	} else { // spherical routine
		
		// Capture the views
		for(int i=0; i < 5; i++) {

			sp_CaptureBegin(i,eyeSeparation);

			this->draw(mode);
			
			sp_CaptureEnd(i);

		}

		#ifdef DEBUG1
			cout << "\t\tDrawing in the Spherical viewport" << endl;	// Message about debuging
		#endif
		this->selectFrameBuffer(window,mode);
		sp_draw(this->lll->conf->getWidth(window),this->lll->conf->getHeight(window)); //Draw The Fisheye View
		
	}
	
	#ifdef DEBUG1
		cout << "\t\tFinishing the projection" << endl;	// Message about debuging
	#endif
}

void Render::setCamera(int window) {
	
	this->cameraDir[0]=this->lll->conf->getDirX(window);	// Camera dir
	this->cameraDir[1]=this->lll->conf->getDirY(window);	// Camera dir
	this->cameraDir[2]=this->lll->conf->getDirZ(window);	// Camera dir
		
	this->cameraUp[0]=this->lll->conf->getUpX(window);		// Camera up value
	this->cameraUp[1]=this->lll->conf->getUpY(window);		// Camera up value
	this->cameraUp[2]=this->lll->conf->getUpZ(window);		// Camera up value
		
	#ifdef DEBUG
	cout << "Direction : " <<
		this->cameraDir[0] << " , " << 
		this->cameraDir[1] << " , " << 
		this->cameraDir[2] << endl;
	cout << "Up : " << 
	 	this->cameraUp[0] << " , " <<
		this->cameraUp[1] << " , " << 
		this->cameraUp[2] << endl;
	#endif
	
}


void Render::getScreenParameter(double *ScreenConners, double *ScreenPlanarDepth, int window) 
{
	
	ScreenConners[0]=this->lll->conf->getLeft(window);  	 // position of screen coner
	ScreenConners[1]=this->lll->conf->getRight(window);		 // position of screen coner
	ScreenConners[2]=this->lll->conf->getBottom(window);	 // position of screen coner
	ScreenConners[3]=this->lll->conf->getTop(window);		 // position of screen coner
	
	(*ScreenPlanarDepth)=this->lll->conf->getPlanarDepth(window);// position of screen distance
		
	#ifdef DEBUG
	cout << "Coners: " <<
		"Left  : " << ScreenConners[0] << " , " << 
		"Right : " << ScreenConners[1] << " , " << 
		"Bottom: " << ScreenConners[2] << " , " << 
		"Top   : " << ScreenConners[3] << " , " << 
		endl <<
		"PlanarDepth: " << (*ScreenPlanarDepth) << " , " << 
		endl;
	#endif
	
	
}


void Render::calcViewport(Viewport *viewport, int window) {
	
	if(this->lll->conf->getWindow()-1) {  // just do it if more than one window
		viewport->posx = this->lll->conf->getPositionX(window);
		viewport->posy = this->lll->conf->getPositionY(window);
		viewport->width = this->lll->conf->getWidth(window);
		viewport->height = this->lll->conf->getHeight(window);
		#ifdef DEBUG
		cout << "Viewport (" << window << ") PosX  : " << viewport->posx   << endl;
		cout << "Viewport (" << window << ") PosY  : " << viewport->posy   << endl;
		cout << "Viewport (" << window << ") width : " << viewport->width  << endl;
		cout << "Viewport (" << window << ") height: " << viewport->height << endl;
		#endif
	} else {
		viewport->posx = -1;
		viewport->posy = -1;
		viewport->width = -1;
		viewport->height = -1;
	}
	
}

void Render::calcDistEye(float *distEye,float eye_separation) {
	
	distEye[0] *= eye_separation/2;
	distEye[1] *= eye_separation/2;
	distEye[2] *= eye_separation/2;
	
}


void Render::calcFruntum(Eye *eye, double *ScreenConners, double *headPosition, float *distEye, float PlanarDepth) {
	
	
	/* PlanaDeapth is the distance betwen the plan that contains the screen and 
	   the viewer. You must calculate the perpendicular distance */
	
	eye->frustum_near = FRUSTUM_NEAR;		
	
	eye->frustum_far  = FRUSTUM_FAR;			
	
	eye->frustum_left = (
	 ((ScreenConners[0])-(headPosition[0])+(distEye[0])) / 
	 ((PlanarDepth)+(headPosition[2])) ) * eye->frustum_near;
				 
	eye->frustum_right = (
	 ((ScreenConners[1])-(headPosition[0])+(distEye[0])) /
	 ((PlanarDepth)+(headPosition[2])) ) * eye->frustum_near;
				
	eye->frustum_bottom = ( 
	 ((ScreenConners[2])-(headPosition[1])+(distEye[1])) /
	 ((PlanarDepth)+(headPosition[2])) ) * eye->frustum_near;

	eye->frustum_top = ( 
	 ((ScreenConners[3])-(headPosition[1])+(distEye[1])) /
	 ((PlanarDepth)+(headPosition[2])) ) * eye->frustum_near;

	
	
	#ifdef DEBUG
	cout << "Frustum : " << " (LEFT) " << eye->frustum_left <<
							" (RIGHT) " << eye->frustum_right <<
							" (BOTTOM) " << eye->frustum_bottom <<
							" (TOP) " << eye->frustum_top <<
							" (NEAR) " << eye->frustum_near <<
							" (FAR) " << eye->frustum_far << endl;
	#endif
	
}

void Render::calcLookAt(LookAt *lookat, double *headPosition, float *distEye, float *cameradDir, float *cameraUP) {
	
	// Where the eye is
	lookat->frustum_eyex=headPosition[0]+this->distEye[0];
	lookat->frustum_eyey=headPosition[1]+this->distEye[1];
	lookat->frustum_eyez=headPosition[2]+this->distEye[2];
				
	// Where the eye is looking at
	lookat->frustum_centrex=headPosition[0]+this->distEye[0]+this->cameraDir[0];
	lookat->frustum_centrey=headPosition[1]+this->distEye[1]+this->cameraDir[1];
	lookat->frustum_centrez=headPosition[2]+this->distEye[2]+this->cameraDir[2];
			
	// The up vector
	lookat->frustum_upx=this->cameraUp[0];
	lookat->frustum_upy=this->cameraUp[1];
	lookat->frustum_upz=this->cameraUp[2];
	
	#ifdef DEBUG
	cout << "Look At : " << endl;
	cout << " EYE    " << lookat->frustum_eyex << " , " << lookat->frustum_eyey << " , " << lookat->frustum_eyez << endl;
	cout << " CENTER " << lookat->frustum_centrex << " , " << lookat->frustum_centrey << " , " << lookat->frustum_centrez << endl;
	cout << " UP     " << lookat->frustum_upx << " , " << lookat->frustum_upy << " , " << lookat->frustum_upz << endl;
	#endif
	
}

 
void Render::finishCamera(Viewport *viewport, Eye *eye, LookAt *lookat) {
	
	// Defining for Viewport
	if(viewport->posx>=0) {
		glViewport(viewport->posx,viewport->posy,viewport->width,viewport->height);
	}
	
	// Defining for Frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(eye->frustum_left,eye->frustum_right,
			  eye->frustum_bottom,eye->frustum_top,
			  eye->frustum_near,eye->frustum_far);

	// Defining for lookAt
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lookat->frustum_eyex,lookat->frustum_eyey,lookat->frustum_eyez,
			  lookat->frustum_centrex,lookat->frustum_centrey,lookat->frustum_centrez,
			  lookat->frustum_upx,lookat->frustum_upy,lookat->frustum_upz);
	
}


Viewport::Viewport() {
	
}


Viewport::~Viewport() {
	
}

LookAt::LookAt() {
	
}


LookAt::~LookAt() {
	
}
 
Eye::Eye() {

	frustum_left=0;
	frustum_right=0;
	frustum_top=0;
	frustum_bottom=0;
	frustum_near=0;// The near clipping of the frustum
	frustum_far=0;// The far clipping of the frustum
	
	
}

Eye::~Eye() {
}
