
#include "wand.h"

#include "../parsehandlers.h"
#include "../ClientSocket.h"
#include "../SocketException.h"
#include "../communication.h"
#include "../sball.h"

// Includes XML
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include <iostream>

#include <sys/poll.h>

using namespace std;

/* Read the tracker by XML Stream */
void Wand::readtracker() {
	
	static bool     doNamespaces       = false;
	static bool     doSchema           = false;
	static bool     schemaFullChecking = false;
	static SAXParser::ValSchemes    valScheme       = SAXParser::Val_Auto;
		
	posWand[0] = new PosWand();
		
	char tserver[256];
	int port;
	
	
	strcpy(tserver,"localhost");
	port=3030;
	
	
    // Initialize the XML4C system
    try {
         XMLPlatformUtils::Initialize();
    } catch (const XMLException& toCatch) {
         cerr << "Error during initialization! : ";
         cerr << XMLString::transcode(toCatch.getMessage()) << endl;
    }

	std::string reply;

	char  gXMLInMemBuf[1024];
	char  gMemBufId[4] = "fob";
	try {

	ClientSocket *client_socket;
    	SAXParser* parser;
    	PParseHandlers handler;
    	MemBufInputSource* memBufIS;

while(true) {
		
	sleep(10);

	client_socket = new ClientSocket(tserver,port);
	
	*client_socket << "<position/>";
	*client_socket >> reply;
		
    strcpy(gXMLInMemBuf,reply.c_str());

    parser = new SAXParser;
	
    parser->setDocumentHandler(&handler);
    parser->setErrorHandler(&handler);
    parser->setValidationScheme(valScheme);
    parser->setDoNamespaces(doNamespaces);
    parser->setDoSchema(doSchema);
    parser->setValidationSchemaFullChecking(schemaFullChecking);

    memBufIS = new MemBufInputSource (
        (const XMLByte*)gXMLInMemBuf
        , strlen(gXMLInMemBuf)
        , gMemBufId
        , false
    );
   	
    try {
        
		XMLPScanToken token;

		#ifdef DEBUG
			float maxx=0;
			float maxy=0;
			float maxz=0;
			float minx=0;
			float miny=0;
			float minz=0;
		#endif
		
		struct timespec req; // used for nanosleep
		struct timespec rem; // used for nanosleep		
		
		while(true) {

			*client_socket << "<position/>";
		
			*client_socket >> reply;

			#ifdef DEBUG
				cout << reply;
			#endif

			strcpy(gXMLInMemBuf,reply.c_str());
						
			if (!parser->parseFirst(*memBufIS, token)) {
				cerr << "scanFirst() failed\n" << endl;
				XMLPlatformUtils::Terminate();
			}
	
			bool gotMore = true;
			while (gotMore && !parser->getErrorCount()) {
				gotMore = parser->parseNext(token);
			} 
			
			MPI_Status status;
			int received=1;
			
			MPI_Test(&request, &received, &status);
			if(received) {
				MPI_Wait(&request, &status);
				MPI_Irecv(&this->input_update, 1, MPI_INT, MAIN, INPUT_UPDATE, MPI_COMM_WORLD, &this->request);
				
				
				posWand[0]->wand.trans[0]=handler.getposx();
				posWand[0]->wand.trans[1]=handler.getposy();
				posWand[0]->wand.trans[2]=handler.getposz()-5;
				posWand[0]->wand.rotat[0]=-handler.getangz();
				posWand[0]->wand.rotat[1]=handler.getangx();
				posWand[0]->wand.rotat[2]=handler.getangy();
				
				posWand[0]->wand.quaternion[0]=handler.getquaternionw();
				posWand[0]->wand.quaternion[1]=handler.getquaternionx();
				posWand[0]->wand.quaternion[2]=handler.getquaterniony();
				posWand[0]->wand.quaternion[3]=handler.getquaternionz();
				
				posWand[0]->wand.type=handler.gettype();
				
				posWand[0]->head.rotat[0]=posWand[0]->wand.rotat[0];
				posWand[0]->head.rotat[1]=posWand[0]->wand.rotat[1];
				posWand[0]->head.rotat[2]=posWand[0]->wand.rotat[2];

				posWand[0]->wand.buttons=( SBALL_BUTTON_2 );
				
				if(handler.getlft()) {
					#ifdef DEBUG
						cout << "Left button" << endl;
					#endif
					posWand[0]->wand.buttons = SBALL_BUTTON_9 ;
				} else
				
				if(handler.getmid()) {
					#ifdef DEBUG
						cout << "mid button" << endl;
					#endif
					posWand[0]->wand.buttons= SBALL_BUTTON_7 ;
				} else
				
				if(handler.getrgt()) {
					#ifdef DEBUG
						cout << "rig button" << endl;
					#endif
					posWand[0]->wand.buttons= SBALL_BUTTON_8 ;
				} 

				posWand[0]->wand.lixo = posWand[0]->wand.buttons;

				#ifdef DEBUG3
					if (posWand[0]->wand.buttons & SBALL_BUTTON_9) cout << "Left button in wandFOB" << endl;
				#endif

				posWand[0]->wand.wonoff=2;
							
				#ifdef DEBUG
				cout << "FOB : " << 
					posWand[0]->wand.trans[0] << " , " <<
					posWand[0]->wand.trans[1] << " , " <<
					posWand[0]->wand.trans[2] << " , r>  " <<
					posWand[0]->wand.rotat[0] << " , " <<
					posWand[0]->wand.rotat[1] << " , " <<
					posWand[0]->wand.rotat[2] << " , " <<
				endl;
				#endif
				
				MPI_Ssend(&posWand[0]->wand, 1, posWand[0]->wandDatatype, WAND, INPUT_WAND, MPI_COMM_WORLD);
	
			}
			
			#ifdef DEBUG			
			
				cout << "WPos : " << posWand[0]->wand.trans[0] << " , " << posWand[0]->wand.trans[1] << " , " << posWand[0]->wand.trans[2] << endl;
				if(posWand[0]->wand.trans[0]>maxx) maxx = posWand[0]->wand.trans[0]; cout << "max X " << maxx << endl;
				if(posWand[0]->wand.trans[0]<minx) minx = posWand[0]->wand.trans[0]; cout << "max Y " << maxy << endl;
				if(posWand[0]->wand.trans[1]>maxy) maxy = posWand[0]->wand.trans[1]; cout << "max Z " << maxz << endl;
				if(posWand[0]->wand.trans[1]<miny) miny = posWand[0]->wand.trans[1]; cout << "mix X " << minx << endl;
				if(posWand[0]->wand.trans[2]>maxz) maxz = posWand[0]->wand.trans[2]; cout << "mix Y " << miny << endl;
				if(posWand[0]->wand.trans[2]<minz) minz = posWand[0]->wand.trans[2]; cout << "mix Z " << minz << endl;

			#endif

		}
			
        parser->parseReset(token);
			
		req.tv_sec=0;
		req.tv_nsec=0;
		nanosleep(&req,&rem); // this is a critical point in the program and could use too much processament otherwise
		
			
    } catch (const XMLException& toCatch) {
        cerr << "Exception message is: " << endl ;
        cerr << XMLString::transcode(toCatch.getMessage()) << endl;
//        XMLPlatformUtils::Terminate();
    }

}

	delete client_socket;
	
	delete memBufIS;
	delete parser;

    XMLPlatformUtils::Terminate();

	}
	catch ( SocketException& ) {
		//while(true) {
			cout << "Error Tracker daemon on server " << tserver << " port " << port << " not reach !" << endl;
			sleep(5);
		//}
		
	}
	
	delete posWand[0]; 
	
	cout << "Program Aborted by Wand" << endl;
  	MPI_Abort(MPI_COMM_WORLD,0);

}
