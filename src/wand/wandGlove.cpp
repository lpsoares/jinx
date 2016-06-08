
#include "wand.h"

#include "../parsehandlers.h"

#include "../ClientSocket.h"
#include "../SocketException.h"
#include "../sball.h"
#include "../communication.h"

// Includes XML
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>



using namespace std;

/* Read the glove by XML Stream */
void Wand::readglove() {
		
	
	
	static bool     doNamespaces       = false;
	static bool     doSchema           = false;
	static bool     schemaFullChecking = false;
	static SAXParser::ValSchemes    valScheme       = SAXParser::Val_Auto;
		
	posWand[0] = new PosWand();
		
	char tserver[256];
	strcpy(tserver,"10.0.200.180");
	int port;
	port=30001;
		
    // Initialize the XML4C system
    try {
         XMLPlatformUtils::Initialize();
    } catch (const XMLException& toCatch) {
         cerr << "Error during initialization! : ";
         cerr << XMLString::transcode(toCatch.getMessage()) << endl;
    }

	std::string reply;

	char  gXMLInMemBuf[1024];
	char  gMemBufId[1024] = "prodInfo";

	try {
		
	ClientSocket client_socket(tserver,port);
	
	client_socket << "<position> ";
	client_socket >> reply;
		
    strcpy(gXMLInMemBuf,reply.c_str());

    SAXParser* parser = new SAXParser;
    PParseHandlers handler;
	
    parser->setDocumentHandler(&handler);
    parser->setErrorHandler(&handler);
    parser->setValidationScheme(valScheme);
    parser->setDoNamespaces(doNamespaces);
    parser->setDoSchema(doSchema);
    parser->setValidationSchemaFullChecking(schemaFullChecking);

    MemBufInputSource* memBufIS = new MemBufInputSource (
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
	
			client_socket << "<position>";
			client_socket >> reply;
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
			
				posWand[0]->wand.trans[0]=(handler.getposx());
				posWand[0]->wand.trans[2]=(handler.getposy()-0.6);
				posWand[0]->wand.trans[1]=(handler.getposz());
				posWand[0]->wand.rotat[1]=handler.getangx()+180;
				posWand[0]->wand.rotat[0]=handler.getangy();
				posWand[0]->wand.rotat[2]=handler.getangz();
				posWand[0]->wand.buttons=( SBALL_BUTTON_2 | SBALL_BUTTON_9 );
				posWand[0]->wand.wonoff=1;
	
				float *fingers;
				fingers = handler.getfingers();
								
				posWand[0]->wand.fingers[0]=fingers[0];
				posWand[0]->wand.fingers[1]=fingers[1];
				posWand[0]->wand.fingers[2]=fingers[2];
				posWand[0]->wand.fingers[3]=fingers[3];
				posWand[0]->wand.fingers[4]=fingers[4];
				posWand[0]->wand.fingers[5]=fingers[5];
				posWand[0]->wand.fingers[6]=fingers[6];
				posWand[0]->wand.fingers[7]=fingers[7];
				posWand[0]->wand.fingers[8]=fingers[8];
				posWand[0]->wand.fingers[9]=fingers[9];
				posWand[0]->wand.fingers[10]=fingers[10];
				posWand[0]->wand.fingers[11]=fingers[11];
				posWand[0]->wand.fingers[12]=fingers[12];
				posWand[0]->wand.fingers[13]=fingers[13];
				posWand[0]->wand.fingers[14]=fingers[14];
				posWand[0]->wand.fingers[15]=fingers[15];
				posWand[0]->wand.fingers[16]=fingers[16];
				posWand[0]->wand.fingers[17]=fingers[17];
				posWand[0]->wand.fingers[18]=fingers[18];
				posWand[0]->wand.fingers[19]=fingers[19];
				
				#ifdef DEBUG

				cout << " FIN: " ;
				for(int ff=0;ff<20;ff++) {
						cout << posWand[0]->wand.fingers[ff] << " , ";
				}
				cout << endl;

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
        XMLPlatformUtils::Terminate();
    }

	delete memBufIS;
	delete parser;

    XMLPlatformUtils::Terminate();

	}
	catch ( SocketException& ) {
		while(true) {
			cout << "Error Tracker daemon on server " << tserver << " port " << port << " not reach !" << endl;
			sleep(5);
		}
		
	}
	
	delete posWand[0]; 
	
	cout << "Program Aborted by Wand" << endl;
  	MPI_Abort(MPI_COMM_WORLD,0);
	exit(0);
}
