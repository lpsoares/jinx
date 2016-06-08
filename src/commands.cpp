#include "commands.h"

Commands::Commands() {
	
	command.reboot=0;
	
	type[0] = MPI_INT;
	type[1] = MPI_FLOAT;
	type[2] = MPI_FLOAT;
	type[3] = MPI_CHAR;
	
	blocklen[0] = 1;
	blocklen[1] = 1;
	blocklen[2] = 3;
	blocklen[3] = 256;
	
	MPI_Address(&command.reboot,&disp[0]);
	MPI_Address(&command.valor1,&disp[1]);
	MPI_Address(command.valor2,&disp[2]);
	MPI_Address(command.url,&disp[3]);
	
	int NUN_ELEMENTOS=4;
	
	for(int i=(NUN_ELEMENTOS-1); i>=0; i--) 
		disp[i] -= disp[0];
	
	MPI_Type_struct(NUN_ELEMENTOS,blocklen,disp,type,&commandsDatatype);
	MPI_Type_commit(&commandsDatatype);
	
}
