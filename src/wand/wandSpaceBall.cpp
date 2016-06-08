
#include "wand.h"

#include "../sball.h"
#include "../communication.h"

#include "../general.h"

#include <iostream>

using namespace std;

/* Just to read space ball */
void Wand::readspaceball() {
	
	
	
	char sballname[256]; 
	SBallHandle sball;
	int nullsize;
	int done = 0;

	
	MPI_Status status;
	int received;
	
	posWand[0] = new PosWand(); 
	
	//strcpy(sballname,"/dev/ttyS1");
	
	strcpy(sballname,"/dev/ttyS0");
	
	#ifdef DEBUG 
	cout << "Opening Spaceball: using port " << sballname << " for I/O\n" << endl;
	#endif

	int tx;		// translation x
	int ty;		// translation y
	int tz;		// translation z
	int rx;		// rotation x
	int ry;		// rotation y
	int rz;		// rotation z
	int buttons;	// buttons
	//int onoff = 1;
	
	int ftx;		// translation x
	int fty;		// translation y
	int ftz;		// translation z
	int frx;		// rotation x
	int fry;		// rotation y
	int frz;		// rotation z
	int fbuttons;	// buttons
	int fonoff;

	struct timespec start,stop;
	
	
	// Position that will be sent, it is zero to avoid sending trash
	
	tx = 0;		// translation x
	ty = 0;		// translation y
	tz = 0;		// translation z
	rx = 0;		// rotation x
	ry = 0;		// rotation y
	rz = 0;		// rotation z
	buttons = 0;	// buttons
	//onoff = 1;
	
	ftx = 0;		// translation x
	fty = 0;		// translation y
	ftz = 0;		// translation z
	frx = 0;		// rotation x
	fry = 0;		// rotation y
	frz = 0;		// rotation z
	fbuttons = 0;	// buttons
	fonoff = 1;
	
	
	unsigned short int counter;
	counter=0;
	
	while ((sball = sball_open(sballname)) == NULL) {
	
		if(counter==0) {
			cerr << "Not opening Spaceball port" << endl;
			counter=100;
		} else {
			counter--;
		}
		
		
		received=1;
		MPI_Test(&request, &received, &status);
		if(received) {
			MPI_Wait(&request, &status);
			MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);

			posWand[0]->wand.trans[0]=(((float)(ftx))/400);
			posWand[0]->wand.trans[1]=(((float)(fty))/400)-1;
			posWand[0]->wand.trans[2]=(-((float)(ftz))/400)-2;
			posWand[0]->wand.rotat[0]=(((float)(frx))/6);
			posWand[0]->wand.rotat[1]=(((float)(fry))/6)+180;
			posWand[0]->wand.rotat[2]=(((float)(frz))/6);
			
			posWand[0]->head.rotat[0]=(((float)(frx))/6);
			posWand[0]->head.rotat[1]=(((float)(fry))/6);
			posWand[0]->head.rotat[2]=(((float)(frz))/6);

			posWand[0]->wand.buttons=fbuttons;
			posWand[0]->wand.wonoff=0;
			posWand[0]->wand.lixo=fbuttons;
			
			MPI_Ssend(&posWand[0]->wand, 1, posWand[0]->wandDatatype, WAND, INPUT_WAND, MPI_COMM_WORLD);

		}
		
		
		breath();

	}
	
	{
		cout << "Spaceball port open";
		#ifdef DEBUG
			cout << " ID : " << sball ;
		#endif
		cout << endl;
	

	nullsize = 65;
	//printf("Setting default null-region to 65 on all 6 axes...\n");
	sball_set_nullregion(sball, nullsize, nullsize, nullsize, 
                          nullsize, nullsize, nullsize);
	
	if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		cerr << "clock gettime" << endl;
	}
	
	while (!done) {

		if (sball_getstatus(sball, &tx, &ty, &tz, &rx, &ry, &rz, &buttons)) {
			
			done = buttons & SBALL_BUTTON_12;  
			if (buttons & SBALL_BUTTON_2) {
				cout << "doing soft sball init." << endl;
				sball_init(sball);
			}
			
			if (buttons & SBALL_BUTTON_3) {
				cout << "Setting null-region to 0 on all 6 axes..." << endl;
				nullsize = 0;
				sball_set_nullregion(sball, nullsize, nullsize, nullsize, 
                                nullsize, nullsize, nullsize);
			}

			if (buttons & SBALL_BUTTON_4) {
				cout << "Setting null-region to 65 on all 6 axes..." << endl;
				nullsize = 65;
				sball_set_nullregion(sball, nullsize, nullsize, nullsize, 
                                nullsize, nullsize, nullsize);
			}

			if (buttons & SBALL_BUTTON_1) {
				cout << "BUTTON 1" << endl;
			}
			
			if (buttons & SBALL_BUTTON_2) {
				cout << "BUTTON 2" << endl;
			}
			
			if (buttons & SBALL_BUTTON_3) {
				cout << "BUTTON 3" << endl;
			}
			
			if (buttons & SBALL_BUTTON_4) {
				cout << "BUTTON 4" << endl;
			}
			
			if (buttons & SBALL_BUTTON_5) {
				cout << "BUTTON 5" << endl;
			}
			
			if (buttons & SBALL_BUTTON_6) {
				cout << "BUTTON 6" << endl;
			}
			
			if (buttons & SBALL_BUTTON_7) {
				cout << "BUTTON 7" << endl;
			}
			
			if (buttons & SBALL_BUTTON_8) {
				cout << "BUTTON 8" << endl;
			}
			
			if (buttons & SBALL_BUTTON_9) {
				cout << "BUTTON 9" << endl;
			}
			
			if (buttons & SBALL_BUTTON_10) {
				cout << "BUTTON A" << endl;
			}
			
			if (buttons & SBALL_BUTTON_11) {
				cout << "BUTTON B" << endl;
			}
			
			if (buttons & SBALL_BUTTON_12) {
				cout << "BUTTON C" << endl;
			}
			
			if (buttons & SBALL_MODE_LEFTY) {
				#ifdef DEBUG
					cout << "The SpaceBall is configured for Left Hand" << endl ;
				#endif
			}
			
			#ifdef DEBUG
				cout << " Translate X " << tx << "  Rotate X " << rx << endl;
				cout << " Translate Y " << ty << "  Rotate Y " << ry << endl;
				cout << " Translate Z " << tz << "  Rotate Z " << rz << endl;
				cout << " Buttons" << buttons << endl;
			#endif
	
			ftx=tx;		fty=ty;		ftz=tz;
			frx=rx;		fry=ry;		frz=rz;
			fbuttons = buttons;
			
			if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
				cerr << "clock gettime" << endl;
			}
			
			fonoff=1;
			
		} else {

			if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
				cerr <<  "clock gettime" << endl;
			}

			if(stop.tv_sec - start.tv_sec > 20) {
				fonoff=0;
			}
			
		}
	
		received=1;
		MPI_Test(&request, &received, &status);
		if(received) {
			MPI_Wait(&request, &status);
			MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);

			posWand[0]->wand.trans[0]=(((float)(ftx))/400);
			posWand[0]->wand.trans[1]=(((float)(fty))/400)-1;
			posWand[0]->wand.trans[2]=(-((float)(ftz))/400)-2;
			posWand[0]->wand.rotat[0]=(((float)(frx))/6);
			posWand[0]->wand.rotat[1]=(((float)(fry))/6)+180;
			posWand[0]->wand.rotat[2]=(((float)(frz))/6);
			
			posWand[0]->head.rotat[0]=(((float)(frx))/6);
			posWand[0]->head.rotat[1]=(((float)(fry))/6);
			posWand[0]->head.rotat[2]=(((float)(frz))/6);

			
			posWand[0]->wand.buttons=fbuttons;
			posWand[0]->wand.wonoff=fonoff;
			posWand[0]->wand.lixo=fbuttons;
			
			MPI_Ssend(&posWand[0]->wand, 1, posWand[0]->wandDatatype, WAND, INPUT_WAND, MPI_COMM_WORLD);

		}
		
		breath();

	}
	
	} 

	#ifdef DEBUG
	cout << "Closing Spaceball connection" << endl;
	#endif
	
	sball_close(sball);

	delete posWand[0]; 
	
	cout << "Program Aborted by Wand" << endl;
  	MPI_Abort(MPI_COMM_WORLD,0);
	exit(0);
	
}
