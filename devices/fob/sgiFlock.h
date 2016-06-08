////////////////////////////////////////////////////////////////////////////////////
// sgiFlock.h                                                                     //
//                                                                                //
// Written by Greg Schmidt/Jeff Finkelstein                                       //
//            gschmidt@cs.tamu.edu                                                //
//            greg@viz.tamu.edu                                                   //
//            www-viz.tamu.edu/students/greg/greg.html                            //
//                                                                                //
// Copyright (c) Greg Schmidt, July 1998                                          //
//               Visualization Laboratory, College of Architecture                //
//               Virtual Environment Robotics Teaching Lab, Computer Science Dept.//
//               Texas A&M University                                             //
//                                                                                //
//               This is "shareware" and not to be used for profit or sale without//
//               explicit agreement from Greg Schmidt and Texas A&M University.   //
//                                                                                //
//               Permission for distribution and use for educational or research  //
//               purposes is granted.                                             //
//                                                                                //
//               An exception is made to Ascension and Jeff Finkelstein, of course//
//               since most of this code was derived from the original Ascension  //
//               Flock of Birds code written by Jeff Finkelstein.                 //
////////////////////////////////////////////////////////////////////////////////////


#ifndef __SGIFLOCK_H__
#define __SGIFLOCK_H__

#include <signal.h>
#include "Defs.h"
#include "extra.h"


#define DEFAULT_BAUD           115200
#define DEFAULT_DEV            "/dev/ttyd"
#define DEFAULT_PORT           2
#define DEFAULT_NUM_SENSORS    1
#define DEFAULT_MODE           POINT
#define DEFAULT_CLOCK_SPEED    144.0
#define DEFAULT_RS232TOFBBADDR 0
#define DEFAULT_NUMFBBADDRS    30

#define MAXPKTS                10
#define BADSIG (void (*)(int))-1



/////////////////////////////////////////////////////////////////////////////////////
// The main class.  This code is by far not minimal.  I duplicated a lot of code to//
// get stuff done fast.  I did attempt to use templates to reduce some of it, but  //
// as it turned out, my idea didn't work.  I guess there may still be another way  //
// to make the code templated, but I will worry about that for the next release.   //
/////////////////////////////////////////////////////////////////////////////////////
// Also another flaw in the design might be that I used a class for one flock unit.//
// It is legal to make multiple instances, but who know what will happen.  That    //
// might be kind of intersting to test out.  But, regardless, the class is a nice  //
// way to organize everything.                                                     //
/////////////////////////////////////////////////////////////////////////////////////

class sgiFlock
{
  private:

    // private variables

    boolean       flock_init;
    short         numSensors;
    int           baud;
    int           baudspeedbits;
    short         port;
    float         clockSpeed;
    int           numDataVals;
    ModeType      mode;
    DataModeType* dataMode;
    float         posk;
    short         rs232tofbbaddr;
    short         numfbbaddrs;
    uchar         fbbaddrbits;
    uchar         cmdbitshft;
    uchar*        fbbsystemstatus;
    uchar         fbbgroupdataflg;
    uchar         displayliststartaddr;
    uchar         displayliststopaddr;
    short         numfbbrcvrs;
    uchar         rs232tofbbstartaddr;
    boolean       dataModeFlag;
    struct termio oldstdin_termio;
    static struct termio oldcom_termio2;    // save the old com config
    struct termio oldcom_termio;    // save the old com config
    struct termio com_termio;       // for the new com config
    static int    comhandle2;
    int           comhandle;
    char*         sys_com_port;
    struct _origin { float pos[3]; } *origin;
    float*        precision;

    // private functions

    boolean       serialInit();
    void          restoreConsole();
    int           getRecord(short,char*,short);
    void          clearRx();
    int           getAddrMode();
    int           sendSerialCmd(uchar*,short);
    int           matchExamineValue(ushort*,ushort);
    int           getsystemstatus();
    void          displayflocksys(uchar*);
    int           setdatamode(uchar,uchar);
    void          constructorHelper(short,int,short);    
    boolean       setmeasurementrate();
    boolean       getmultirecords(uchar,uchar,uchar[],int);
    boolean       readbirddata(short*,short,short);
    boolean       readStream(uchar*,short);
    int           get_serial_record(uchar*,short);
    int           waitforchar();
    int           get_serial_char();    
    void          restoreserialconfig();
    boolean       saveserialconfig();
    boolean       configserialport();
    char          keystroke();
    void          initPosition();


    ////////////////////////////////////////////////////////////////////////////////
    // Shut Down Flock Helper Routine
    ////////////////////////////////////////////////////////////////////////////////

    void          shutDownFlock();

    ////////////////////////////////////////////////////////////////////////////////
    // Get Position Scaling
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean getPositionScaling(short addr, float& posk_)
      { short trs232=rs232tofbbaddr; uchar cmd=3; short numwords=2; uchar parms[2];
        rs232tofbbaddr=addr;
        if(!Examine(cmd, parms, numwords)) return(FALSE);
        rs232tofbbaddr=trs232;
        print("Position Scaling = %d",((int)parms[0]==0?36:72));  
        if(parms[0]==0) posk_ = POSK36; else posk_ = POSK72;
        return(TRUE);
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Get Bird System Status Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean getBirdSystemStatus(short addr, uchar* parms)
      { short trs232=rs232tofbbaddr; uchar cmd=0; short numwords=2;
        rs232tofbbaddr=addr;
        if(!Examine(cmd, parms, numwords)) return(FALSE);
        rs232tofbbaddr=trs232;
        print("Bird System Status (high=%x low=%x)",(int)parms[1],(int)parms[0]);  
        return(TRUE);
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Get Flock Software Revision Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean getSoftwareRevision(short addr, uchar* parms)
      { short trs232=rs232tofbbaddr; uchar cmd=1; short numwords=2;
        rs232tofbbaddr=addr;
        if(!Examine(cmd, parms, numwords)) return(FALSE);
        rs232tofbbaddr=trs232;
        print("Software Revision is #%d.%d", (int)parms[0], (int)parms[1]);
        return(TRUE);
      }

    ///////////////////////////////////////////////////////////////////////////////
    //  getCrystalFreq  
    //
    //  Get the Crystal Frequency from the Bird
    ///////////////////////////////////////////////////////////////////////////////

    inline
    boolean getCrystalFreq(short addr, float& freq)
      { short trs232=rs232tofbbaddr; uchar cmd=2; short numwords=2; uchar parms[2];
        rs232tofbbaddr=addr;
        if(!Examine(cmd, parms, numwords)) return(FALSE);
        rs232tofbbaddr=trs232;
        freq = (float)(int)parms[0];
        print("Crystal Speed is %f MHz", freq);
        return(TRUE);
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Auto Config Routines
    ////////////////////////////////////////////////////////////////////////////////

    boolean       autoConfig();
    boolean       newAutoConfig(uchar*);

    ////////////////////////////////////////////////////////////////////////////////
    // Convert Data Routines
    ////////////////////////////////////////////////////////////////////////////////

    void          convertData(short*,float*,int);
    void          convertData(short*,short*,int);
    void          convertData(short*,char*,int);
    void          convertData(uchar*,float*,int);
    void          convertData(uchar*,short*,int);
    void          convertData(uchar*,char*,int);

    ////////////////////////////////////////////////////////////////////////////////
    // Output Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline                                    // prints message if VERBOSE on
    void print(char* msg) { 
      #ifdef VERBOSE
         fprintf(stdout,"[Output]: %s\n",msg);
      #endif
    }

    inline                                    // prints message if VERBOSE on
    void print(char* msg, int num) {
      #ifdef VERBOSE
         char tmp[MAXSTR];
         sprintf(tmp,"[Output]: %s\n",msg);
         fprintf(stdout, tmp, num);
      #endif
    }

    inline                                    // prints message if VERBOSE on
    void print(char* msg, float num) {
      #ifdef VERBOSE
         char tmp[MAXSTR];
         sprintf(tmp,"[Output]: %s\n",msg);
         fprintf(stdout, tmp, num);
      #endif
    }

    inline                                    // prints message if VERBOSE on
    void print(char* msg, int num1, int num2) {
      #ifdef VERBOSE
         char tmp[MAXSTR];
         sprintf(tmp,"[Output]: %s\n",msg);
         fprintf(stdout, tmp, num1, num2);
      #endif
    }

    inline                                    // prints message if VERBOSE on
    void print(int msg) {
      #ifdef VERBOSE
        fprintf(stdout,"[Output]: %d\n",msg); 
      #endif
    }

    inline
    void output(char* msg)                    // outputs message
      { fprintf(stdout, "%s", msg); }

    inline
    void output(char* msg, int num)           // outputs message
      { fprintf(stdout, msg, num); }


    ////////////////////////////////////////////////////////////////////////////////
    // Abort Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void abort(int n) { exit(n); }            // abort

    ////////////////////////////////////////////////////////////////////////////////
    // Error Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void err(char* msg)                       // prints error message
      { fprintf(stderr, "[Error]: %s\n", msg); }

    inline
    void err(char* msg, int num)              // prints error message
      { fprintf(stderr, "[Error]: %s%d\n", msg, num); }


    ////////////////////////////////////////////////////////////////////////////////
    // Get Data Size
    ////////////////////////////////////////////////////////////////////////////////

    inline
    short getDataSize(int i)
      { short datasize;
        switch (dataMode[i]) {
          case POS:       datasize=3;  break; // position
          case ANGLE:     datasize=3;  break; // angles
          case MATRIX:    datasize=9;  break; // matrix
          case POSANGLE:  datasize=6;  break; // position/angles
          case POSMATRIX: datasize=12; break; // position/matrix
          case QUATER:    datasize=4;  break; // quaternions
          case POSQUATER: datasize=7;  break; // position and quaternions
          default:        datasize=FALSE; err("Illegal datamode"); break;
        } return(datasize);
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Get Bird Data - Get data from multiple birds
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean getbirddata(char* dt, int numPkts)
    //  Parameters Passed:  outputmode - POINT, CONTINUOUS, STREAM
    //			(global) displayliststartaddr - 1st address in
    //				 the group to retrieve data from
    //			(global) displayliststopaddr - last address in
    //				 the group to retrieve data from
    //
    //  Return Value:       TRUE if command executed OK
    //			    FALSE otherwise
    //
    //  Remarks:            Get data from a group of Birds using
    //	            	    the datamode specified
    { uchar startaddr = displayliststartaddr;
      uchar stopaddr  = displayliststopaddr;
      uchar birddata[25 * 30 * MAXPKTS];

      if(getmultirecords(startaddr, stopaddr, birddata, numPkts)) {
        if(!fbbgroupdataflg) convertData((short*)birddata,dt,numPkts);
        else convertData(birddata,dt,numPkts);
      } else return(FALSE);
      return(TRUE);
    }

    inline
    boolean getbirddata(float* dt, int numPkts)
    { uchar startaddr = displayliststartaddr;
      uchar stopaddr  = displayliststopaddr;
      uchar birddata[25 * 30 * MAXPKTS];

      if(getmultirecords(startaddr, stopaddr, birddata, numPkts)) {
        if(!fbbgroupdataflg) convertData((short*)birddata,dt,numPkts);
        else convertData(birddata,dt,numPkts);
      } else return(FALSE);
      return(TRUE);
    }

    inline
    boolean getbirddata(short* dt, int numPkts)
    { uchar startaddr = displayliststartaddr;
      uchar stopaddr  = displayliststopaddr;
      uchar birddata[25 * 30 * MAXPKTS];

      if(getmultirecords(startaddr, stopaddr, birddata, numPkts)) {
        if(!fbbgroupdataflg) convertData((short*)birddata,dt,numPkts);
        else convertData(birddata,dt,numPkts);
      } else return(FALSE);
      return(TRUE);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Read Helper Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean readHelper(char* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Returning read error"); return(7);}
        #ifdef VERBOSE
          for(int j=0;j<numPkts;j++) {
            for(int i=0;i<numDataVals;i++)
              fprintf(stdout,"[%d:%d]: %d, ",j,i,(signed char)dt[j*numDataVals+i]);
            fprintf(stdout, "\n");
          }
          fprintf(stdout, "\n");
        #endif
        return(0);
      }

    inline
    boolean readHelper(float* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Returning read error"); return(7);}
        #ifdef VERBOSE
          for(int j=0;j<numPkts;j++) {
            for(int i=0;i<numDataVals;i++)
              fprintf(stdout,"[%d:%d]: %f, ",j,i,(float)dt[j*numDataVals+i]);
            fprintf(stdout, "\n");
          }
          fprintf(stdout, "\n");
        #endif
        return(0);
      }

    inline
    boolean readHelper(short* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Returning read error"); return(7);}
        #ifdef VERBOSE
          for(int j=0;j<numPkts;j++) {
            for(int i=0;i<numDataVals;i++)
              fprintf(stdout,"[%d:%d]: %d, ",j,i,dt[j*numDataVals+i]);
            fprintf(stdout, "\n");
          }
          fprintf(stdout, "\n");
        #endif
        return(0);
      }


    ////////////////////////////////////////////////////////////////////////////////
    // Print Helper Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean printHelper(char* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Premature termination"); return(7);}
        for(int j=0;j<numPkts;j++) {
          for(int i=0;i<numDataVals;i++)
            fprintf(stdout,"[%d:%d]: %d, ",j,i,(signed char)dt[j*numDataVals+i]);
          fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
        return(0);
      }

    inline
    boolean printHelper(float* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Premature termination"); return(7);}
        for(int j=0;j<numPkts;j++) {
          for(int i=0;i<numDataVals;i++)
            fprintf(stdout,"[%d:%d]: %f, ",j,i,(float)dt[j*numDataVals+i]);
          fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
        return(0);
      }

    inline
    boolean printHelper(short* dt, int numPkts)
      { if(!getbirddata(dt, numPkts)) {err("Premature termination"); return(7);}
        for(int j=0;j<numPkts;j++) {
          for(int i=0;i<numDataVals;i++)
            fprintf(stdout,"[%d:%d]: %d, ",j,i,dt[j*numDataVals+i]);
          fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
        return(0);
      }

    static void interruptHandler(int i) {
      uchar cmd[] = {'B'};
      fprintf(stdout, "Trapped an interrupt, shutting down improperly\n");
      write(comhandle2,cmd,1);
      ioctl(comhandle2,TCFLSH,TIOCFLUSH);
//      sleep(1);
//      ioctl(comhandle2,TCSETA,&oldcom_termio2);    /* restore config */
//      close(comhandle2);    /* close the handle */
      exit(1);
    }

  public:

    ////////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////////

    sgiFlock();                                // init just the instance 
    sgiFlock(short);                           // number of sensors
    sgiFlock(short, int);                      // number of sensors, baud
    sgiFlock(short, short);                    // number of sensors, port
    sgiFlock(short, int, short);               // number of sensors, baud, port

    ////////////////////////////////////////////////////////////////////////////////
    // Destructors
    ////////////////////////////////////////////////////////////////////////////////

    ~sgiFlock();                               // shuts down flock automatically

    ////////////////////////////////////////////////////////////////////////////////
    // Initialization Functions
    ////////////////////////////////////////////////////////////////////////////////

    void    Init();                            // init to default 
    void    Init(short);                       // number of sensors
    void    Init(short, int);                  // number of sensors, baud
    void    Init(short, short);                // number of sensors, port
    void    Init(short, int, short);           // number of sensors, baud, port

    ////////////////////////////////////////////////////////////////////////////////
    // Set Attribute Routines
    ////////////////////////////////////////////////////////////////////////////////

    void    SetReadMode(DataModeType*);     // set read of sensors
    boolean SetMeasurementRate(float);      // set measurement rate
    boolean GroupOn();                      // turn flock GROUP on
    boolean GroupOff();                     // turn flock GROUP off
    boolean StreamMode();                   // turn flock STREAM on
    boolean PointMode();                    // turn flock STREAM off/POINT on
    int     Change(uchar,uchar*,short);     // change value command
    boolean Examine(uchar,uchar*,short&);   // examine value command
    int     Command(uchar, uchar*, short);  // execute command
    boolean Hemisphere(HemisphereType);     // set the hemisphere

    ////////////////////////////////////////////////////////////////////////////////
    // Read Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean Read(char* dt)
      { int numPkts=1;                      // read 1 record as chars
        if(flock_init) return(readHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      } 

    inline
    boolean Read(char* dt, int numPkts)
      { if(flock_init) return(readHelper(dt, numPkts)); // read n records as chars
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Read(short* dt)
      { int numPkts=1;                      // read 1 record as shorts
        if(flock_init) return(readHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Read(short* dt, int numPkts)
      { if(flock_init) return(readHelper(dt, numPkts)); // read n records as shorts
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Read(float* dt)
      { int numPkts=1;                      // read 1 record as floats
        if(flock_init) return(readHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Read(float* dt, int numPkts)
      { if(flock_init) return(readHelper(dt, numPkts)); // read n records as floats
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Read(double* dt)
      { int numPkts=1; return(Read(dt, numPkts)); }
      // read 1 record as doubles

    boolean Read(double*, int);             // read n records as doubles


    ////////////////////////////////////////////////////////////////////////////////
    // Print Routines
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean Print(char* dt)
      { int numPkts=1;                      // read 1 record as chars
        if(flock_init) return(printHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Print(char* dt, int numPkts)
      { if(flock_init) return(printHelper(dt, numPkts)); // read n records as chars
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Print(short* dt)
      { int numPkts=1;                      // read 1 record as shorts
        if(flock_init) return(printHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Print(short* dt, int numPkts)
      { if(flock_init) return(printHelper(dt, numPkts)); // read n records as shorts
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Print(float* dt)
      { int numPkts=1;                      // read 1 record as floats
        if(flock_init) return(printHelper(dt, numPkts));
        err("Flock not initialized"); return(FALSE);
      }

    inline
    boolean Print(float* dt, int numPkts)
      { if(flock_init) return(printHelper(dt, numPkts)); // read n records as floats
        err("Flock not initialized"); return(FALSE);
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Discard Values Routine
    ////////////////////////////////////////////////////////////////////////////////

    boolean Discard(int);                   // read and discard first n values

    ////////////////////////////////////////////////////////////////////////////////
    // Calibration Routines
    ////////////////////////////////////////////////////////////////////////////////

    void    CalibratePosition(int);         // define an origin for the data
    void    UnCalibratePosition(int);       // use an origin of (0,0,0)
    void    SetPositionPrecision(int,float);// sets reading precision in terms of inches

    ////////////////////////////////////////////////////////////////////////////////
    // Status Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    boolean Status()
      { return(flock_init); }               // returns status TRUE=on,FALSE=off

    ////////////////////////////////////////////////////////////////////////////////
    // Set Range Function
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void    SetRange(HardwareType ht)       
      {                                     // set the modes for SRT and ERT
        switch (ht){ case SRT36: posk=POSK36; break; 
                     case SRT72: posk=POSK72; break;
                     case ERT: posk=POSK144; break; 
                     default: break; }}


    ////////////////////////////////////////////////////////////////////////////////
    // Version Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void    Version()
      { // prints the version and author info
         fprintf(stdout, "\nsgiFlock v1.0.5-23-99 (May 1999)\n\n");
         fprintf(stdout, "Written by Greg Schmidt\n");
         fprintf(stdout, "           Jeff Finkelstein\n");
         fprintf(stdout, "           Gary Bamberger\n\n");
      }

    ////////////////////////////////////////////////////////////////////////////////
    // Shut Down Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void    ShutDown()                      // shuts down the flock manually
      { if(flock_init) shutDownFlock(); else err("Flock not initialized"); }

    ////////////////////////////////////////////////////////////////////////////////
    // Trap Interrupt Routine
    ////////////////////////////////////////////////////////////////////////////////

    inline
    void    TrapInterrupt()                      // resets the flock manually
      { if(flock_init) { 
          if(signal(SIGINT, interruptHandler)==BADSIG)
	    { err("signal error"); ShutDown(); }
        } else err("Flock not initialized");
      }

};


#endif // __SGIFLOCK_H__



