
#ifndef __wand_h
#define __wand_h


// Includes MPI
#include <mpi.h>

//using namespace std;

// Struct to hold wand information
struct Wander {
	int wonoff;
	float lixo;
	float trans[3];
	float rotat[3];
	int buttons;
	float fingers[20];
	float quaternion[4];
	int type; // 0-Euler  1-quaternion
};

// Struct to hold head tracking information
struct Header {
	float rotat[3];	// head rotatio
};


class PosWand {
	
	public:
	
	PosWand();			// Constructor
	~PosWand() {};		// Destructor
	
	struct Wander wand;
	struct Header head;
	
	MPI_Datatype wandDatatype;
	MPI_Datatype type[9];
	int blocklen[9];
	MPI_Aint disp[9];
	
};


class Wand {
	
	public:

	Wand();				// Constructor
	~Wand();			// Destructor
	
	int input_update;
	MPI_Request request;
	
	PosWand *posWand[8];

	void readWand(int type); // gets the wand by its type
	
	private:
	
	void readtracker();			// read flock of birds
	void readspaceball();		// read space ball
	void readheadtracking();	// read intertrax
	void readglove();			// read 5dt data gloves
	
};

#endif
