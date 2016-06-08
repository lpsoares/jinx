#ifndef __viewer_h
#define __viewer_h

// Includes do MPI
#include <mpi.h>

struct Viewer {
	float pos[3];
	float dir[3];
	float up[3];
	float vel[3];
	float rot[3];
};

class PosViewer {
	public:
	
	PosViewer();
	
	struct Viewer view;
	
	MPI_Datatype viewDatatype;
	
	MPI_Datatype type[5];
	
	int blocklen[5];
	
	MPI_Aint disp[5];
	
};

#endif
