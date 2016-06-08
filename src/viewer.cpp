#include "viewer.h"


PosViewer::PosViewer() {
	view.pos[0]=0;
	view.pos[1]=0;
	view.pos[2]=-10;
	view.dir[0]=0;
	view.dir[1]=0;
	view.dir[2]=-1;
	view.up[0]=0;
	view.up[1]=1;
	view.up[2]=0;
	view.vel[0]=0;
	view.vel[1]=0;
	view.vel[2]=0;
	view.rot[0]=0;
	view.rot[1]=0;
	view.rot[2]=0;
	
	type[0] = MPI_FLOAT;
	type[1] = MPI_FLOAT;
	type[2] = MPI_FLOAT;
	type[3] = MPI_FLOAT;
	type[4] = MPI_FLOAT;
	
	blocklen[0] = 3;
	blocklen[1] = 3;
	blocklen[2] = 3;
	blocklen[3] = 3;
	blocklen[4] = 3;
	
	MPI_Address(view.pos,&disp[0]);
	MPI_Address(view.dir,&disp[1]);
	MPI_Address(view.up,&disp[2]);
	MPI_Address(view.vel,&disp[3]);
	MPI_Address(view.rot,&disp[4]);
	
	for(int i=4; i>=0; i--) 
		disp[i] -= disp[0];
	
	MPI_Type_struct(5,blocklen,disp,type, &viewDatatype);
	MPI_Type_commit(&viewDatatype);
	
}
