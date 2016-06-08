/* JINX */


#include <stdlib.h>
#include "engine.h" 

int main(int argc, char* argv[]) {
	
	char configurationFile[256];
	char x3dFile[256];
	
	strcpy(configurationFile,"");
	strcpy(x3dFile,"");
	
	MPI_Status status;
	int procesSize;
	
	Engine* engine = new Engine(&argc,argv);
	
	if(engine->my_rank==0) {
	
		// Parse the command line arguments
		for (int i=1;i<argc;i++) {
		
			if(strstr(argv[i],"-configuration") != NULL) {
				strcpy(configurationFile,(argv[i]+14));
				cout << "Reading Configuration : " << configurationFile << endl;
			} else {
				#ifdef DEBUG1
					cout << "No defined configuration file" << endl;	
				#endif
			}
			
			if(strstr(argv[i],"-X3D") != NULL) {
				strcpy(x3dFile,(argv[i]+4));
				cout << "Reading X3D : " << x3dFile << endl;
			} else {
				#ifdef DEBUG1
					cout << "No defined X3D file" << endl;
				#endif
			}
			
		}
		
		MPI_Comm_size(MPI_COMM_WORLD, &procesSize);
		for(int g=1;g<procesSize;g++) {
			MPI_Send(configurationFile, 256, MPI_CHAR, g, 100, MPI_COMM_WORLD);
		}
		for(int g=1;g<procesSize;g++) {
			MPI_Send(x3dFile, 256, MPI_CHAR, g, 100, MPI_COMM_WORLD);
		}
		
	} else {
		
		MPI_Recv(configurationFile, 256, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
		cout << "Getting Configuration : " << configurationFile << endl;
		
		MPI_Recv(x3dFile, 256, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
		cout << "Getting X3D : " << x3dFile << endl;
		
	}

	// Cheking if the file name was passed
	if(strcmp(configurationFile,"")) {
		engine->Configuration(configurationFile);
	} else {
		cout << "Using defualt configuration file" << endl;
		engine->Configuration("http://localhost/x3d/render.xml");
	}
	
	// Cheking if the file name was passed
	if(strcmp(x3dFile,"")) {
		engine->X3D(x3dFile);
	} else {
		cout << "Using defualt file" << endl;
		engine->X3D("http://power.midianet.intranet/x3d/rio/RioDeJaneiro.x3d");
	}
	
	
	engine->run();

	delete engine;

	return(0); 

}
