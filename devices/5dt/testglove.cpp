
#include "testglove.h"

#include "ServerSocket.h"
#include "SocketException.h"
#include <unistd.h>
#include <string>
#include <stdio.h>

int main() {

	Glove *glove = new Glove();
	
	int      i;
	
	for ( i=0; i<10000000; i++ )
	{
	glove->faz();
	}
}

Glove::~Glove() {

	// Close glove
	cout << "closing glove" << endl;
	fdClose( pGlove );
	cout << "glove closed, goodbye" << endl;
	
} 

Glove::Glove() {
	
	szPort = NULL;
	pGlove    = NULL;
	showraw   = false;
	glovetype = FD_GLOVENONE;
	
	if (!szPort)
	{
#ifdef WIN32
		szPort = "COM1";
#else
		szPort = "/dev/fglove";
#endif
	}

	// Initialize glove
	cout << "Attempting to open glove on " << szPort << " .. ";
	if (NULL == (pGlove = fdOpen( szPort )))
	{
		cout << "failed" << endl;
		
		// RETURN AN ERROR
				
	}
	cout << "succeeded" << endl;

	szType = "?";

	glovetype = fdGetGloveType(pGlove);
	switch (glovetype)
	{
	case FD_GLOVENONE: szType = "None"; break;
	case FD_GLOVE7:    szType = "Glove7"; break;
	case FD_GLOVE7W:   szType = "Glove7W"; break;
	case FD_GLOVE16:   szType = "Glove16"; break;
	case FD_GLOVE16W:  szType = "Glove16W"; break;
	}
	cout << "glove type: " << szType << endl;
	cout << "glove handedness: " << (fdGetGloveHand(pGlove)==FD_HAND_RIGHT?"Right":"Left" ) << endl;
	int iNumSensors = fdGetNumSensors(pGlove);
	cout << "glove num sensors: " << iNumSensors << endl;
	// Display glove info
	unsigned char buf[64];
	fdGetGloveInfo( pGlove, buf );
	cout << "glove info: " << (char*)buf << endl;

	
}

/*--------------------------------------------------------------------------*/


void Glove::faz( )
{
	
	float datam[20];
	
	cout << "running....\n";

	char teste[1024];
	
	try
	{
		// Create the socket
		ServerSocket server ( 30001 );

		while ( true )
		{

		ServerSocket new_sock;
		server.accept ( new_sock );

		try
		{
			while ( true )
			{ 
				
				std::string data;
				new_sock >> data;

				#ifdef WIN32
					Sleep(15);
				#else
					usleep(15000); // fixme
				#endif

				
		if (showraw)
		{
			if (glovetype==FD_GLOVE7 || glovetype==FD_GLOVE7W)
			{
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_INDEXNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_MIDDLENEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_RINGNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_LITTLENEAR));
			}
			else
			{
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_THUMBNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_THUMBFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBINDEX));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_INDEXNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_INDEXFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_INDEXMIDDLE));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_MIDDLENEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_MIDDLEFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_MIDDLERING));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_RINGNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_RINGFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_RINGLITTLE));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_LITTLENEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_LITTLEFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBPALM));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_WRISTBEND));
				
			}
			printf( "r=%4d,p=%4d",
				(int)fdGetSensorRaw(pGlove,FD_ROLL),
				(int)fdGetSensorRaw(pGlove,FD_PITCH));
		}
		else // show scaled values
		{
			if (glovetype==FD_GLOVE7 || glovetype==FD_GLOVE7W)
			{
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_THUMBNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_INDEXNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_MIDDLENEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_RINGNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_LITTLENEAR));
			}
			else
			{
				
				datam[0]=fdGetSensorScaled(pGlove,FD_THUMBNEAR);
				datam[1]=fdGetSensorScaled(pGlove,FD_THUMBFAR);
				datam[2]=fdGetSensorScaled(pGlove,FD_THUMBINDEX);
				datam[3]=fdGetSensorScaled(pGlove,FD_INDEXNEAR);
				datam[4]=fdGetSensorScaled(pGlove,FD_INDEXFAR);
				datam[5]=fdGetSensorScaled(pGlove,FD_INDEXMIDDLE);
				datam[6]=fdGetSensorScaled(pGlove,FD_MIDDLENEAR);
				datam[7]=fdGetSensorScaled(pGlove,FD_MIDDLEFAR);
				datam[8]=fdGetSensorScaled(pGlove,FD_MIDDLERING);
				datam[9]=fdGetSensorScaled(pGlove,FD_RINGNEAR);
				datam[10]=fdGetSensorScaled(pGlove,FD_RINGFAR);
				datam[11]=fdGetSensorScaled(pGlove,FD_RINGLITTLE);
				datam[12]=fdGetSensorScaled(pGlove,FD_LITTLENEAR);
				datam[13]=fdGetSensorScaled(pGlove,FD_LITTLEFAR);
				datam[14]=fdGetSensorScaled(pGlove,FD_THUMBPALM);
				datam[15]=fdGetSensorScaled(pGlove,FD_WRISTBEND);
				
/*				
				printf("%.1f,", datam[0]);
				printf("%.1f,", datam[1]);
				printf("%.1f ", datam[2]);
				printf("%.1f ", datam[3]);
				printf("%.1f ", datam[4]);
				printf("%.1f ", datam[5]);
				printf("%.1f ", datam[6]);
				printf("%.1f ", datam[7]);
				printf("%.1f ", datam[8]);
				printf("%.1f ", datam[9]);
				printf("%.1f ", datam[10]);
				printf("%.1f ", datam[11]);
				printf("%.1f ", datam[12]);
				printf("%.1f ", datam[13]);
				printf("%.1f ", datam[14]);
				printf("%.1f ", datam[15]);
	*/			
			}
			
				datam[16]=fdGetSensorScaled(pGlove,FD_ROLL),
				datam[17]=fdGetSensorScaled(pGlove,FD_PITCH);
		/*	
			printf( "r=%.1f,p=%.1f", datam[16], datam[17]);
		*/
		
		}
		printf(" %d\n", fdGetGesture(pGlove));
						
			std::string value[20];
			
			int f;
			for(f=0;f<20;f++) {
				sprintf(teste,"%f",datam[f]);
				value[f]=teste;
			}
				
			string data2;
			
			string a = "\n";

			data2 = a + "<track id=\"0\">\n" +
						 "\t<pos>\n" +
						 		"\t\t<x>" + "0" + "</x>\n" +
								"\t\t<y>" + "0" + "</y>\n" + 
								"\t\t<z>" + "0" + "</z>\n" +
							"\t</pos>\n" +
							"\t<ang>\n" + 
								"\t\t<x>" + "0" + "</x>\n" +
								"\t\t<y>" + "0" + "</y>\n" +
								"\t\t<z>" + "0" + "</z>\n" +
							"\t</ang>\n" +
							"\t<finger>\n" + 
								"\t\t<f0>"  + value[0]  + "</f0>\n"  +
								"\t\t<f1>"  + value[1]  + "</f1>\n"  +
								"\t\t<f2>"  + value[2]  + "</f2>\n"  +
								"\t\t<f3>"  + value[3]  + "</f3>\n"  +
								"\t\t<f4>"  + value[4]  + "</f4>\n"  +
								"\t\t<f5>"  + value[5]  + "</f5>\n"  +
								"\t\t<f6>"  + value[6]  + "</f6>\n"  +
								"\t\t<f7>"  + value[7]  + "</f7>\n"  +
								"\t\t<f8>"  + value[8]  + "</f8>\n"  +
								"\t\t<f9>"  + value[9]  + "</f9>\n"  +
								"\t\t<f10>" + value[10] + "</f10>\n" +
								"\t\t<f11>" + value[11] + "</f11>\n" +
								"\t\t<f12>" + value[12] + "</f12>\n" +
								"\t\t<f13>" + value[13] + "</f13>\n" +
								"\t\t<f14>" + value[14] + "</f14>\n" +
							"\t</finger>\n" +
							"</track>\n";

				new_sock << data2;

				cout << data2 ;

				//data = data2;

				//new_sock << data;

		}
	}
	catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
		
		
	

}

