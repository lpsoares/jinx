#include "joystick.h"

#include "general.h"
#include "config/xmlsax.h"

#include <iostream>
#include <string>

// Includes do MPI
#include <mpi.h>

#ifdef LINUX
	#include <linux/joystick.h>
#endif

/*
#ifdef LINUX
Joystick::Joystick(char *deviceName) {

	strcpy(this->name,""); // reseting the name of the joystick
	
	this->deviceName=deviceName;
	
	this->numberOfAxes=0;				//reseting the number of axes that the joystick will suport
	this->numberOfButtons=0;			//reseting the number of buttons that the joystick will suport
		
	speed=SPEED;				// initial speed used by the joystick
	
	this->sucess=false;			// sucess to connect with file descriptor
	
	if(!joyInit(this->deviceName)) {
		cout << "Not able to connect with joystick" << endl;
	}
}
#else
Joystick::Joystick(char *deviceName) {
}
#endif
*/

#ifdef LINUX
Joystick::Joystick(xml* temp_lll) {

	this->lll = temp_lll;		// the xml class that holds the configuration
	
	strcpy(this->name,""); // reseting the name of the joystick
	
	this->deviceName = new char[256];
	
	strcpy(this->deviceName,this->lll->conf->getDeviceName(0)); // gets the device name usualy it is /dev/js0
	#ifdef DEBUG2
		cout << "Device Name for Joystick : " << this->deviceName << endl;
	#endif
	
	this->numberOfAxes=0;				//reseting the number of axes that the joystick will suport
	this->numberOfButtons=0;			//reseting the number of buttons that the joystick will suport
		
	speed=SPEED;				// initial speed used by the joystick
	
	this->sucess=false;			// sucess to connect with file descriptor
	
	if(!joyInit(this->deviceName)) {
		cout << "Not able to connect with joystick" << endl;
	}
}
#else
Joystick::Joystick(xml* temp_lll) {
}
#endif






Joystick::~Joystick() {
	
	close(this->fd);			// clossing file descriptor
	
	delete[] this->axis;
	delete[] this->button;
	
}

#ifdef LINUX
int Joystick::joyInit(char *fileDescName) {
	
	#ifdef DEBUG
		cout << "Joystick Initialization" << endl;
	#endif
	
	char oldName[128];
	strcpy(oldName,this->name);
		
	if (!((this->fd = open(fileDescName, O_RDONLY | O_NONBLOCK)) < 0)) {
		
		version = 0x000800;
		strcpy(name,"Unknown");
		
		ioctl(fd, JSIOCGVERSION, &version);
		ioctl(fd, JSIOCGAXES, &numberOfAxes);
		ioctl(fd, JSIOCGBUTTONS, &numberOfButtons);
		ioctl(fd, JSIOCGNAME(128), this->name);
	
		if(strcmp(oldName,this->name)) {
			cout << "Joystick Name: " << this->name << endl;
		}
		
		this->mode=0;
		if(!strcmp("Microsoft SideWinder Force Feedback 2 Joystick",this->name)) { 
			mode=1; //force feedback
		} else 
		if(!strcmp("tonggong   USB Game Device ",this->name)) { 
			mode=2; //skate
		} else
		if(!strcmp("Microsoft® Microsoft® SideWinder® Plug & Play Game Pad",this->name)) {
		   mode=3; //gamepad
		} else
		if(!strcmp("Analog 2-axis 4-button joystick",this->name)) { 
			mode=4; //soundblaster
		} else
		if(!strcmp("Microsoft SideWinder Joystick",this->name)) { 
			mode=5; //
		} else 
		if(!strcmp("Microsoft® Microsoft® SideWinder® Game Pad USB",this->name) ) { 
			mode=6; //
		} else
		if(!strcmp("Analog 3-axis 4-button joystick",this->name) ) { 
			mode=7; //

		}
	
		if(!this->sucess) {
			cout << endl << "Detected Joystick : " << this->name << endl;
		}

		this->axis = new short int[numberOfAxes];
		this->button = new short int[numberOfButtons];

		for(int f=0;f<((int)this->numberOfAxes);f++) {
			axis[f]=0;
		}
	
		for(int g=0;g<((int)this->numberOfButtons);g++) {
			button[g]=0;
		}
	
		this->sucess=true;
		
		#ifdef DEBUG
			cout << "Joystick " << this->name << " has " << (int)this->numberOfAxes << 
				    " axes and " << (int)this->numberOfButtons << " buttons " << endl;
			if(mode!=0) cout << "Using specific configuration" << endl;
		#endif
	
	} else {
		return(0);
	}
		
	return(1);
	
}

#else

int Joystick::joyInit(char *fileDescName)
{
	
}

#endif


#ifdef LINUX

void Joystick::JoyRead() {
	
	struct js_event js;		// joystick structure
	MPI_Status status;
	int received;
	int input_update;
	MPI_Request request;	

	MPI_Irecv(&input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &request);
	received=1;
	
	int checkUpdate=0; // to check if joystick was removed
	
	while(true)	{

		if(this->sucess) {
		
    	    if (read(this->fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
				checkUpdate++;
				if(checkUpdate>800) {
					close(this->fd);
					if(!joyInit(this->deviceName)) {
						cout << "Connection lost with joystick" << endl;
						this->sucess=false;
					}
					checkUpdate=0;
				}
    	    }
    	    else {
				checkUpdate=0;
				switch (js.type) {
					case JS_EVENT_BUTTON: // Botoes controlam movimentacao
						button[((int)js.number)] = (short int)js.value;
						break;
					case JS_EVENT_AXIS:   // Axis controla direcao do olhar
						axis[((int)js.number)] = (short int)js.value;
						break;
				}
        	}
		
			MPI_Test(&request, &received, &status);
			if(received) {
				MPI_Wait(&request, &status);
				MPI_Irecv(&input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &request);
	
				this->data[0]=0;
				this->data[1]=0;
				this->data[2]=0;
				this->data[3]=0;
				this->data[4]=0;
				this->data[5]=0;


				if(this->lll->conf->getJoystick(0)) {
					cout << "Joystick defined in the configuration file" << endl;
					
					
					
				} else {
					//cout << "Joystick not defined in the configuration file" << endl;
				}
				
				switch(this->mode) {
				case 1: // force feedback
					if (button[2]==1) { this->data[1]= speed;}
					if (button[3]==1) { this->data[1]= -speed; } 
					if ( (!button[2]) && (!button[3])) { this->data[1]= 0; }
					this->data[4]= speed*((float)((int)(-axis[4])))/32767 ; // rotate in y
					this->data[2]= (speed*20*((float)((int)(-axis[1])))/327670) + (speed*((float)((int)(-axis[5])))/327670); // translate in z
					this->data[0]= speed*20*((float)((int)axis[0]))/327670 ; // translate in x
					this->data[3]= speed*((float)((int)axis[7]))/32767 ; // rotate in x
				break;

				case 2: //skate
					if (button[0]==1) { this->data[0]= -speed;} // translate to left
					if (button[2]==1) { this->data[0]= speed;} // translate to right
					if ( (!button[0]) && (!(int)button[2])) { this->data[0]= 0; } // not left and right
					if (button[1]==1) { this->data[1]= speed;} // translate down
					if (button[3]==1) { this->data[1]= -speed;} // translate up
					if ( (!button[1]) && (!(int)button[3])) { this->data[1]= 0; } // not up and down
					if (button[8]==1 || (((int)axis[1]) < 0 )) { this->data[2]= speed;} // left

					//if ((int)button[9]==1 || (((int)axis[1]) > 0 )){ this->data[2]= -0.1; } // fowrward
					if ( ((axis[1]) > 0 )){ this->data[2]= -speed; } // fowrward
					if ( (!button[8]) && (!(int)button[9]) && (((int)axis[1])==0) ) { this->data[2]= 0; } // not left and right
						
					this->data[4]= -((float)((int)axis[0]))/32767;
					break;

				case 3: //game pad
					if ( ((axis[1]) <  -1 )) { this->data[2]= speed;} // 
					if ( ((axis[1]) >  1 )) { this->data[2]= -speed;} // 
					if ( ((axis[0]) <  -1 )) { this->data[4]=+1.25+speed*0.12;} // 
					if ( ((axis[0]) >  1 )) { this->data[4]= -1.25-speed*0.12;} // 

					if ( button[0]==1 ) { this->data[1]= speed;} // 
					if ( button[1]==1 ) { this->data[0]= speed;} // 
					if ( button[2]==1 ) { this->data[0]= -speed;} // 
					if ( button[3]==1 ) { this->data[1]= -speed;} // 

					if (button[4]==1) { speed /= 1.04;} // 
					if (button[5]==1) { speed *= 1.04;} // 
					break;
											
				case 4: //soundblaster
					if(axis[0]<30000) this->data[4] = speed/3; else
					if(axis[0]<10000) this->data[4] = 0; else
					this->data[4] = -speed/3; 

					if(axis[1]<20000) this->data[1] = speed/3; else
					if(axis[1]<10000) this->data[1] = 0; else
					this->data[1] = -speed/3; 
					
					this->data[2]= 2*speed/3;
					break;

				case 5: // 
					if(axis[0]<30000) this->data[4] = speed/3; else
					if(axis[0]<10000) this->data[4] = 0; else
					this->data[4] = -speed/3; 

					if(axis[1]<20000) this->data[1] = speed/3; else
					if(axis[1]<10000) this->data[1] = 0; else
					this->data[1] = -speed/3; 

					this->data[2]= 2*speed/3;
					break;
					
					
				case 6: //game pad
					if ( ((axis[1]) <  -1 )) { this->data[2]= speed;} // 
					if ( ((axis[1]) >  1 )) { this->data[2]= -speed;} // 

					if ( ((axis[0]) <  -1 )) { this->data[4]=+1.25+speed*0.12;} // 
					if ( ((axis[0]) >  1 )) { this->data[4]= -1.25-speed*0.12;} // 

					if ( button[0]==1 ) { this->data[1]= speed;} // 
					if ( button[1]==1 ) { this->data[0]= speed;} // 
					if ( button[3]==1 ) { this->data[0]= -speed;} // 
					if ( button[4]==1 ) { this->data[1]= -speed;} // 

					if (button[6]==1) { speed /= 1.04;} // 
					if (button[7]==1) { speed *= 1.04;} // 
					break;
					
				case 7: //soundblaster
					if(axis[0]<-10000) this->data[4] = 1*speed; else
					//if(axis[0]<-100) this->data[4] = 0; else
					this->data[4] = -1*speed; 

					this->data[1]= 0.3*speed*((float)((int)(-axis[1])))/(32767) ; // rotate in y
					/*
					if(axis[1]<20000) this->data[1] = speed/3; else
					if(axis[1]<10000) this->data[1] = 0; else
					this->data[1] = -speed/3; 
					
					this->data[2]= 2*speed/3;
					
					*/
					break;

					
				default:
					if ( ((axis[1]) <  -1 )) { this->data[2]= speed;} // 
					if ( ((axis[1]) >  1 )) { this->data[2]= -speed;} // 

					if ( ((axis[0]) <  -1 )) { this->data[4]= 2;} // 
					if ( ((axis[0]) >  1 )) { this->data[4]= -2;} // 

					if ( button[0]==1 ) { this->data[1]= speed;} // 
					if ( button[1]==1 ) { this->data[0]= speed;} // 
					if ( button[2]==1 ) { this->data[0]= -speed;} // 
					if ( button[3]==1 ) { this->data[1]= -speed;} // 

					if (button[4]==1) { speed /= 1.1;} // 
					if (button[5]==1) { speed *= 1.1;} // 
					break;
			}

			Input::move(this->data);
					
		}
		
		#ifdef DEBUG
			unsigned short int f;
			for(f=0;f<(this->numberOfAxes);f++) 
				cout << "Axes " << f << " : " << axis[f] << endl;
			for(f=0;f<(this->numberOfButtons);f++) 
				cout << "Button " << f << " : " << button[f] << endl;
			for(f=0;f<6;f++)
				cout << "Data " << f << " : " << data[f] << endl; 
		#endif
			
	} else {
		
		// do nothing
		MPI_Test(&request, &received, &status);
		if(received) {
			MPI_Wait(&request, &status);
			MPI_Irecv(&input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &request);
			Input::move(this->data); //just for not locking but have to be fixed
		}
		
		joyInit(this->deviceName); // try to reconect
	}
	
	breath();	// just a sleep	

	}
	
}


#else


void Joystick::JoyRead() {

breath();	// just a sleep	
	
}

#endif
