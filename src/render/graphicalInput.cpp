
#include "render.h"

/* This code is used to display the hotkeys in the user console */
void helpMessage() {
	cout << " -- HELP MENU -- " << endl << endl;
	cout << 'f' << " Shows framerate" << endl;
	cout << 'g' << " Shows grid on floor" << endl;
	cout << 'i' << " invertEyes" << endl;
	cout << 'l' << " Calibration screen" << endl;
	cout << 'j' << " Joystick Mode" << endl;
	cout << 'm' << " Graphical Menu " << endl;
	cout << 'o' << " Show clock" << endl;
	cout << 'r' << " resets" << endl;
	cout << 's' << " Solid view" << endl;
	cout << 'v' << " Shows camera" << endl;
	cout << 'w' << " wireframe" << endl;
}

/* The function called whenever a key is pressed. */
void Render::keyPressed(unsigned char key, int x, int y) {
	
	if(x) {}
	if(y) {}
	
	float data[6] = {0,0,0,0,0,0};

	if(my_rrr->calibration!=true) {
	
		if(my_rrr->sendInput) {
			switch (key) {
			case ESCAPE: // If escape is pressed, kill everything
				cout << "Exiting by ESC key" << endl;
				glutDestroyWindow(my_rrr->window); // shut down our window
				MPI_Abort(MPI_COMM_WORLD,0);
				exit(0);
				break;
			case '8':
				data[2]=0.2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '2':
					data[2]=-0.2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case '7':
				data[3]=-2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '9':
				data[3]=2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '4':
					data[4]=2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case '6':
					data[4]=-2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case '1':
					data[2]=-1;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case '3':
					data[2]=1;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case '+':
				data[0]=1;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '-':
				data[0]=-1;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '/':
				data[1]=-1;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case '*':
				data[1]=1;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case 'c':
			case 'C':
				break;
			case 'f':
				(my_rrr->framerate?my_rrr->framerate=false:my_rrr->framerate=true);
				break;
			case 'g':
			case 'G':
				(my_rrr->grid?my_rrr->grid=false:my_rrr->grid=true);
				cout << "PASSEI " << my_rrr->grid << endl;
				break;
			case 'h':
			case 'H':
				helpMessage();
				break;
			case 'i':
				(my_rrr->invertEyes?my_rrr->invertEyes=false:my_rrr->invertEyes=true);
				break;
			case 'j':
			case 'J':
				(my_rrr->joystickMode?my_rrr->joystickMode=0:my_rrr->joystickMode=1);
				break;
			case 'l':
				(my_rrr->calibration?my_rrr->calibration=false:my_rrr->calibration=true);
				break;
			case 'm':
				(my_rrr->gmenu?my_rrr->gmenu=false:my_rrr->gmenu=true);
				break;
			case 'o':
				(my_rrr->gclock?my_rrr->gclock=false:my_rrr->gclock=true);
				break;
			case 'r':
			case 'R':
				my_rrr->regressivo=my_rrr->posRender->render.seconds;
				Input::setViewpoint(0);  // to viepoint default
				break;
			case 's':
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case 'v':
				(my_rrr->camera?my_rrr->camera=false:my_rrr->camera=true);
				break;
			case 'w':
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case 'W':
				glPolygonMode(GL_BACK,GL_LINE);
				break;
			case 'S':
				glPolygonMode(GL_BACK,GL_FILL);
				break;
			default:
				break;	
    		}
		
		}
	} else {
		switch (key) {
			case ESCAPE: // If escape is pressed, kill everything
				cout << "Exiting by ESC key" << endl;
				glutDestroyWindow(my_rrr->window); // shut down our window
				MPI_Abort(MPI_COMM_WORLD,0);
				exit(0);
				break;
			case '8':
				break;
			case '2':
				my_rrr->calibrationDivisions=20;
				break;
			case '7':
				(my_rrr->degrade?my_rrr->degrade=false:my_rrr->degrade=true);
				my_rrr->calibrationDegradeDirection=4;
				break;
			case '9':
				my_rrr->calibrationClearColor[0]=0;
				my_rrr->calibrationClearColor[1]=0;
				my_rrr->calibrationClearColor[2]=1;
				break;
			case '4':
				my_rrr->calibrationDivisions=12;
				break;
			case '6':
				(my_rrr->degrade?my_rrr->degrade=false:my_rrr->degrade=true);
				my_rrr->calibrationDegradeDirection=0;
			break;
			case 'f':
				(my_rrr->frontBackColor?my_rrr->frontBackColor=false:my_rrr->frontBackColor=true);
				break;
			case '1':
				my_rrr->calibrationDivisions=10;
				break;
			case '3':
				my_rrr->calibrationDivisions=30;
				break;
			case '5':
				my_rrr->calibrationDivisions=15;
				break;
			case '0':
				break;
			case 'l':
				(my_rrr->calibration?my_rrr->calibration=false:my_rrr->calibration=true);
				break;
			case 'g':
			case 'G':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColor[0]=0;
					my_rrr->calibrationFrontColor[1]=1;
					my_rrr->calibrationFrontColor[2]=0;
					cout << "Front color : green" << endl;
				} else {
					my_rrr->calibrationClearColor[0]=0;
					my_rrr->calibrationClearColor[1]=1;
					my_rrr->calibrationClearColor[2]=0;
					cout << "Back color : green" << endl;
				}
			break;
			case 'h':
			case 'H':
				helpMessage();
				break;
			case 'r':
			case 'R':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColor[0]=1;
					my_rrr->calibrationFrontColor[1]=0;
					my_rrr->calibrationFrontColor[2]=0;
					cout << "Front color : red" << endl;
				} else {
					my_rrr->calibrationClearColor[0]=1;
					my_rrr->calibrationClearColor[1]=0;
					my_rrr->calibrationClearColor[2]=0;
					cout << "Back color : red" << endl;
				}
				break;
			case 'b':
			case 'B':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColor[0]=0;
					my_rrr->calibrationFrontColor[1]=0;
					my_rrr->calibrationFrontColor[2]=1;
					cout << "Front color : blue" << endl;
				} else {
					my_rrr->calibrationClearColor[0]=0;
					my_rrr->calibrationClearColor[1]=0;
					my_rrr->calibrationClearColor[2]=1;
					cout << "Back color : blue" << endl;
				}
				break;
			case 'w':
			case 'W':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColor[0]=1;
					my_rrr->calibrationFrontColor[1]=1;
					my_rrr->calibrationFrontColor[2]=1;
					cout << "Front color : white" << endl;
				} else {
					my_rrr->calibrationClearColor[0]=1;
					my_rrr->calibrationClearColor[1]=1;
					my_rrr->calibrationClearColor[2]=1;
					cout << "Back color : white" << endl;
				}
			break;
			case 'd':
			case 'D':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColor[0]=0;
					my_rrr->calibrationFrontColor[1]=0;
					my_rrr->calibrationFrontColor[2]=0;
					cout << "Front color : black" << endl;
				} else {
					my_rrr->calibrationClearColor[0]=0;
					my_rrr->calibrationClearColor[1]=0;
					my_rrr->calibrationClearColor[2]=0;
					cout << "Back color : black" << endl;
				}
				break;
			case '_':
			case '-':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColorPercent-=10;
					cout << "Percentage of Front Color : " << my_rrr->calibrationFrontColorPercent << endl;
				} else {
					my_rrr->calibrationClearColorPercent-=10;
					cout << "Percentage of Back Color : " << my_rrr->calibrationClearColorPercent << endl;
				}
				break;
			case '=':
			case '+':
				if(my_rrr->frontBackColor) {
					my_rrr->calibrationFrontColorPercent+=10;
					cout << "Percentage of Front Color : " << my_rrr->calibrationFrontColorPercent << endl;
				} else {
					my_rrr->calibrationClearColorPercent+=10;
					cout << "Percentage of Back Color : " << my_rrr->calibrationClearColorPercent << endl;
				}
				break;
			case '.':
			case '>':
				my_rrr->calibrationPatern++;
				cout << "Calibration Patern" << endl;
				break;
			case ',':
			case '<':
				my_rrr->calibrationPatern--;
				cout << "Calibration Patern" << endl;
				break;			
			case ']':
			case '}':
				my_rrr->calibrationLineWidth++;
				cout << "Calibration Line Width" << endl;
				break;
			case '[':
			case '{':
				my_rrr->calibrationLineWidth--;
				cout << "Calibration Line Width" << endl;
				break;
			case 'c':
			case 'C':
				(my_rrr->calibrationColorBars?my_rrr->calibrationColorBars=false:my_rrr->calibrationColorBars=true);
				cout << "Calibration Color Bars" << endl;
				break;
			default:
				break;	
    	}
		
	}
	
}

/* The function called whenever a key is pressed. */
void Render::Special(int key, int x, int y) {

	if(x) {}
	if(y) {}
	
		
	float data[6] = {0,0,0,0,0,0};


	if(my_rrr->calibration!=true) {
	
		if(my_rrr->sendInput) {
			switch (key) {
			case GLUT_KEY_UP:
					data[2]=0.2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case GLUT_KEY_DOWN:
					data[2]=-0.2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case GLUT_KEY_HOME:
				data[3]=-2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case GLUT_KEY_PAGE_UP:
				data[3]=2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case GLUT_KEY_LEFT:
					data[4]=2;
					Input::move(data);
					my_rrr->sendInput=0;
				break;
			case GLUT_KEY_RIGHT:
					data[4]=-2;
					Input::move(data);
					my_rrr->sendInput=3;
				break;
			case GLUT_KEY_END:
				data[0]=-0.2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
			case GLUT_KEY_PAGE_DOWN:
				data[0]=0.2;
				Input::move(data);
				my_rrr->sendInput=0;
				break;
		
			default:
				break;	
			
    	}
		}
	} else {
		
		switch (key) {
			case GLUT_KEY_UP:
					my_rrr->calibrationDegradeDirection=0;
				break;
			case GLUT_KEY_DOWN:
					my_rrr->calibrationDegradeDirection=2;
				break;
			case GLUT_KEY_LEFT:
					my_rrr->calibrationDegradeDirection=3;
				break;
			case GLUT_KEY_RIGHT:
					my_rrr->calibrationDegradeDirection=1;
				break;
			default:
				break;	
			
    	}
	}
}

void Render::Mouse(int button, int state, int x, int y) {
	
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_HEIGHT)/2);
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		my_rrr->move=1;
		my_rrr->motionX=0;
		my_rrr->motionY=0;
	}
		
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		my_rrr->move=0;
	}
		
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		my_rrr->move=-1;
	}
		
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		my_rrr->move=0;
    }
		
	/* this is used by the menu
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	}
		
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
	}
	*/
	
}

/* Mouse motion function */
void Render::Motion(int x, int y) {
    	
	my_rrr->motionX = (glutGet(GLUT_WINDOW_WIDTH)/2)-x;
	my_rrr->motionY = (glutGet(GLUT_WINDOW_HEIGHT)/2)-y;

	if(my_rrr->joystickMode==1) { // joystick mode 0
		float data[6] = {0,0,0,0,0,0};
		if(my_rrr->sendInput) {
		   	if(my_rrr->move>0) {
				data[2] = ((float)(my_rrr->motionY)) / 30;
				data[4] = ((float)(my_rrr->motionX)) / 60;
				Input::move(data);
				my_rrr->motionY=0;
				my_rrr->motionX=0;
				my_rrr->sendInput=0;
				glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_HEIGHT)/2);
			}
		}
	}
	
}


/* Idle function */
void Render::Idle() {

}
