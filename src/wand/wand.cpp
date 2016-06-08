
#include "wand.h"
#include "../communication.h"

#include <iostream>
	
#define ESC 0X1B

using namespace std;

/* Constructor */
Wand::Wand() {

	MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);

}

/* Destructor */
Wand::~Wand() {
}


void Wand::readWand(int type) {

	cout << " Wand : ";
				
	if(type==1) {
		cout << " Space Ball " << endl;
		this->readspaceball();
	} else if(type==2) {
		cout << " Flock of Birds " << endl;
		this->readtracker();
	} else if(type==3) {
		cout << " Intertrax " << endl;
		this->readheadtracking();
	} else if(type==4) {
		cout << " Glove " << endl;
		this->readglove();
	} else {
		cout << " Not defined " << endl;
		this->readspaceball();
	}

}
