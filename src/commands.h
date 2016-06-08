#ifndef __commands_h
#define __commands_h

// Includes do MPI
#include <mpi.h>

struct Command {
	int reboot;
	float valor1;
	float valor2[3];
	char url[256];
};

class Commands {

	public:
	
	Commands();
	
	struct Command command;
	
	MPI_Datatype commandsDatatype;
	
	MPI_Datatype type[4];
	
	int blocklen[4];
	
	MPI_Aint disp[4];
	
};

#endif
