#include "input.h"

#include "engine.h"
#include "general.h"
#include "math.h"

#include "Serial.h"

// Includes do MPI
#include <mpi.h>


#include <unistd.h>
//#include <time.h>


unsigned int Viewpoints::counter=0; // counter for Viewpoints

/* Constructor */
Viewpoints::Viewpoints() {
	
	this->position[0]=0;	// default
	this->position[1]=0;	// default
	this->position[2]=-10;	// default // EFECTIVE EM POSVIEW
	
	this->orientation[0]=0;
	this->orientation[1]=1;
	this->orientation[2]=0;
	this->orientation[3]=0;
	
	strcpy(this->description,"");	// reseting the description
	
}

Input::Input(x3d *temp_xxx) {
	
	
	this->xxx = temp_xxx;		// the x3d class that holds the model
	createViewpoint();			// creat a viewpoint table

	int totalProcesses;			// the total number of process
	if(MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses) != MPI_SUCCESS) {
		cerr << "Problems getting MPI Communicator size" << endl;
	}
	
	for(unsigned int destin = 0; destin < (unsigned int)totalProcesses; destin++)
	{
		if(destin>=GRAPH)
		{
			if(MPI_Send(&Viewpoints::counter, 1, MPI_INT, destin, OUTPUT_NVIEWPOINT, MPI_COMM_WORLD) != MPI_SUCCESS) {
				cerr << "Problems sending MPI viewpoint data" << endl;
			}
			for(unsigned int h=0;h<Viewpoints::counter;h++)
			{
				if(MPI_Send(&this->viewpoints[h].description, 256, MPI_CHAR, destin, OUTPUT_VIEWPOINT, MPI_COMM_WORLD) != MPI_SUCCESS) {
					cerr << "Problems sending MPI viewpoint data" << endl;
				}
			}
		}	
	}
	
}

/* This function is used because the special format required */
void *Input::tmovescene(void *ptr){
  return reinterpret_cast<Input*>(ptr)->movescene();
}

/* This class is used to create a view point table */
void Input::createViewpoint() {

	this->xxx->viewpoint(this->viewpoints); // Call the parser for viewpoints

	if(Viewpoints::counter) {
		Input::setViewpoint(0);
	}

	#ifdef DEBUG
		for(int g=0;g<Viewpoints::counter;g++) {
			cout << "Viewpoint " << g << " :  " << viewpoints[g].description <<
			"Position "  << viewpoints[g].position[0] << " " << viewpoints[g].position[1] << " " << viewpoints[g].position[2] << endl;
		}
	#endif

}

void *Input::movescene() {

	#ifdef DEBUG
		cout << "Debuging movescene" << endl;
	#endif





	
	int received;
		
	MPI_Status status;
		
	PosViewer* posView = new PosViewer();
	
	MPI_Request request_move;
	float move[6];
	MPI_Irecv(&move, 6, MPI_FLOAT, MPI_ANY_SOURCE, INPUT_MOVE, MPI_COMM_WORLD, &request_move);

	MPI_Request request_viewpoint;
	int id=0;  // start in viewpoint zero
	MPI_Irecv(&id, 1, MPI_INT, MPI_ANY_SOURCE, INPUT_VIEWPOINT, MPI_COMM_WORLD, &request_viewpoint);

	this->posRender = new PosRender();
	Engine::getTime(this->posRender);
	
	bool new_message=false;	// Case a mesage is received from some input device this flag is set informing that a message should be sent to main
	
	bool reach;
	bool search;

	reach=true;				// This flag is set informing that a view point is to be looked and is not yet reached
	search=false;			// inform that the search process for a view point had begun



	// Ventilador //
				
	bool ventilador;
	ventilador=false;
	//ventilador=true;

	Serial serial;

	if(ventilador) {
				                                                                                                                                                                
        cout << "Opening ventilador" << endl;
        serial.sopen("/dev/ttyS0",57600);
                    
	}
		
    unsigned char Vel1; // fan speed
	unsigned char Vel2; // fan speed
	unsigned char Vel3; // fan speed
	
    unsigned char VelNumb; // fan number
        
        
	// Ventilador //

	do {
		
		/* Case a viewpoint is set */

		Engine::getTime(this->posRender);		
		
		
		{
			
			
			/*
			// CONCERTAR ISSO //
			//this->xxx->serialize(this->posRender,0);  // this rotine is wrong but will be usufull now
			// Acredito que isto e para viewpoints animados
			
			for(int f=0;f<this->xxx->routeCount;f++) {	//check each possible route
				if (!strcmp (this->xxx->routeTable[f]->toNode,viewpoints[id].DEF)) {
					if(!strcmp(this->xxx->routeTable[f]->toField,"position")) {
						posView->view.pos[0] = atof(strtok(this->xxx->routeTable[f]->fvalue," "));
						posView->view.pos[1] = atof(strtok(NULL," "));
						posView->view.pos[2] = atof(strtok(NULL," "));
						reach=true;
					}
				}
			}
			*/
			
			
			
			
			
			if(!reach) { // until reach a possition
			
				if(!search) { // if search process has not begun
					
					this->nseconds=this->posRender->render.nseconds;
					this->seconds=this->posRender->render.seconds;
						
					this->dif[0]=(viewpoints[id].position[0]-posView->view.pos[0]);
					this->dif[1]=(viewpoints[id].position[1]-posView->view.pos[1]);
					this->dif[2]=(viewpoints[id].position[2]-posView->view.pos[2]);
					
					this->ini[0]=posView->view.pos[0];
					this->ini[1]=posView->view.pos[1];
					this->ini[2]=posView->view.pos[2];
					
					search=true;
					
				} else {
					
					if( (this->posRender->render.seconds>this->seconds) & (this->posRender->render.nseconds > this->nseconds) ) {
						search=false;
						reach=true;
					
						posView->view.pos[0] = viewpoints[id].position[0];
						posView->view.pos[1] = viewpoints[id].position[1];
						posView->view.pos[2] = viewpoints[id].position[2];
					
						float m[4][4];
						
						rotmatrix(m,viewpoints[id].orientation[3],
							viewpoints[id].orientation[0], viewpoints[id].orientation[1], viewpoints[id].orientation[2]);
						
						float vec[4];
						
						vec[0]=0;
						vec[1]=0;
						vec[2]=-1;
						vec[3]=0;
						
						multipleMatrix(m,vec);
						
						posView->view.dir[0] = vec[0];
						posView->view.dir[1] = vec[1];
						posView->view.dir[2] = vec[2];
					
						vec[0]=0;
						vec[1]=1;
						vec[2]=0;
						vec[3]=0;
												
						multipleMatrix(m,vec);
						
						posView->view.up[0] = vec[0];
						posView->view.up[1] = vec[1];
						posView->view.up[2] = vec[2];

						// ISTO E UM ABSURDO DE GAMBIARA MAS VAI FUNCIONAR EM SISTEMA DE UMA SO ROTACAO
						
						posView->view.rot[0] = viewpoints[id].orientation[0] * viewpoints[id].orientation[3] ;
						posView->view.rot[1] = viewpoints[id].orientation[1] * viewpoints[id].orientation[3] ;
						posView->view.rot[2] = viewpoints[id].orientation[2] * viewpoints[id].orientation[3] ;
						
						#ifdef DEBUG
						cout << " Viewpoints Direction : " << posView->view.dir[0] << " " << 
							posView->view.dir[1] << " " << posView->view.dir[2] << " " << endl;
						#endif

					} else {
						short int temp = (short int)(this->posRender->render.seconds - this->seconds);
						
						#ifdef DEBUG
						cout << "inicial time nsec" << this->nseconds << " sec " << this->seconds << endl;
						cout << "instant time nsec" << this->posRender->render.nseconds << " sec " << this->posRender->render.seconds << endl;
						cout << "diferenca principal" << temp << endl;
						cout << "current position    " << posView->view.pos[0] << " , "  << posView->view.pos[1] << " , "  << posView->view.pos[2] << endl;
						cout << "viewpoint position  " << viewpoints[id].position[0] << " , " << viewpoints[id].position[1] << " , "  << viewpoints[id].position[2] << endl;
						cout << "vector de diferenca " << this->dif[0] << " , " << this->dif[1] << " , " << this->dif[2] << endl;
						#endif
						
						posView->view.pos[0] = ini[0] + dif[0]*((float)( this->posRender->render.nseconds + ( (temp*100)-this->nseconds) )/100);
						posView->view.pos[1] = ini[1] + dif[1]*((float)( this->posRender->render.nseconds + ( (temp*100)-this->nseconds) )/100);
						posView->view.pos[2] = ini[2] + dif[2]*((float)( this->posRender->render.nseconds + ( (temp*100)-this->nseconds) )/100);

					}
				}
			}

		}

		// --- MOVE --- //
		
		MPI_Test(&request_move, &received, &status);
		while(received) {								// This solves the problem of buffering

			// Just pass this part after clean the buffer
			MPI_Wait(&request_move, &status);
			
			if(move[0]!=0) { // translationX
				float vec[3];
				vec[0] = posView->view.dir[0];
				vec[1] = posView->view.dir[1];
				vec[2] = posView->view.dir[2];
				rotate(-90, posView->view.up,vec);
				posView->view.pos[0] += move[0]*vec[0];
				posView->view.pos[1] += move[0]*vec[1];
				posView->view.pos[2] -= move[0]*vec[2];
			}
			
			if(move[1]!=0) { // translationY
				posView->view.pos[0] += move[1]*posView->view.up[0];
				posView->view.pos[1] += move[1]*posView->view.up[1];
				posView->view.pos[2] -= move[1]*posView->view.up[2];
			}
			
			
			if(move[2]!=0) { // translationZ
				posView->view.pos[0] += move[2]*posView->view.dir[0];
				posView->view.pos[1] += move[2]*posView->view.dir[1];
				posView->view.pos[2] -= move[2]*posView->view.dir[2];
			}
			
			if(move[3]!=0) { // rotationX
				float vec2[3];
				vec2[0] = posView->view.dir[0];
				vec2[1] = posView->view.dir[1];
				vec2[2] = posView->view.dir[2];
				rotate(-90, posView->view.up,vec2);
				
				rotate( ((double)move[3]) ,vec2,posView->view.dir);
				rotate( ((double)move[3]) ,vec2,posView->view.up);
				
				posView->view.rot[0] -= move[3];
			}
			
			if(move[4]!=0) { // rotationY
				rotate( ((double)move[4]) ,posView->view.up,posView->view.dir);
				posView->view.rot[1] += move[4];
			}
			
			if(move[5]!=0) { // rotationZ
				rotate( ((double)move[5]),posView->view.dir,posView->view.up);
				posView->view.rot[2] += move[5];
			}
			
						
			MPI_Irecv(&move, 6, MPI_FLOAT, MPI_ANY_SOURCE, INPUT_MOVE, MPI_COMM_WORLD, &request_move);
			new_message=true;
			
			MPI_Test(&request_move, &received, &status);
		}

		MPI_Test(&request_viewpoint, &received, &status);
			if(received) {
				reach=false;
				MPI_Wait(&request_viewpoint, &status);
				MPI_Irecv(&id, 1, MPI_INT, MPI_ANY_SOURCE, INPUT_VIEWPOINT, MPI_COMM_WORLD, &request_viewpoint);
				new_message=true;
			}

			if(new_message) {
			
			bool useLimits;
			useLimits=false;
			//useLimits=true;
	

			if(ventilador) {
				

				// Ventilador //				
			
				Vel1 = 30;
				Vel2 = 30;
				Vel3 = 30;
                    
				#ifdef DEBUG1				
   		 	    	cout << "Writing : " << endl;
				#endif
   			    
				// Versao Antiga
				// Os valores bons são de 19 a 44 (porem por questão de seguranca de 24 a 42)
				//Vel = ((unsigned char)(((posView->view.dir[2] + 1) / 2) * (42 - 24) )) + 24;
				
				// Completo (Versao atual)
				// Os valores bons são de 19 a 44 (porem por questão de seguranca de 24 a 42)
				//Vel = ((unsigned char)(((posView->view.dir[2] + 1) / 2) * (228 - 99) )) + 99;
				
				Vel1 = ((unsigned char)(((posView->view.dir[2] + 1) / 2) * (255) )) + 0;
				Vel2 = ((unsigned char)(((posView->view.dir[0] + 1) / 2) * (255) )) + 0;
				Vel3 = ((unsigned char)(((-posView->view.pos[1] + 0) / 50) * (200) )) + 20;
				
				//#ifdef DEBUG1
					cout << "ventilador : (" << posView->view.pos[1] << ")  ("  << (int)Vel1 << ")  (" << (int)Vel2 << ")  ("<< (int)Vel3 << ")" << endl;
				//#endif
				
				VelNumb='1';
				serial.swrite(&VelNumb,1);
				serial.swrite(&Vel1,1);
				
				
				VelNumb='2';
				serial.swrite(&VelNumb,1);
				serial.swrite(&Vel2,1);

				VelNumb='3';
				serial.swrite(&VelNumb,1);
				serial.swrite(&Vel3,1);
				
				// Ventilador //

			}			
				
			if(useLimits) {

	
				
				
				#ifdef DEBUG
					cout << " Dir X " << posView->view.dir[0] << endl;
					cout << " Dir Y " << posView->view.dir[1] << endl;
					cout << " Dir Z " << posView->view.dir[2] << endl;
					cout << endl;
				#endif
				
				
				posView->view.pos[0] += 0.4*posView->view.dir[0];
				posView->view.pos[1] += 0.4*posView->view.dir[1];
				posView->view.pos[2] -= 0.4*posView->view.dir[2];
				
				
				float limits[6];
				
				limits[0]=144.5;
				limits[1]=-303.5;
				//limits[2]= 370;
				limits[2]= 0;
				limits[3]= 50.0;
				limits[4]=-32.2;
				limits[5]=257.0;
			
				if(posView->view.pos[0]>- (limits[0]) ) {
					posView->view.pos[0]=posView->view.pos[0]-1;
				}
			
				if(posView->view.pos[0]<- (limits[1]) ) {
					posView->view.pos[0]=posView->view.pos[0]+1;
				}
			
				if(posView->view.pos[1]>- (limits[2])) {
					posView->view.pos[1]=posView->view.pos[1]-1;
				}
			
				if(posView->view.pos[1]<- (limits[3]) ) {
					posView->view.pos[1]=posView->view.pos[1]+1;
				}
								
				if(posView->view.pos[2]>- (limits[4]) ) {
					posView->view.pos[2]=posView->view.pos[2]-1;
				}
			
				if(posView->view.pos[2]<- (limits[5])) {
					posView->view.pos[2]=posView->view.pos[2]+1;
				}
	
			}
	
			#ifdef DEBUG
				cout << " Pos X " << posView->view.pos[0] << endl;
				cout << " Pos Y " << posView->view.pos[1] << endl;
				cout << " Pos Z " << posView->view.pos[2] << endl;
				cout << endl;
			#endif
			
			#ifdef DEBUG
				cout << "Sending message from input to main" << endl;
			#endif
			
			MPI_Rsend(&posView->view, 1, posView->viewDatatype, MAIN, VIEWER_MOVEMENT, MPI_COMM_WORLD);
			new_message=false;
			
		} 
	
		breath(); // this is just an sleep	
		
		
		
	} while(true);

	delete posView;

	return NULL;
}

void Input::movewand() {
	
	#ifdef DEBUG
		cout << "Debuging movewand" << endl;
	#endif
	
	int received;
		
	MPI_Status status;
		
	PosWand *posSend = new PosWand(); 
	PosWand *posWand[2];
	
	MPI_Request request_wand;
	posWand[0]= new PosWand(); 
	MPI_Irecv(&posWand[0]->wand, 1, posWand[0]->wandDatatype, MPI_ANY_SOURCE, INPUT_WAND, MPI_COMM_WORLD, &request_wand);
			
	bool new_message=false;
	
	while(true) {

		MPI_Test(&request_wand, &received, &status);
		if(received) {
			MPI_Wait(&request_wand, &status);
			posSend->wand.trans[0]=posWand[0]->wand.trans[0];
			posSend->wand.trans[1]=posWand[0]->wand.trans[1];
			posSend->wand.trans[2]=posWand[0]->wand.trans[2];
			posSend->wand.rotat[0]=posWand[0]->wand.rotat[0];
			posSend->wand.rotat[1]=posWand[0]->wand.rotat[1];
			posSend->wand.rotat[2]=posWand[0]->wand.rotat[2];
			posSend->wand.buttons=posWand[0]->wand.buttons;
			posSend->wand.wonoff=posWand[0]->wand.wonoff;
			
			#ifdef DEBUG
			if (posWand[0]->wand.buttons & SBALL_BUTTON_9) cout << "Left button in input" << endl;
			cout << "trans : " <<  posSend->wand.trans[0] << endl;
			cout << "rotat : " <<  posSend->wand.rotat[0] << endl;
			#endif
			
			posSend->wand.fingers[0]=posWand[0]->wand.fingers[0];
			posSend->wand.fingers[1]=posWand[0]->wand.fingers[1];
			posSend->wand.fingers[2]=posWand[0]->wand.fingers[2];
			posSend->wand.fingers[3]=posWand[0]->wand.fingers[3];
			posSend->wand.fingers[4]=posWand[0]->wand.fingers[4];
			posSend->wand.fingers[5]=posWand[0]->wand.fingers[5];
			posSend->wand.fingers[6]=posWand[0]->wand.fingers[6];
			posSend->wand.fingers[7]=posWand[0]->wand.fingers[7];
			posSend->wand.fingers[8]=posWand[0]->wand.fingers[8];
			posSend->wand.fingers[9]=posWand[0]->wand.fingers[9];
			posSend->wand.fingers[10]=posWand[0]->wand.fingers[10];
			posSend->wand.fingers[11]=posWand[0]->wand.fingers[11];
			posSend->wand.fingers[12]=posWand[0]->wand.fingers[12];
			posSend->wand.fingers[13]=posWand[0]->wand.fingers[13];
			posSend->wand.fingers[14]=posWand[0]->wand.fingers[14];
			posSend->wand.fingers[15]=posWand[0]->wand.fingers[15];
			posSend->wand.fingers[16]=posWand[0]->wand.fingers[16];
			posSend->wand.fingers[17]=posWand[0]->wand.fingers[17];
			posSend->wand.fingers[18]=posWand[0]->wand.fingers[18];
			posSend->wand.fingers[19]=posWand[0]->wand.fingers[19];
			
			#ifdef DEBUG
			cout << " FIN: " ;
			for(int ff=0;ff<20;ff++) {
				cout << posSend->wand.fingers[ff] << " , ";
			}
			cout << endl;
			#endif
			
			posSend->wand.type=posWand[0]->wand.type;
			
			posSend->wand.quaternion[0]=posWand[0]->wand.quaternion[0];
			posSend->wand.quaternion[1]=posWand[0]->wand.quaternion[1];
			posSend->wand.quaternion[2]=posWand[0]->wand.quaternion[2];
			posSend->wand.quaternion[3]=posWand[0]->wand.quaternion[3];
			
			
			#ifdef DEBUG3
			cout << "Type : " << posWand[0]->wand.type << endl;
			cout << "Quaternion : " << " w=" << posWand[0]->wand.quaternion[0] << 
										 ", x=" << posWand[0]->wand.quaternion[1] << 
										 ", y=" << posWand[0]->wand.quaternion[2] <<
										 ", z=" << posWand[0]->wand.quaternion[3] << endl;
			#endif
			
			posSend->head.rotat[0]=posWand[0]->head.rotat[0];
			posSend->head.rotat[1]=posWand[0]->head.rotat[1];
			posSend->head.rotat[2]=posWand[0]->head.rotat[2];
			
			MPI_Irecv(&posWand[0]->wand, 1, posWand[0]->wandDatatype, MPI_ANY_SOURCE, INPUT_WAND, MPI_COMM_WORLD, &request_wand);
			new_message=true;
		}
						
		if(new_message) {
			#ifdef DEBUG
				cout << "Sending message from wand to main" << endl;
			#endif
			MPI_Ssend(&posSend->wand, 1, posWand[0]->wandDatatype, MAIN, WAND_MOVEMENT, MPI_COMM_WORLD);
			new_message=false;

		}
	
		breath();	// just a sleep			
		
	}

	delete posSend; 
	delete posWand[0]; 
	
}

/** Move, acept translationX, translationY, translationZ, rotationX, rotationY, rotationZ */
// Inserir Inline para versao futura
void Input::move(float data[6]) {
	MPI_Rsend(data, 6, MPI_FLOAT, INPUT, INPUT_MOVE, MPI_COMM_WORLD);
}

void Input::setViewpoint(int id) {
	MPI_Rsend(&id, 1, MPI_INT, INPUT, INPUT_VIEWPOINT, MPI_COMM_WORLD);
}
