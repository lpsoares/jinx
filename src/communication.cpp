
#include "communication.h"


Tunnel::Tunnel(int type) {
	
	if(type==INPUT) {
		posView = new PosViewer();
		MPI_Irecv(&posView->view, 1, posView->viewDatatype, INPUT, VIEWER_MOVEMENT, MPI_COMM_WORLD, &request);		
	}
	
	if(type==WAND) {
		posWand = new PosWand();
		MPI_Irecv(&posWand->wand, 1, posWand->wandDatatype, WAND, WAND_MOVEMENT, MPI_COMM_WORLD, &request);
	}
	
	if(type==GRAPH) {
		
	}
	
	if(type==SOUND) {
		posView2 = new PosViewer();
		MPI_Isend(&posView2->view, 1, posView2->viewDatatype, SOUND, RENDER_MOVEMENT, MPI_COMM_WORLD, &request);
	}
	
}


void Tunnel::setSound(PosRender* posRender) {

	MPI_Status status;

	float matrix[4][4]; // matrix for multipli operations
	
	midentity(matrix); // creating the identity matrix
	
	float m2[4][4];	// temporary matrix to hold the homogeneus operations
		
	// This is in oposite order, because the matrix operates like this, the other multiplications is in oposite order
	rotzmatrix(m2,-posRender->render.head_rotat[2]);	multipleMatrix(m2,matrix);	// rotation in x and multiply
	rotymatrix(m2,-posRender->render.head_rotat[1]);	multipleMatrix(m2,matrix);	// rotation in y and multiply
	rotxmatrix(m2,-posRender->render.head_rotat[0]);	multipleMatrix(m2,matrix);	// rotation in z and multiply

	float vec[4];

	posView2->view.pos[0] = posRender->render.pos[0];
	posView2->view.pos[1] = posRender->render.pos[1];
	posView2->view.pos[2] = posRender->render.pos[2];

	vec[0] = posRender->render.dir[0];
	vec[1] = posRender->render.dir[1];
	vec[2] = posRender->render.dir[2];

	#ifdef DEBUG
		cout << " Dir bef : " << vec[0] << " " << vec[1] << " " << vec[2];
	#endif

	multipleMatrix(matrix,vec);
			
	#ifdef DEBUG
		cout << " aft : " << vec[0] << " " << vec[1] << " " << vec[2] << endl;
	#endif
			
	posView2->view.dir[0] = vec[0];
	posView2->view.dir[1] = vec[1];
	posView2->view.dir[2] = vec[2];

	vec[0] = posRender->render.up[0];
	vec[1] = posRender->render.up[1];
	vec[2] = posRender->render.up[2];
			
	multipleMatrix(matrix,vec);
			
	posView2->view.up[0] = vec[0];
	posView2->view.up[1] = vec[1];
	posView2->view.up[2] = vec[2];
			
	MPI_Wait(&request, &status);
	MPI_Isend(&posView2->view, 1, posView2->viewDatatype, SOUND, RENDER_MOVEMENT, MPI_COMM_WORLD, &request);	
		
}



void Tunnel::setRender(PosRender* posRender) {
	
	int procesSize;
	MPI_Comm_size(MPI_COMM_WORLD, &procesSize);
		
	for(int destin = 0; destin < procesSize; destin++) {
		if(destin>=GRAPH) {	
			MPI_Ssend(&posRender->render, 1, posRender->renderDatatype, destin, RENDER_MOVEMENT, MPI_COMM_WORLD);
		}	
	}
	
}

void Tunnel::getWand(PosRender* posRender) {
	
	int received;
	MPI_Status status;
	
	MPI_Test(&request, &received, &status);
	while(received) {
				
		MPI_Wait(&request, &status);
		MPI_Irecv(&posWand->wand, 1, posWand->wandDatatype, WAND, WAND_MOVEMENT, MPI_COMM_WORLD, &request);
			
		posRender->render.trans[0]=(posWand->wand.trans[0]);
		posRender->render.trans[1]=(posWand->wand.trans[1]);
		posRender->render.trans[2]=(posWand->wand.trans[2]);
				
		posRender->render.rotat[0]=(posWand->wand.rotat[0]);
		posRender->render.rotat[1]=(posWand->wand.rotat[1]); 
		posRender->render.rotat[2]=(posWand->wand.rotat[2]);

		posRender->render.head_rotat[0]=(posWand->head.rotat[0]);
		posRender->render.head_rotat[1]=(posWand->head.rotat[1]); 
		posRender->render.head_rotat[2]=(posWand->head.rotat[2]);
			
		posRender->render.buttons=posWand->wand.buttons;
			
		posRender->render.fingers[0]=(posWand->wand.fingers[0]);
		posRender->render.fingers[1]=(posWand->wand.fingers[1]);
		posRender->render.fingers[2]=(posWand->wand.fingers[2]);
		posRender->render.fingers[3]=(posWand->wand.fingers[3]);
		posRender->render.fingers[4]=(posWand->wand.fingers[4]);
		posRender->render.fingers[5]=(posWand->wand.fingers[5]);
		posRender->render.fingers[6]=(posWand->wand.fingers[6]);
		posRender->render.fingers[7]=(posWand->wand.fingers[7]);
		posRender->render.fingers[8]=(posWand->wand.fingers[8]);
		posRender->render.fingers[9]=(posWand->wand.fingers[9]);
		posRender->render.fingers[10]=(posWand->wand.fingers[10]);
		posRender->render.fingers[11]=(posWand->wand.fingers[11]);
		posRender->render.fingers[12]=(posWand->wand.fingers[12]);
		posRender->render.fingers[13]=(posWand->wand.fingers[13]);
		posRender->render.fingers[14]=(posWand->wand.fingers[14]);
		posRender->render.fingers[15]=(posWand->wand.fingers[15]);
		posRender->render.fingers[16]=(posWand->wand.fingers[16]);
		posRender->render.fingers[17]=(posWand->wand.fingers[17]);
		posRender->render.fingers[18]=(posWand->wand.fingers[18]);
		posRender->render.fingers[19]=(posWand->wand.fingers[19]);
			
		posRender->render.wonoff=posWand->wand.wonoff;
		
		posRender->render.type=(posWand->wand.type);
		
		posRender->render.quaternion[0]=(posWand->wand.quaternion[0]);
		posRender->render.quaternion[1]=(posWand->wand.quaternion[1]);
		posRender->render.quaternion[2]=(posWand->wand.quaternion[2]);
		posRender->render.quaternion[3]=(posWand->wand.quaternion[3]);
		
		
		MPI_Test(&request, &received, &status);
	}
	
	int inputUpdate=1;
	MPI_Send(&inputUpdate, 1, MPI_INT, TRACK, INPUT_UPDATE, MPI_COMM_WORLD);
		
}

void Tunnel::getViewPosition(PosRender* posRender) {
	
	int received;
	MPI_Status status;
	
	//cout << "Entrou do teste" << endl; 
	MPI_Test(&request, &received, &status);
	//cout << "received " << received << endl;
	
	while(received) {
		MPI_Wait(&request, &status);
		MPI_Irecv(&posView->view, 1, posView->viewDatatype, INPUT, VIEWER_MOVEMENT, MPI_COMM_WORLD, &request);
			
		#ifdef DEBUG
			cout << " Viewpoints" ;
		#endif
			
		posRender->render.pos[0]=posView->view.pos[0];
		posRender->render.pos[1]=posView->view.pos[1];
		posRender->render.pos[2]=posView->view.pos[2];
			
		posRender->render.dir[0]=posView->view.dir[0];
		posRender->render.dir[1]=posView->view.dir[1];
		posRender->render.dir[2]=posView->view.dir[2];
			
		#ifdef DEBUG	
		cout << " Direction : " << posView->view.dir[0] << " " << 
			posView->view.dir[1] << " " << posView->view.dir[2];
		#endif
			
		posRender->render.up[0]=posView->view.up[0];
		posRender->render.up[1]=posView->view.up[1];
		posRender->render.up[2]=posView->view.up[2];
		
		#ifdef DEBUG			
		cout << " UP : " << posView->view.up[0] << " " << 
			posView->view.up[1] << " " << posView->view.up[2];
		#endif
			
		posRender->render.vel[0]=posView->view.vel[0];
		posRender->render.vel[1]=posView->view.vel[1];
		posRender->render.vel[2]=posView->view.vel[2];
				
		posRender->render.rot[0]=posView->view.rot[0];
		posRender->render.rot[1]=posView->view.rot[1];
		posRender->render.rot[2]=posView->view.rot[2];

		MPI_Test(&request, &received, &status);
	}
	
	//cout << "Enviou input update" << endl; 
	int inputUpdate=1;
	MPI_Ssend(&inputUpdate, 1, MPI_INT, JOYSTICK, INPUT_UPDATE, MPI_COMM_WORLD);
	//cout << "voltou do update" << endl; 
}



/*  */
Communication::Communication(int *argc, char **argv) {
	MPI_Init(argc, &argv);
}

/*  */
Communication::~Communication() {
	MPI_Finalize();		// closing the MPI communication system
}

/*  */
int Communication::getRID() {
	
	int my_rank;								//
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	//
	
	return(my_rank);
	
}
