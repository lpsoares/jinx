#include "ServerSocket.h"
#include "SocketException.h"
#include <unistd.h>
#include <string>
#include <stdio.h>
#include "timing.h"
#include "sgiFlock.h"

int main ( int argc, int argv[] )
{

  sgiFlock fob;                          // simple constructor

  DataModeType dmt[] = {POSANGLE,POSANGLE};
  float datam[12];
//  float datam[12];
//  int j;
  double delta;
  boolean flag = TRUE;                   // set this to FALSE if want actual timing

  fob.Init(2,115200,3);                  // init routine if simple constructor used
                                         //     set to 1 sensor, 115.2 k baud and
                                         //     port /dev/ttyd2
  fob.SetReadMode(dmt);                  // set the read mode to position/angles
  fob.Hemisphere(HEMI_LOWER);

  fob.GroupOn();                         // turn group mode on


  fob.SetMeasurementRate(144.);          // set the measurement rate to maximum

  fob.StreamMode();                      // turn on streaming mode

  // throw out the first 25 samples
  fob.Discard(25);

  // read 1000 samples (do the timing now that it is running)

  delta = u_realTime();

  cout << "running....\n";

	char teste[1024];
	
  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

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
	
		fob.Read(datam);
//		if(flag)
//		    {

			printf("<track id=\"0\">\n\t<pos>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</pos>\n\t<ang>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</ang>\n</track>\n",
				datam[0],datam[1],datam[2],datam[3],datam[4],datam[5]);
			printf("<track id=\"1\">\n\t<pos>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</pos>\n\t<ang>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</ang>\n</track>\n",
				datam[6],datam[7],datam[8],datam[9],datam[10],datam[11]);
	

			sprintf(teste,"<track id=\"0\">\n\t<pos>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</pos>\n\t<ang>\n\t\t<x>%f</x>\n\t\t<y>%f</y>\n\t\t<z>%f</z>\n\t</ang>\n</track>\n",
				//datam[0],datam[1],datam[2],datam[3],datam[4],datam[5]);
				datam[6],datam[7],datam[8],datam[9],datam[10],datam[11]);
			
			data = teste;
			
			new_sock << data;
//			}
		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }


 delta  = u_realTime();

  fob.PointMode();
  fob.Read(datam);                       // read one in point mode to shutoff stream

  printf("time for 1000 samples = %lf secs\n", delta/(double)1000000.);
  printf("rate = %lf samples/sec\n", (double)1000000000./delta);

  fob.ShutDown();

  return 0;
}
