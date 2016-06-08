// Engine is the core class

// Jinx Includes
#include "engine.h"
#include "general.h"
#include "commands.h"

// PThread Includes
#include <pthread.h>

// C & C++ Includes
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>

// Includes do MPI
#include <mpi.h>


#include <strings.h>

#define MESSAGES

Engine* Engine::my_Engine;

/* Constructor */
Engine::Engine(int *argc, char **argv) {
	
	this->argc = argc;	// it is necessary to MPI and OpenGL Glut
	this->argv = argv;	// it is necessary to MPI and OpenGL Glut

	this->func = NULL;	// setting null for the function used for openGL applications

	strcpy(this->configurationFileName,"");
	strcpy(this->x3dfile,"");
	strcpy(this->source,"");
	
	this->rrr = NULL;
	this->xxx = NULL;
	this->lll = NULL;
	
	Engine::my_Engine = this;


	
	xmlbase = new xmlBase();
	// verificar se nao esta rodando em um so computador.
	comm = new Communication(argc, argv);
	
	this->my_rank = comm->getRID();	// get rank id

	MPI_Comm_size(MPI_COMM_WORLD, &this->procesSize);
	
}

/* Destructor */
Engine::~Engine() {
	
}

void Engine::OpenGL(void (*nfunc)(void)) {
	this->func = nfunc;
}

void Engine::X3D(char *tx3dfile) {
	
	#ifdef DEBUG
		cout << " Reading X3D file" << endl;
	#endif
	
	strcpy(this->x3dfile,tx3dfile);
	
	char *tmp;
	tmp = strrchr(this->x3dfile,'/');

	// this is used to get the http path	
	strncpy(this->source, this->x3dfile, (tmp-this->x3dfile) );
	this->source[(tmp-this->x3dfile)]='/';
	this->source[(tmp-this->x3dfile)+1]='\0';
}

/* returns the source path of the URL specified */
char* Engine::getSource() {
	return(my_Engine->source);
}

void Engine::Configuration(char *fileName) {
	strcpy(this->configurationFileName,fileName);
}

int Engine::run() {
	
	
	
			
	#ifdef MESSAGES
		int pid; 									// Holds the process id.
		pid = getpid();								// Gets the process id.
		char computername[100];						// Holds the computer name.
		gethostname(computername,100);				// Gets the computer name.
		cout << "Loading in process : " << my_rank;	// Display rank id,
		cout << " in computer " << computername;	// computer name,
		cout << " PID : " << pid << endl;			// process ID.
	#endif	
	
	pthread_t cxml;			//
	pthread_t cx3d;			// Thread for 
	
	pthread_t inputThread;	// Thread for getting the joystick input
	pthread_t joystThread;	// Thread for joystick intercation system
	pthread_t wandThread;	// Thread for getting the tracking system
	pthread_t trackThread;	// Thread for tracking system
	
	pthread_t mainThread;	// Thread for managment system
	
	pthread_t graphThread;	// Thread for graphical rendering
	pthread_t soundThread;	// Thread for sound rendering

	if( (my_rank < GRAPH) ) {
		#ifdef DEBUG
			cout << "Graph communicator " << my_rank << endl;
		#endif
		Render::graphCommunicatorGlobal();
	}


	while(true) {
		
	if( (my_rank >= GRAPH) || (my_rank == INPUT) || (my_rank == SOUND) || (my_rank == WAND)) {
		pthread_create(&cx3d, NULL, Engine::createx3d, NULL);		// parse the x3d file
	}
		
	if( (my_rank >= GRAPH) || (my_rank == SOUND) || (my_rank == TRACK) || (my_rank == JOYSTICK)) {
		pthread_create(&cxml, NULL, Engine::createxml, NULL);		// parse the xml file
	}
	
	
	if( (my_rank >= GRAPH) || (my_rank == INPUT) || (my_rank == SOUND) || (my_rank == WAND)) {
		if(pthread_join(cx3d, NULL)) {
			cerr << "Error on thread " << "cx3d" << endl;
		}
	}
	
	if( (my_rank >= GRAPH) || (my_rank == SOUND) || (my_rank == TRACK) || (my_rank == JOYSTICK)) {
		if(pthread_join(cxml, NULL)) {
			cerr << "Error on thread " << "cx3d" << endl;
		}
	}
	
	if( my_rank >= GRAPH ) {	
		pthread_create(&graphThread,NULL,Engine::tgraph,NULL);
		if(pthread_join(graphThread, NULL)) {
			cerr << "Error on thread " << "graphThread" << endl;
		}
	}
	
	if( my_rank == INPUT ) {
		pthread_create(&inputThread,NULL,Engine::tinput,NULL);
		if(pthread_join(inputThread, NULL)) {
			cerr << "Error on thread " << "inputThread" << endl;
		}
	}
	
	if( my_rank == SOUND ) {
		#ifdef DEBUG
		cout << "Entrando no SOM" << endl;
		#endif
		
		pthread_create(&soundThread,NULL,Engine::tsound,NULL);
		if(pthread_join(soundThread, NULL)) {
			cerr << "Error on thread " << "soundThread" << endl;
		}
		
		#ifdef DEBUG
		cout << "Saindo do SOM" << endl;
		#endif
	}
		
	if(my_rank == JOYSTICK) {
		pthread_create(&joystThread,NULL,Engine::tjoyst,NULL);
		if(pthread_join(joystThread, NULL)) {
			cerr << "Error on thread " << "joystThread" << endl;
		}
	}

	if(my_rank == WAND)	{
		pthread_create(&wandThread,NULL,Engine::twand,NULL);
		if(pthread_join(wandThread, NULL)) {
			cerr << "Error on thread " << "wandThread" << endl;
		}
	}

	if(my_rank == TRACK)	{
		pthread_create(&trackThread,NULL,Engine::ttrack,NULL);
		if(pthread_join(trackThread, NULL)) {
			cerr << "Error on thread " << "trackThread" << endl;
		}
	}

	if( my_rank == MAIN ) {
		pthread_create(&mainThread,NULL,Engine::tmain,NULL);
		if(pthread_join (mainThread, NULL)) {
			cerr << "Error on thread " << "mainThread" << endl;
		}
	}

	
	if( (my_rank >= GRAPH) || (my_rank == INPUT) || (my_rank == SOUND) || (my_rank == WAND)) {
		delete xxx;
	}
	
	if( (my_rank >= GRAPH) || (my_rank == SOUND) || (my_rank == TRACK) ) {
		delete lll;
	}

	
	}
	
	delete comm;
	delete xmlbase;
	
	return(0);
	
}

//* main funcion that manage the main messages
void Engine::manager() {
	
	PosRender* posRender = new PosRender();
		
	Tunnel *tunnelInput = new Tunnel(INPUT);
	Tunnel *tunnelWand  = new Tunnel(WAND);
	Tunnel *tunnelGraph = new Tunnel(GRAPH);
	Tunnel *tunnelSound = new Tunnel(SOUND);
		



		MPI_Status statusZ;
		MPI_Request request;		// used to get the data from main process
		MPI_Request requestZ;		// used to get the data from main process
		int receivedZ=1;
		Commands *comm1 = NULL;
		comm1 = new Commands();
		if(comm1==NULL) {
			cout << "Error in Commands" << endl;
		}

		MPI_Irecv(&comm1->command, 1, comm1->commandsDatatype,MPI_ANY_SOURCE, COMMAND, MPI_COMM_WORLD, &requestZ);

	
	while(true) {

		#ifdef DEBUG
			cout << "Managing messages throught main" << endl;
		#endif
		
		getTime(posRender);

		// Input systems
		tunnelInput->getViewPosition(posRender); // get the possition from input system
		tunnelWand->getWand(posRender);
	
		// Output systems
		tunnelGraph->setRender(posRender);
		tunnelSound->setSound(posRender);
		// The Graph and Sound could have different speed, then one can lock the other
		
		breath(); // this is just a sleep

		
			receivedZ=1;
			MPI_Test(&requestZ, &receivedZ, &statusZ);
			if(receivedZ) {
			
				MPI_Wait(&requestZ, &statusZ);
				MPI_Irecv(&comm1->command, 1, comm1->commandsDatatype,MPI_ANY_SOURCE, COMMAND, MPI_COMM_WORLD, &requestZ);
			
				#ifdef DEBUG
				cout << "Retorno MAIN : " << comm1->command.reboot << endl;
				#endif
				
				MPI_Isend(&comm1->command, 1, comm1->commandsDatatype, SOUND, COMMAND, MPI_COMM_WORLD, &request);
				MPI_Isend(&comm1->command, 1, comm1->commandsDatatype, INPUT, COMMAND, MPI_COMM_WORLD, &request);
				
			}	
		
	}

	delete posRender;
	
	delete tunnelInput;
	delete tunnelWand;
	delete tunnelGraph;
	delete tunnelSound;
			
}

/* gets the system time and puts in the Rendering info */
void Engine::getTime(PosRender* posRender) {

	struct timespec time; // hold the time for animations in X3D
		
	if( clock_gettime( CLOCK_REALTIME, &time) == -1 )	// gets the time fro the system
	{
		  perror( "Error at clock gettime" );			// if an error display a message
	}
		
	posRender->render.seconds  = time.tv_sec;
	posRender->render.nseconds = time.tv_nsec/10000000;
	
	#ifdef DEBUG
		cout << "time seconds : " << posRender->render.seconds 
			 << " nseconds : "    << posRender->render.nseconds << endl;
	#endif
		
}
