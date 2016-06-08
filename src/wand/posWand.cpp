

#include "wand.h"


PosWand::PosWand() {
	
	wand.wonoff=1;		// inform if the wand appear or not
	wand.lixo=0;		// for future
	wand.trans[0]=0;	// translation of the wand in X
	wand.trans[1]=0;	// translation of the wand in Y
	wand.trans[2]=0;	// translation of the wand in Z
	wand.rotat[0]=0;	// rotation of the wand in X
	wand.rotat[1]=0;	// rotation of the wand in Y
	wand.rotat[2]=0;	// rotation of the wand in Z
	wand.buttons=0;		// status of the buttons
	
	wand.fingers[ 0]=0;
	wand.fingers[ 1]=0;
	wand.fingers[ 2]=0;
	wand.fingers[ 3]=0;
	wand.fingers[ 4]=0;
	wand.fingers[ 5]=0;
	wand.fingers[ 6]=0;
	wand.fingers[ 7]=0;
	wand.fingers[ 8]=0;
	wand.fingers[ 9]=0;
	wand.fingers[10]=0;
	wand.fingers[11]=0;
	wand.fingers[12]=0;
	wand.fingers[13]=0;
	wand.fingers[14]=0;
	wand.fingers[15]=0;
	wand.fingers[16]=0;
	wand.fingers[17]=0;
	wand.fingers[18]=0;
	wand.fingers[19]=0;
		
	wand.rotat[0]=0;	// head rotation in X
	wand.rotat[1]=0;	// head rotation in Y
	wand.rotat[2]=0;	// head rotation in Z
	
	wand.quaternion[0]=1.0f;
	wand.quaternion[1]=0.0f;
	wand.quaternion[2]=0.0f;
	wand.quaternion[3]=0.0f;
	
	wand.type=0;
	
	type[0] = MPI_INT;
	type[1] = MPI_FLOAT;
	type[2] = MPI_FLOAT;
	type[3] = MPI_FLOAT;
	type[4] = MPI_INT;
	type[5] = MPI_FLOAT;
	type[6] = MPI_FLOAT;
	type[7] = MPI_INT;
	type[8] = MPI_FLOAT;
	
	
	
	blocklen[0] =  1;
	blocklen[1] =  1;	
	blocklen[2] =  3;
	blocklen[3] =  3;
	blocklen[4] =  1;
	blocklen[5] = 20;
	blocklen[6] =  4;
	blocklen[7] =  1;
	blocklen[8] =  3;
	
	MPI_Address(&wand.wonoff,&disp[0]);
	MPI_Address(&wand.lixo,&disp[1]);	
	MPI_Address(wand.trans,&disp[2]);
	MPI_Address(wand.rotat,&disp[3]);
	MPI_Address(&wand.buttons,&disp[4]);
	MPI_Address(wand.fingers,&disp[5]);
	MPI_Address(wand.quaternion,&disp[6]);
	MPI_Address(&wand.type,&disp[7]);
	MPI_Address(head.rotat,&disp[8]);
		
	for(int i=8; i>=0; i--) 
		disp[i] -= disp[0];
	
	MPI_Type_struct(9,blocklen,disp,type,&wandDatatype);
	MPI_Type_commit(&wandDatatype);
	
}
