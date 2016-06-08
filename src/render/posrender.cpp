
#include "render.h"

/* Constructor */
PosRender::PosRender() {
	
	// reseting all values
	render.pos[0]=0;	render.pos[1]=0;	render.pos[2]=-10;	// default of viewpoint // EFECTIVE EM POSVIEW
	
	render.dir[0]=0;	render.dir[1]=0;	render.dir[2]=-1;
	
	render.up[0]=0;		render.up[1]=1; 	render.up[2]=0;		
	
	render.vel[0]=0;	render.vel[1]=0;	render.vel[2]=0;
	
	render.rot[0]=0;	render.rot[1]=0;	render.rot[2]=0;	
	
	render.trans[0]=0;	render.trans[1]=0;	render.trans[2]=0;
	
	render.rotat[0]=0;	render.rotat[1]=0;	render.rotat[2]=0;

	render.buttons=0;	render.seconds=0;	render.nseconds=0;
	
	render.wonoff=1;
	
	render.head_rotat[0]=0;	render.head_rotat[1]=0;	render.head_rotat[2]=0;
	render.head_trans[0]=0;	render.head_trans[1]=0;	render.head_trans[2]=0;
	
	render.fingers[0]=0;
	render.fingers[1]=0;
	render.fingers[2]=0;
	render.fingers[3]=0;
	render.fingers[4]=0;
	render.fingers[5]=0;
	render.fingers[6]=0;
	render.fingers[7]=0;
	render.fingers[8]=0;
	render.fingers[9]=0;
	render.fingers[10]=0;
	render.fingers[11]=0;
	render.fingers[12]=0;
	render.fingers[13]=0;
	render.fingers[14]=0;
	render.fingers[15]=0;
	render.fingers[16]=0;
	render.fingers[17]=0;
	render.fingers[18]=0;
	render.fingers[19]=0;
	
	render.quaternion[0]=1.0f;
	render.quaternion[1]=0.0f;
	render.quaternion[2]=0.0f;
	render.quaternion[3]=0.0f;
	
	render.type=0;
	

	type[0] = MPI_FLOAT;	type[1] = MPI_FLOAT;	type[2] = MPI_FLOAT;	type[3] = MPI_FLOAT;	type[4] = MPI_FLOAT;
	type[5] = MPI_FLOAT;	type[6] = MPI_FLOAT;	type[7] = MPI_INT;		type[8] = MPI_DOUBLE;	
	type[9] = MPI_SHORT_INT;
	type[10] = MPI_INT;
	type[11] = MPI_FLOAT;
	type[12] = MPI_FLOAT;
	type[13] = MPI_FLOAT;
	type[14] = MPI_FLOAT;
	type[15] = MPI_INT;
	
	blocklen[0]  = 3;	blocklen[1]  = 3;	blocklen[2] = 3;	blocklen[3] = 3;	blocklen[4] = 3;	
	blocklen[5]  = 3;	blocklen[6]  = 3;	blocklen[7] = 1;	blocklen[8] = 1;	blocklen[9] = 1;
	blocklen[10] = 1;	blocklen[11] = 3;	blocklen[12] = 3;	blocklen[13] = 20;
	blocklen[14] = 4;	blocklen[15] = 1;

	MPI_Address(render.pos,&disp[0]);
	MPI_Address(render.dir,&disp[1]);
	MPI_Address(render.up,&disp[2]);
	MPI_Address(render.vel,&disp[3]);
	MPI_Address(render.rot,&disp[4]);
	MPI_Address(render.trans,&disp[5]);
	MPI_Address(render.rotat,&disp[6]);
	MPI_Address(&render.buttons,&disp[7]);
	MPI_Address(&render.seconds,&disp[8]);
	MPI_Address(&render.nseconds,&disp[9]);
	MPI_Address(&render.wonoff,&disp[10]);
	MPI_Address(render.head_rotat,&disp[11]);
	MPI_Address(render.head_trans,&disp[12]);
	MPI_Address(render.fingers,&disp[13]);
	MPI_Address(render.quaternion,&disp[14]);
	MPI_Address(&render.type,&disp[15]);
	
	for(int i=15; i>=0; i--) {
		disp[i] -= disp[0];
	}
	
	MPI_Type_struct(16,blocklen,disp,type, &renderDatatype);
	MPI_Type_commit(&renderDatatype);
}
