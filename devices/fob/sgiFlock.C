////////////////////////////////////////////////////////////////////////////////////
// sgiFlock.C                                                                     //
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

#include <stdio.h>
#include <assert.h>
#include "sgiFlock.h"

////////////////////////////////////////////////////////////////////////////////////
// static initializations
////////////////////////////////////////////////////////////////////////////////////

int           sgiFlock::comhandle2     = 0;
struct termio sgiFlock::oldcom_termio2;

////////////////////////////////////////////////////////////////////////////////////
// constructors
////////////////////////////////////////////////////////////////////////////////////

sgiFlock::sgiFlock()
{
  flock_init = FALSE; 
}

sgiFlock::sgiFlock(short ns)
{
  constructorHelper(ns, DEFAULT_BAUD, DEFAULT_PORT);
}

sgiFlock::sgiFlock(short ns, int bd)
{
  constructorHelper(ns, bd, DEFAULT_PORT);
}

sgiFlock::sgiFlock(short ns, short pt)
{
  constructorHelper(ns, DEFAULT_BAUD, pt);
}

sgiFlock::sgiFlock(short ns, int bd, short pt)
{
  constructorHelper(ns, bd, pt);
}


////////////////////////////////////////////////////////////////////////////////////
// destructor
////////////////////////////////////////////////////////////////////////////////////

sgiFlock::~sgiFlock()
{
  if(flock_init) shutDownFlock();
  else err("Flock not initialized");
}


////////////////////////////////////////////////////////////////////////////////////
// Initialization functions
////////////////////////////////////////////////////////////////////////////////////

void sgiFlock::Init(short ns, int bd, short pt)
{
  constructorHelper(ns, bd, pt);
}

void sgiFlock::Init(short ns, short pt)
{
  constructorHelper(ns, DEFAULT_BAUD, pt);
}

void sgiFlock::Init(short ns, int bd)
{
  constructorHelper(ns, bd, DEFAULT_PORT);
}

void sgiFlock::Init(short ns)
{
  constructorHelper(ns, DEFAULT_BAUD, DEFAULT_PORT);
}

void sgiFlock::Init()
{
  constructorHelper(DEFAULT_NUM_SENSORS, DEFAULT_BAUD, DEFAULT_PORT);
}


////////////////////////////////////////////////////////////////////////////////////
// Calibration functions
////////////////////////////////////////////////////////////////////////////////////

void sgiFlock::CalibratePosition(int sensor)
{
  float corner1[6];
  float corner2[6];
  float prec = precision[sensor-1];

  if(!flock_init) { err("Flock not initialized"); return; }
  
  // reset the precision to 1.0 to get the proper reading in inches
  precision[sensor-1] = 1.0;

  output("\nCalibrating sensor #%d\n\n", sensor);

  // get first corner
  output("Imagine you have a 3D bounding box around your workspace for this sensor.\n"); 
  output("Put the sensor at one of the corners.\n");
  output("Press a key when ready to record."); keystroke();
  readHelper(corner1, 1);

  // get second corner
  output("Put the sensor at the opposite corner along the main diagonal.\n");
  output("Press a key when ready to record."); keystroke();
  readHelper(corner2, 1);

  origin[sensor-1].pos[0] = (corner2[0] + corner1[0])/2.;
  origin[sensor-1].pos[1] = (corner2[1] + corner1[1])/2.;
  origin[sensor-1].pos[2] = (corner2[2] + corner1[2])/2.;

  // reset the precision
  precision[sensor-1] = prec;

  output("Your relative origin has been set.\n"); 
}

///////////////////////////////////////////////////////////////////////////////////
// UnCalibratePosition
//
// Simply reset the relative origin 
// to the absolute origin (0,0,0) for sensor specified.
///////////////////////////////////////////////////////////////////////////////////
void sgiFlock::UnCalibratePosition(int sensor)
{
  if(flock_init) for(int i=0;i<3;i++) origin[sensor-1].pos[i] = 0.0;
  else err("Flock not initialized");
}

///////////////////////////////////////////////////////////////////////////////////
// SetPositionPrecision
//
// Set the precision to units specified
///////////////////////////////////////////////////////////////////////////////////
void sgiFlock::SetPositionPrecision(int sensor, float prec)
{
  if(flock_init) // number of sensor known
  {
    assert(sensor>0 && sensor <= numSensors && prec > 0.0);
    precision[sensor-1] = prec;
  }
  else err("Flock not initialized");
}

///////////////////////////////////////////////////////////////////////////////////
// Discard           : data should be an array of size specified in SetReadMode
//                   : this reads the data and discards (used to clean out system) 
//                   : the exact order depends upon your setup, etc...
//                   : this returns something other than 0 if an error occurred
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::Discard(int numPkts)
{
  float *dt;
  int i;
  int localNumPkts = 1;

  dt = new float[numDataVals]; 

  for(i=0;i<numPkts;i++)
    if(!readHelper(dt, localNumPkts)) return(FALSE);

  return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////////
// Read              : data should be an array of size specified in SetReadMode
//                   : this returns position and orientation information
//                   : the exact order depends upon your setup, etc...
//                   : this returns something other than 0 if an error occurred
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::Read(double *dt, int numPkts)
{

printf(" TESTE \n");

  boolean resultVal;
  int sz = sizeof(dt)/sizeof(double);
  float *dt2;
  int i, j;

  dt2 = new float[sz];

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  resultVal = readHelper(dt2, numPkts);

  for(j=0;j<numPkts;j++)
    for(i=0;i<numDataVals;i++)
      dt[j*numDataVals+i] = (double)dt2[j*numDataVals+i];

  delete(dt2);

  return(resultVal);
}


//
// private functions
//


//////////////////////////////////////////////////////////////////////////////////
// keystroke
//
// Code adapted from Advanced Unix Programming by Rochkind
//////////////////////////////////////////////////////////////////////////////////
char sgiFlock::keystroke()
{
  struct termio tbuf;
  struct termio tbufsave;
  char key;

  if(ioctl(fileno(stdin), TCGETA, &tbuf) == -1)
    err("keystroke: ioctl<1> failed");

  tbufsave = tbuf;
  tbuf.c_iflag &= ~(INLCR|ICRNL|IUCLC|ISTRIP|IXON|BRKINT);
  tbuf.c_oflag &= ~OPOST;
  tbuf.c_lflag &= ~(ICANON|ISIG|ECHO);
  tbuf.c_cc[4] = 5;
  tbuf.c_cc[5] = 2;

  if(ioctl(fileno(stdin), TCSETAF, &tbuf) == -1)
    err("keystroke: ioctl<2> failed");

  key = fgetc(stdin);

  // reset stdin

  if(ioctl(fileno(stdin), TCSETAF, &tbufsave) == -1)
    err("keystroke: ioctl<3> failed");

  return key;
}


//////////////////////////////////////////////////////////////////////////////////
// shutDownFlock
//////////////////////////////////////////////////////////////////////////////////
void sgiFlock::shutDownFlock()
{
  print("Shutting down flock.");
  restoreserialconfig();
  delete((uchar*)fbbsystemstatus);
  delete((char*)sys_com_port);
  delete((struct _origin*)origin);
  delete((float*)precision);
}

////////////////////////////////////////////////////////////////////////////////////
//  restoreserialconfig -   Restore the original serial port configuration
//
//  Remarks:            restores the configuration of the serial port
////////////////////////////////////////////////////////////////////////////////////
void sgiFlock::restoreserialconfig()
{
  // Restore the Com Port Configuration.. if already opened
  if(comhandle != -1)
  {
//    ioctl(comhandle,TCSETA,&oldcom_termio);    /* restore config */

//    close(comhandle);    /* close the handle */
    comhandle = -1;        /* make the comhandle 'look' closed */
  }
}

////////////////////////////////////////////////////////////////////////////////////
//  saveserialconfig    -   save serial port configuration
//
//  Remarks:            saves the current configuration of the serial port
////////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::saveserialconfig()
{
  // if already opened, return FALSE
  if(comhandle != -1)
    return(FALSE);

  //  Open the Comport for RD and WR and get a handle
  if((comhandle = open(sys_com_port, OPENPARAMS)) == -1)
  {
    err("Could not open the COM port.");
    return(FALSE);
  }
  else // store in static variables
  {
    sgiFlock::comhandle2 = comhandle;
    ioctl(sgiFlock::comhandle2, TCGETA, &sgiFlock::oldcom_termio2);
  }

  // Save the Current Com Port Configuration
  if(ioctl(comhandle,TCGETA,&oldcom_termio) >= 0)
  {
    // Setup the new port configuration...NON-CANONICAL INPUT MODE
    // .. as defined in termio.h

    com_termio.c_iflag = XOFF;
    com_termio.c_cflag = CS8 | CLOCAL | CREAD;
    com_termio.c_lflag = 0;
    com_termio.c_cc[VMIN] = 0;     // setup to return after 2 seconds
    com_termio.c_cc[VTIME] = 20;   // ..if no characters are received 
                                 // TIME units are assumed to be 0.1 secs 
    for(int i=0;i<10;i++)
      if(ioctl(comhandle,TCSETA,&com_termio) >= 0)
        return(TRUE);
  }

  // Put up the Error and return

  err("Could not configure the COM port to RAW mode");

  return(FALSE);
}


////////////////////////////////////////////////////////////////////////////////////
//  configserialport    -   Configure the Serial Port connected to the BIRD
//
//  Return Value:       TRUE if successfull, else FALSE
////////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::configserialport()
{
  struct termio tempcom_termio;

  // Get the Current Com Port Configuration

  if(ioctl(comhandle,TCGETA,&tempcom_termio) >= 0)
  {
    //
    // old TERMIOS (Set the flag -D_OLD_TERMIOS when compiling )
    //
    #ifdef _OLD_TERMIOS
       tempcom_termio.c_cflag |= baudspeedbits;
    #else
       tempcom_termio.c_ospeed = baudspeedbits;
       tempcom_termio.c_ispeed = baudspeedbits;
    #endif

    if(ioctl(comhandle,TCSETA,&tempcom_termio) >= 0)
      return(TRUE);
  }

  err("Could not set the COM port Baud Rate");
  return(FALSE);
}



/////////////////////////////////////////////////////////////////////////////////////
//
//  serialInit          Serial Port Initialization
//
//  Return Value:       TRUE if successful
//			FALSE if unsuccessful
//
//  Remarks:            Routine prompts the user for the serial port
//			configuration parameters of COM1 or COM2 and
//			tries to configure the port via configserialport()
/////////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::serialInit()
{
  // Must be a Com Port Change

  restoreserialconfig();
  if(!saveserialconfig())
  {
    err("Error trying to save serial configuration.");
    return(FALSE);
  }

  // Configure the Serial Port Hardware
  if(!configserialport())
  {
    err("Problem with initializing serial port");
//    hitkeycontinue();
    return(FALSE);
  }
  return(TRUE);
}




//////////////////////////////////////////////////////////////////////////////////
//  readStream          -   Read Bird Data and convert to 2's complement
//
//  Parameters Passed:  birddata    - pointer to integer array to store data
//			numwords    - number of words (16 bits) to read
//			outputmode  - POINT, CONTINUOUS, or STREAM
//			buttonmode  - 0 if off, 1 if on (send button value)
//
//  Return Value:       TRUE if successful
//			FALSE otherwise
//
//  Remarks:            routine reads a record from the bird, and adjusts
//			the least and most significant Bird bytes into 2's
//			complemented words.
//////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::readStream(uchar* birddata, short numwords)
{
  // Read 2*numbwords characters from the bird, check for errors

  if((get_serial_record(birddata, numwords)) < 0)
  {
    err("Could not read record data from the bird.");
    return(FALSE);
  }

  return(TRUE);
}

//////////////////////////////////////////////////////////////////////////////////
//  newAutoConfig
//
//  examine the current status of some important parameters, for now, and then
//  act if necessary to auto-config the current setup
//  note: this assumes we know the list of birds in displaylist vars (poor
//        way of doing it, but more efficient for now
//////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::newAutoConfig(uchar* status)
{
  uchar   parms[2];
  short   i;
  float   freq;
  boolean ercflag = FALSE;

  // check for erc
  for(i=0;i<numfbbaddrs;i++)
    if((status[i] & 0x80) && (status[i] & 0x10)) ercflag=TRUE;

  for(i=0;i<numfbbaddrs;i++)
  {
    // check to see if running only (could be an CONTRLR or RECVR)
    if(status[i] & 0x40) // running
    {
      print("Status for Bird at Addr #%d", i);

      if(!getBirdSystemStatus(i, parms))  return(FALSE);
      if(!getSoftwareRevision(i, parms))  return(FALSE);
      if(!getCrystalFreq(i, freq))        return(FALSE);

      //
      // handle position scaling
      //
      if(ercflag) { posk=POSK144; print("Position Scaling = 144"); }
      else if(!getPositionScaling(i, posk)) return(FALSE);
    }
  }

  return(TRUE);
}


void sgiFlock::constructorHelper(short ns, int bd, short pt)
{
  DataModeType* localDataMode;
  int           i;

  numSensors           = ns;
  baud                 = bd;

  switch (baud)
  {
    #ifndef _OLD_TERMIOS
    case 115200: baudspeedbits = B115200; break;
    #endif
    case 38400:  baudspeedbits = B38400;  break;
    case 19200:  baudspeedbits = B19200;  break;
    case 9600:   baudspeedbits = B9600;   break;
    default:                              break;
  }

  port                 = pt;

  sys_com_port         = new char[MAXSTR];
  sprintf(sys_com_port, "/dev/ttyd%d", port);

  clockSpeed           = DEFAULT_CLOCK_SPEED;
  mode                 = DEFAULT_MODE;
  rs232tofbbaddr       = DEFAULT_RS232TOFBBADDR;
  numfbbaddrs          = DEFAULT_NUMFBBADDRS;
  fbbaddrbits          = 0;
  fbbgroupdataflg      = FALSE;
  displayliststartaddr = 1;
  displayliststopaddr  = 0;
  rs232tofbbstartaddr  = 99;
  dataModeFlag         = FALSE;
  numDataVals          = 0;
  comhandle            = -1;

  // set the origin to the initialized mode by default
  initPosition();

  dataMode        = new DataModeType[ns];
  precision       = new float[ns];
  for(i=0;i<ns;i++) precision[i] = 1.0;

  fbbsystemstatus = new uchar[DEFAULT_NUMFBBADDRS+1];

  flock_init = TRUE;

  if(!serialInit())
    { err("Could Not Init Serial Port."); restoreConsole(); abort(-1); }

  clearRx();
  autoConfig();
  if(!newAutoConfig(fbbsystemstatus))
    { err("New Auto Config Failed"); exit(1); }

  // optional setting up the readmode to default POS/ANGLE for all

  localDataMode = new DataModeType[ns];
  for(i=0;i<ns;i++)
    localDataMode[i] = POSANGLE;
  SetReadMode(localDataMode);
  delete(localDataMode);

}

///////////////////////////////////////////////////////////////////////////////////
// initPosition                                                                  //
///////////////////////////////////////////////////////////////////////////////////
void sgiFlock::initPosition()
{
  int index;
  int coord;

  origin = new struct _origin[numSensors];

  for(index = 0; index < numSensors; index++)
   for(coord = 0; coord < 3; coord++)
     origin[index].pos[coord] = 0.0;
}


void sgiFlock::restoreConsole()
{
  ioctl(fileno(stdin),TCSETA,&oldstdin_termio);
}


//////////////////////////////////////////////////////////////////////////////////
//  get_serial_record   - Get a record from the serial IO Buffer
//
//  Parameters Passed:  rxbuf       -   pointer to a buffer to store the
//                                      received characters
//                      recsize     -   number of characters to receive
//                      outputmode  -   POINT, CONTINUOUS or STREAM
//
//  Return Value:       If successful, returns recsize
//                      else, RXERRORS if Rx errors were detected while
//                      receiving data
//
//  Remarks:            A record of data has the MSB of the first
//                      character set to a 1.  The routine verifies that
//                      the first character received is in PHASE.
//
//                      NOTE that this routine returns the MOST RECENT
//                      record from the Buffer...if more than one record
//                      is present, old records are discarded.
//////////////////////////////////////////////////////////////////////////////////
int sgiFlock::get_serial_record(uchar* rxbuf, short recsize)
{
  int   charsrdy = 0;
  short numrecordsrdy;
  int   totalcharsrdy;

  totalcharsrdy = 0;

  // Get the Number of Chars Ready
  //
  // Note that this approach reads the MOST RECENT record received by the
  // host computer by manually emptying the operating system's serial
  // character buffer until the last valid record

  if(ioctl(comhandle,TIONREAD,&charsrdy))
  {
    err("IOCTL returned an error");
    return(RXERRORS);
  }

  totalcharsrdy += charsrdy;

  // Now take care of 3 cases...
  //   1) charsrdy = recsize
  //   2) charsrdy > recsize
  //   3) charsrdy < recsize

  if(totalcharsrdy >= recsize)
  {
    // Determine the number of records available
    // If the number is greater than 1 then throw away old records

    if(recsize)
      numrecordsrdy = totalcharsrdy/recsize;
    else
    {
      err("Illegal parameter in call to get_serial_record");
      return(RXERRORS);
    }

    // Read the most recent complete Block from the Port

    while(numrecordsrdy--)
    {
      if(getRecord(recsize,(char *) rxbuf,TRUE) == RXERRORS)
        return(RXERRORS);
    }
  }
  else
  {
    return(getRecord(recsize,(char *) rxbuf,TRUE));
  }

  // Everything is OK, so return the recsize

  return(recsize);
}


//////////////////////////////////////////////////////////////////////////////////
// convertData                                                                  //
//////////////////////////////////////////////////////////////////////////////////
void sgiFlock::convertData(short* bdata, float* dt, int numPkts)
{
  int i, j, k, m, l;
  
  for(l=0,k=0,m=0;l<numPkts;l++)
  {
    for(j=0;j<numSensors;j++)
    {
      switch (dataMode[j])
      {
        case POS:
          for(i=0;i<3;i++)
     	    dt[i+k] = (float)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);
          k += 3; m += 3;
          break;

        case POSANGLE:
          for(i=0;i<3;i++)
            dt[i+k] = (float)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);

          for(i=3;i<6;i++)
            dt[i+k] = (float)(bdata[i+m] * ANGK);
          k += 6; m += 6;
          break;

        case ANGLE:
          for(i=0;i<3;i++)
            dt[i+k] = (float)(bdata[i+m] * ANGK);
          k += 3; m += 3;
          break;

        case MATRIX:
          for(i=0;i<9;i++)
            dt[i+k] = (float)(bdata[i+m] / FTW);
          k += 9; m += 9;
          break;

        case POSMATRIX:
          for(i=0;i<3;i++)
            dt[i+k] = (float)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);

          for(i=3;i<12;i++)
            dt[i+k] = (float)(bdata[i+m] / FTW);
          k += 12; m += 12;
          break;

        case QUATER:
          for(i=0;i<4;i++)
            dt[i+k] = (float)(bdata[i+m] / FTW);
          k += 4; m += 4;
          break;

        case POSQUATER:
          for(i=0;i<3;i++)
            dt[i+k] = (float)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);

          for(i=3;i<7;i++)
            dt[i+k] = (float)(bdata[i+m] / FTW);
          k += 7; m += 7;
          break;

        default: break;
      }
    }
  } 
}

void sgiFlock::convertData(short* bdata, short* dt, int numPkts)
{
  int i, j, k, m, l;
  
  for(l=0,k=0,m=0;l<numPkts;l++)
  {
    for(j=0;j<numSensors;j++)
    {
      switch (dataMode[j])
      {
        case POS:
          for(i=0;i<3;i++)
     	    dt[i+k] = (short)(((bdata[i+m]*posk) - origin[j].pos[i])/precision[j]+0.5);
          k += 3; m += 3;
          break;

        case POSANGLE:
          for(i=0;i<3;i++)
            dt[i+k] = (short)(((bdata[i+m]*posk) - origin[j].pos[i])/precision[j]+0.5);

          for(i=3;i<6;i++)
            dt[i+k] = (short)(bdata[i+m] * ANGK);
          k += 6; m += 6;
          break;

        case ANGLE:
          for(i=0;i<3;i++)
            dt[i+k] = (short)(bdata[i+m] * ANGK);
          k += 3; m += 3;
          break;

        case MATRIX:
          for(i=0;i<9;i++)
            dt[i+k] = (short)(bdata[i+m]);
          k += 9; m += 9;
          break;

        case POSMATRIX:
          for(i=0;i<3;i++)
            dt[i+k] = (short)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);

          for(i=3;i<12;i++)
            dt[i+k] = (short)(bdata[i+m]);
          k += 12; m += 12;
          break;

        case QUATER:
          for(i=0;i<4;i++)
            dt[i+k] = (short)(bdata[i+m]);
          k += 4; m += 4;
          break;

        case POSQUATER:
          for(i=0;i<3;i++)
            dt[i+k] = (short)(((bdata[i+m] * posk) - origin[j].pos[i])/precision[j]);

          for(i=3;i<7;i++)
            dt[i+k] = (short)(bdata[i+m]);
          k += 7; m += 7;
          break;

        default: break;
      }
    }
  } 
}

void sgiFlock::convertData(short* bdata, char* dt, int numPkts)
{
  int i, j, k, m, l;
  
  for(l=0,k=0,m=0;l<numPkts;l++)
  {
    for(j=0;j<numSensors;j++)
    {
      switch (dataMode[j])
      {
        case POS:
          for(i=0;i<3;i++)
     	    dt[k+i] = (char)((int)(((bdata[i+m] * posk) - origin[j].pos[i])/
			     precision[j]+0.5));
          k += 3; m += 3;
          break;

        case POSANGLE:
          for(i=0;i<3;i++)
     	    dt[k+i] = (char)((int)(((bdata[i+m] * posk) - origin[j].pos[i])/
			     precision[j]+0.5));

          for(i=3;i<6;i++)
            dt[i+k] = (char)(bdata[i+m] * ANGK);
          k += 6; m += 6;
          break;

        case ANGLE:
          for(i=0;i<3;i++)
            dt[i+k] = ( char)(bdata[i+m] * ANGK);
          k += 3; m += 3;
          break;

        case MATRIX:
          for(i=0;i<9;i++)
            dt[i+k] = (char)((int)(bdata[i+m] / FTW)*128);
          k += 9; m += 9;
          break;

        case POSMATRIX:
          for(i=0;i<3;i++)
            dt[i+k] = (char)((int)(((bdata[i+m] * posk) - origin[j].pos[i])/
				  precision[j]));

          for(i=3;i<12;i++)
            dt[i+k] = (char)((int)(bdata[i+m] / FTW)*128);
          k += 12; m += 12;
          break;

        case QUATER:
          for(i=0;i<4;i++)
            dt[i+k] = (char)((int)(bdata[i+m] / FTW)*128);
          k += 4; m += 4;
          break;

        case POSQUATER:
          for(i=0;i<3;i++)
            dt[i+k] = (char)((int)(((bdata[i+m] * posk) - origin[j].pos[i])/
				   precision[j]));

          for(i=3;i<7;i++)
            dt[i+k] = (char)((int)(bdata[i+m] / FTW)*128);
          k += 7; m += 7;
          break;

        default: break;
      }
    }
  } 
}

//////////////////////////////////////////////////////////////////////////////////
// convertData                                                                  //
//////////////////////////////////////////////////////////////////////////////////
void sgiFlock::convertData(uchar* bdata, float* dt, int numPkts)
{
  int i, j, k, l;
  short datasize;
  uchar xx, yy;
  short xxs, yys, xxss, yyss, xy, xys;

  for(l=0,k=0;l<numPkts;l++)
  {
    for(i=0; i<numSensors; i++)
    {
      datasize = getDataSize(i);

      for(j=0;j<2*datasize;j+=2,k++)
      {
        xx   = *bdata;      xxs  = (short)xx;  xxss = xxs & 0x7F;
        yy   = *(bdata+1);  yys  = (short)yy;  yyss = yys << 7;
        xy   = xxss | yyss; xys  = (xy << 2);
        bdata++; bdata++;

        switch (dataMode[i])
        {
          case POS:
     	    dt[k] = (float)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            break;
 
          case POSANGLE:
            if(j<6) // position
              dt[k] = (float)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            else
              dt[k] = (float)(xys * ANGK);
            break;

          case ANGLE:     dt[k] = (float)(xys * ANGK); break;
          case MATRIX:    dt[k] = (float)(xys / FTW);  break;
          case QUATER:    dt[k] = (float)(xys / FTW);  break;

          case POSMATRIX: 
            if(j<6) // position
              dt[k] = (float)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            else
              dt[k] = (float)(xys / FTW);
            break;

          case POSQUATER: 
            if(j<6) // position
              dt[k] = (float)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            else
              dt[k] = (float)(xys / FTW);
            break;

          default: break;
        }
      }

      // eliminate extra char (FBB addr)
      bdata++;
    }
  }
}

void sgiFlock::convertData(uchar* bdata, char* dt, int numPkts)
{
  int i, j, k, l;
  short datasize;
  uchar xx, yy;
  short xxs, yys, xxss, yyss, xy, xys;

  for(k=0,l=0;l<numPkts;l++)
  {
    for(i=0; i<numSensors; i++)
    {
      datasize = getDataSize(i);

      for(j=0;j<2*datasize;j+=2,k++)
      {
        xx   = *bdata;        xxs  = (short)xx;        xxss = xxs & 0x7F;
        yy   = *(bdata+1);    yys  = (short)yy;        yyss = yys << 7;
        xy   = xxss | yyss;   xys  = (xy << 2);
        bdata++; bdata++;

        switch (dataMode[i])
        {
          case POS:
     	    dt[k] = (char)((int)(((xys*posk) - origin[i].pos[k%3])/precision[i]+0.5));
            break;
 
          case POSANGLE:
            if(j<6) // position
     	      dt[k] = (char)((int)(((xys*posk) - origin[i].pos[k%3])/precision[i]+0.5));
            else
              dt[k] = (char)(xys * ANGK);
            break;

          case ANGLE:     dt[k] = (char)(xys * ANGK); break;
          case MATRIX:    dt[k] = (char)((int)(xys / FTW)*128);  break;
          case QUATER:    dt[k] = (char)((int)(xys / FTW)*128);  break;

          case POSMATRIX: 
            if(j<6) // position
              dt[k] = (char)((int)(((xys * posk) - origin[i].pos[k%3])/precision[i]));
            else
              dt[k] = (char)((int)(xys / FTW)*128);
            break;

          case POSQUATER: 
            if(j<6) // position
              dt[k] = (char)((int)(((xys * posk) - origin[i].pos[k%3])/precision[i]));
            else
              dt[k] = (char)((int)(xys / FTW)*128);
            break;

          default: break;
        }
      }

      // eliminate extra char (FBB addr)
      bdata++;
    }
  }
}

void sgiFlock::convertData(uchar* bdata, short* dt, int numPkts)
{
  int i, j, k, l;
  short datasize;
  uchar xx, yy;
  short xxs, yys, xxss, yyss, xy, xys;

  for(k=0,l=0;l<numPkts;l++)
  {
    for(i=0; i<numSensors; i++)
    {
      datasize = getDataSize(i);

      for(j=0;j<2*datasize;j+=2,k++)
      {
        xx   = *bdata;      xxs  = (short)xx;      xxss = xxs & 0x7F;
        yy   = *(bdata+1);  yys  = (short)yy;      yyss = yys << 7;
        xy   = xxss | yyss; xys  = (xy << 2);
        bdata++; bdata++;

        switch (dataMode[i])
        {
          case POS:
     	    dt[k] = (short)(((xys * posk) - origin[i].pos[k%3])/precision[i] + 0.5);
            break;
 
          case POSANGLE:
            if(j<6) // position
      	      dt[k] = (short)(((xys * posk) - origin[i].pos[k%3])/precision[i] + 0.5);
            else
              dt[k] = (short)(xys * ANGK);
            break;

          case ANGLE:     dt[k] = (short)(xys * ANGK); break;
          case MATRIX:    dt[k] = (short)(xys);  break;
          case QUATER:    dt[k] = (short)(xys);  break;

          case POSMATRIX: 
            if(j<6) // position
              dt[k] = (short)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            else
              dt[k] = (short)(xys);
            break;

          case POSQUATER: 
            if(j<6) // position
              dt[k] = (short)(((xys * posk) - origin[i].pos[k%3])/precision[i]);
            else
              dt[k] = (short)(xys);
            break;

          default: break;
        }
      }

      // eliminate extra char (FBB addr)
      bdata++;
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////
//  getmultirecords     Get Multiple Records
//
//  Parameters Passed:  datamode - POS,ANGLE,MATRIX,etc
//			startaddr - 1st FBB address to collect data from
//			stopaddr - last FBB address to collect data from
//			(global) fbbgroupdataflg - TRUE or FALSE, indicating the state
//				 group mode
//
//  Return Value:       TRUE if all went well
//			FALSE otherwise
//////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::getmultirecords(uchar startaddr, uchar stopaddr, uchar *birddata,
			       int numPkts)
{
  short datasize = 0;
  short tmpdtsz  = 0;
  uchar addr = 0;
  uchar temprs232tofbbaddr;
  uchar birdcmd;
  int i, pkts;

  // Save the Global rs232tofbbaddr in a temp
  temprs232tofbbaddr = rs232tofbbaddr;

  // If the Bird is NOT in the Group Data Mode, get each one separately
  if(!fbbgroupdataflg)
  {
    // check to see if in POINT mode
    if(mode==POINT)
    {
      birdcmd = (uchar) 'B';        /* POINT data mode */

      for(pkts=0;pkts<numPkts;pkts++)
      {
        // Now Get the Data for each device
        for(addr=startaddr,i=0; addr<=stopaddr; addr++,i++)
        {
          if((datasize = getDataSize(i)) == FALSE) return(FALSE);

          // Only Get data from devices with receivers
          if(!(fbbsystemstatus[addr] & 0x20))
	    continue;

          // Set the FBB Address
          rs232tofbbaddr = addr;

          // Send the POINT command

          if(sendSerialCmd(&birdcmd,1) != 1)
          {
 	    rs232tofbbaddr = temprs232tofbbaddr;
	    err("Could not send POINT command to bird at address ",addr);
 	    return(FALSE);
          }
	  else // read the sample from the current addr
	  {
  	    if(!readbirddata((short *)birddata, datasize, POINT))
	    {
	      err("Could not get information from bird at address ",addr);
	      rs232tofbbaddr = temprs232tofbbaddr;
	      return(FALSE);
	    }
	    else
	    {
	      birddata += (datasize * 2);
    	    }
          }
        }
      }
    }
    else // not group and not point
    //      this apparently does not work, will look into it later
    //      in the meantime I set an assertion if StreamMode is called while
    //      in GroupOff mode.
    {
      if(numSensors>1)
      {
        err("Not in GROUP mode.  Cannot read in STREAM mode.");
        return(FALSE);
      }
      else // Stream mode and 1 sensor
      {
        for(pkts=0;pkts<numPkts;pkts++)
	{
          if((datasize = getDataSize(0)) == FALSE) return(FALSE);

          // Only Get data from devices with receivers
          if(!(fbbsystemstatus[addr] & 0x20))
	    continue;

          // Set the FBB Address
          rs232tofbbaddr = startaddr;

          if(!readbirddata((short *)birddata, datasize, STREAM))
	  {
	    err("Could not get information from birds");
	    rs232tofbbaddr = temprs232tofbbaddr;
	    return(FALSE);
	  }
	  else
	  {
	    birddata += (datasize * 2);
    	  }
        }
      }
    }
  }
  else // The Bird is in the Group Data Mode
  {
    // Shut off the rs232 to fbb command..temporarily
    rs232tofbbaddr = 0;

    // check to see if in POINT mode
    if(mode==POINT)
    {
      birdcmd = (unsigned char) 'B';        /* POINT data mode */

      // Now Get the Data for each device
      for(addr=startaddr,i=0; addr<=stopaddr; addr++,i++)
      {
        // Only Get data from devices with receivers
        if(!(fbbsystemstatus[addr] & 0x20))
          continue;

        if((tmpdtsz = getDataSize(i)) == FALSE) return(FALSE);
 	datasize += tmpdtsz;
      }

      for(pkts=0;pkts<numPkts;pkts++)
      {
        if(sendSerialCmd(&birdcmd,1) != 1)
        {
	  err("Could not send POINT command to bird at address ",rs232tofbbaddr);
	  rs232tofbbaddr = temprs232tofbbaddr;
	  return(FALSE);
        }

        if(!readStream(birddata, (datasize*2+i)))
	{
	  err("Problem reading information from birds");
   	  rs232tofbbaddr = temprs232tofbbaddr;
	  return(FALSE);
        }
        else
	{
	  birddata += (datasize * 2) + i; // add in the group mode bytes
        }
      }
    }
    else // mode is STREAM and GROUP mode
    {
      // Now Get the Data for each device
      for(addr=startaddr,i=0; addr<=stopaddr; addr++,i++)
      {
        // Only Get data from devices with receivers
        if(!(fbbsystemstatus[addr] & 0x20))
          continue;

        if((tmpdtsz = getDataSize(i)) == FALSE) return(FALSE);
 	datasize += tmpdtsz;
      }

      datasize *= numPkts;

      if(!readStream(birddata, (datasize*2 + i*numPkts)))  // num chars
      {
        err("Could not get information from birds");
	rs232tofbbaddr = temprs232tofbbaddr;
	return(FALSE);
      }
      else
      {
        birddata += ((datasize * 2) + i*numPkts);
      }
    }
  }

  rs232tofbbaddr = temprs232tofbbaddr;
  return(TRUE);
}


boolean sgiFlock::setmeasurementrate()
{
  uchar birdchangevaluecmd = 6;
  uchar data[2];
  short cmdsize = 4;
  ushort i;
  short temprs232tofbbaddr = rs232tofbbaddr;
  float crystalfreq;

  // Get the CPU crystal freq

  if(!getCrystalFreq(rs232tofbbaddr, crystalfreq)) return(FALSE);

  // convert to transmitter time counts

  i = (unsigned)(((crystalfreq/8.0)*1000.0)*(((1000.0/clockSpeed)-0.3)/4.0));

  *(uchar *)&data[0] = (uchar) (i & 0xff);
  *(uchar *)&data[1] = (uchar) ((i & 0xff00) >> 8);

  if(Change(birdchangevaluecmd,data,cmdsize) != 4)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Problem with changing the clock measurement rate.");
    return(FALSE);
  }

  rs232tofbbaddr = temprs232tofbbaddr;
  return(TRUE);
}



//////////////////////////////////////////////////////////////////////////////////
//  setdatamode         Set the FBB Data Modes
//
//  Parameters Passed:  startaddr - 1st address to set the data mode
//			stopaddr  - last address to set the data mode
//			datamodes - POS,ANGLE,MATRIX,etc
//
//  Return Value:       TRUE if the datamode was set on all the Birds
//			FALSE otherwise
//
//  Remarks:            Sends the desired mode to all the Birds in the
//			start - stop range
//////////////////////////////////////////////////////////////////////////////////
int sgiFlock::setdatamode(uchar startaddr, uchar stopaddr)
{
  int addr;
  uchar posorientcmd;
  uchar temprs232tofbbaddr;
  int i;

  temprs232tofbbaddr = rs232tofbbaddr;

  for(addr=startaddr,i=0; addr<=stopaddr; addr++,i++)
  {
    // Setup the command
    switch(dataMode[i])
    {
      case POS:
	posorientcmd = 'V';
	break;

      case ANGLE:
        posorientcmd = 'W';
        break;

      case MATRIX:
        posorientcmd = 'X';
        break;

      case QUATER:
        posorientcmd = 92;
        break;

      case POSANGLE:
        posorientcmd = 'Y';
        break;

      case POSMATRIX:
        posorientcmd = 'Z';
        break;

      case POSQUATER:
        posorientcmd = ']';
        break;

      default:
        err("Illegal data mode in setdatamode");
        return(FALSE);
    }

    // Set the Address
    rs232tofbbaddr = addr;

    // Send the output position command

    if(sendSerialCmd(&posorientcmd,1) != 1)
    {
      err("Could not initialize output mode of bird ",addr);
      rs232tofbbaddr = temprs232tofbbaddr;
      return(FALSE);
    }
  }

  rs232tofbbaddr = temprs232tofbbaddr;
  return(TRUE);
}



//////////////////////////////////////////////////////////////////////////////////
// displayflocksys                                                              //
//////////////////////////////////////////////////////////////////////////////////
void sgiFlock::displayflocksys(uchar* parameter)
{
  int i;
  char tmp[MAXSTR];

  // Display the Configuration for each Address

  print("Flock Configuration");
  for (i=0;i<numfbbaddrs;i++)
  {
    // Allow the User to View the Data on a 25 line Screen

    sprintf(tmp,"FBB ADDR %d: ",i);

    if (parameter[i] & 0x80)
    {
      if (parameter[i] & 0x10)
 	strcat(tmp," ERC, ");
      else
	strcat(tmp, " 6DFOB, ");
    }
    else
    {
      strcat(tmp, " Not accessible");
      print(tmp);
      continue;
    }

    if(parameter[i] & 0x40)
      strcat(tmp, " Running, ");
    else
      strcat(tmp, " Not running, ");

    if((parameter[i] & 0x30) == 0x20)
      strcat(tmp, " RCVR");
    if((parameter[i] & 0x30) == 0x00)
      strcat(tmp, " No RCVR");

    if((parameter[i] & 0x11) == 0x01)
      strcat(tmp, ", XMTR ");
    else
    {
      if((parameter[i] & 0x10) == 0x00)
        strcat(tmp, ", No XMTR");
    }

    if((parameter[i] & 0x11) == 0x11)
      strcat(tmp, " ERT 0");

    if((parameter[i] & 0x12) == 0x12)
      strcat(tmp, " ERT 1");

    if((parameter[i] & 0x14) == 0x14)
      strcat(tmp, " ERT 2");

    if((parameter[i] & 0x18) == 0x18)
      strcat(tmp, " ERT 3");

    if((parameter[i] & 0x1f) == 0x10)
      strcat(tmp, " No ERTs");

    print(tmp);
  }

  return;
}


///////////////////////////////////////////////////////////////////////////////////
//  getsystemstatus     Get System Status                                        //
//                                                                               //
//  Return Value:       TRUE if all goes OK                                      //
//			FALSE if an Error Occurs                                 //
//                                                                               //
//  Remarks:            Gets the System Status from the Bird                     //
///////////////////////////////////////////////////////////////////////////////////
int sgiFlock::getsystemstatus()
{
  short i;
  short temprs232tofbbaddr;
  unsigned char birdgetsystemstatus[] = {'O',36};
  short rxchar;

  // Save the Current Address, but only send this command to
  // the device we are connected to..DO NOT USE the RS232 To FBB Command

  temprs232tofbbaddr = rs232tofbbaddr;
  rs232tofbbaddr = 0;

  // Send the Examine Value Comand with Parameter 0 and wait for a 2
  // byte response

  sleep(1);
  if(sendSerialCmd(birdgetsystemstatus,2) != 2)
    return(FALSE);

  // Get the n byte response
  for(i=0; i<numfbbaddrs; i++)
  {
    rxchar = waitforchar();

    fbbsystemstatus[i+1] = (uchar) rxchar;
    if (rxchar < 0)
    {
      err("Could not read data back from the Bird");
      return(FALSE);
    }
  }

  // Go through the List and Fill in the number of receivers
  // and the start and stop addresses of currently Running Devices

  numfbbrcvrs = 0;
  displayliststartaddr = 99; // Start with a illegal and High value
  displayliststopaddr = 1;

  for (i=1; i<=numfbbaddrs; i++)
  {
    if(fbbsystemstatus[i] & 0x40)  // Device is Running
    {
      // On the First time through setup the start address
      // ..rs232tofbbstartaddr was initialized to 99

      if(rs232tofbbstartaddr > 30)
        if(i < rs232tofbbstartaddr)
          rs232tofbbstartaddr = i;

      if(fbbsystemstatus[i] & 0x20)  /* Device has a receiver */
      {
	if(i < displayliststartaddr)
	  displayliststartaddr = i;

	displayliststopaddr = i;
	numfbbrcvrs++;
      }
    }
  }

  rs232tofbbaddr = temprs232tofbbaddr;

  return(TRUE);
}




////////////////////////////////////////////////////////////////////////////////////
// clearRx                                                                        //
//                                                                                //
// Read the characters out of the Rx buffer if available                          //
////////////////////////////////////////////////////////////////////////////////////
void sgiFlock::clearRx()
{
  short charsrdy;
  char temprxbuf[255];    // should never be more than 255 chars in buffer ??
//  phaseerror_count = 0;   // clear the phase counter

  //  Flush Pending Buffers
  ioctl(comhandle,TCFLSH,TIOCFLUSH);  /* flush pending I/O chars */

  //  Determine if there are chars in the Buffer
  if(ioctl(comhandle,TIONREAD,&charsrdy))
  {
    err("Function IOCTL returned an error.");
    return;
  }

  // Get the number of chars ready out of the read buffer
  if(charsrdy)
  {
    if(getRecord(charsrdy,temprxbuf,FALSE) != charsrdy)
      err("Could not clear the RX buffer");
  }
}


///////////////////////////////////////////////////////////////////////////////////
//  getRecord          - Get a record from the serial port
//
//  Parameters Passed:
//
//                      short charsneeded   - number of chars needed
//                      char * rxbufinptr   - place to store the chars
//                      short checkphasebit - check phase on/off
//
//  Return Value:       If successful, returns recsize
//                      else, RXERRORS if Rx errors were detected while
//                      receiving data
//
//  Remarks:            A record of data has the MSB of the first
//                      character set to a 1.  The routine verifies that
//                      the first character received is in PHASE.
//////////////////////////////////////////////////////////////////////////////////
int sgiFlock::getRecord(short charsneeded, char* rxbufinptr, short checkphasebit)
{
  short tempcharsneeded;
  uchar * temprxbufinptr = (uchar *)rxbufinptr;
  short charsread;

  /*
        Now Try to Read the required amount of characters...

        Note that we do NOT assume that the read will block until
        required number of characters are available because the
        read can return with less than the number of characters
        needed
  */
  tempcharsneeded = charsneeded;
  while(tempcharsneeded)
  {
    charsread = read(comhandle,temprxbufinptr,tempcharsneeded);

    if (charsread <= 0)  /* check to make sure we got a least 1 */
    {
      err("Serial receive timeout.");
      return(RXERRORS);
    }

    // Keep track of the number of characters read
    if (charsread != -1)
    {
      temprxbufinptr += charsread;
      tempcharsneeded -= charsread;
    }

    /*
         Verify that the first character has the Phasing Bit Set
         and all the remaining characters do NOT have the Phasing Bit
         Set ..only if enabled
    */

    if (checkphasebit && !(*rxbufinptr & 0x80))
    {
      err("Phasing bit error");
      return(RXERRORS);
    }
  }

  return(charsneeded);
}


///////////////////////////////////////////////////////////////////////////////////
// GroupOff                                                                       //
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::GroupOff()
{
  static unsigned char birdchangevaluecmd[] = {'P',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  short temprs232tofbbaddr;  

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  // Store the rs232tofbbaddr

  temprs232tofbbaddr = rs232tofbbaddr;

  // turn group mode off

  birdchangevaluecmd[0] = 0;

  if(Change((uchar)35,birdchangevaluecmd,3) != 3)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Problem with changing to group mode.");
    return(FALSE);
  }

  fbbgroupdataflg = FALSE;
  rs232tofbbaddr = temprs232tofbbaddr;

  // set the read mode to POINT if numSensors > 1
  if(numSensors > 1)
  {
    mode = POINT;
  }

  return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////
// GroupOn                                                                       //
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::GroupOn()
{
  static unsigned char birdchangevaluecmd[] = {'P',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  short temprs232tofbbaddr;  

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  // Store the rs232tofbbaddr

  temprs232tofbbaddr = rs232tofbbaddr;

  // change to group mode

  birdchangevaluecmd[0] = 1;

  if(Change((uchar)35,birdchangevaluecmd,3) != 3)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Problem with changing to group mode.");
    return(FALSE);
  }

  fbbgroupdataflg = TRUE;
  rs232tofbbaddr = temprs232tofbbaddr;
  return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////
// StreamMode                                                                    //
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::StreamMode()
{
  short temprs232tofbbaddr;  
  uchar birdcmd = '@';

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  // Store the rs232tofbbaddr

  assert(fbbgroupdataflg==TRUE); // group mode must be set to use Stream Mode

  temprs232tofbbaddr = rs232tofbbaddr;

  if(sendSerialCmd(&birdcmd,1) != 1)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Could not send STREAM command to bird at address ",(int)rs232tofbbaddr);
    return(FALSE);
  }

  rs232tofbbaddr = temprs232tofbbaddr;

  mode = STREAM;

  return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////
// Hemisphere                                                                     //
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::Hemisphere(HemisphereType ht)
{
  short temprs232tofbbaddr;  
  uchar birdcmd[3];
  uchar addr;

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  birdcmd[0] = 'L';

  switch (ht)
  {
    case HEMI_FORWARD:
      birdcmd[1] = 0x00;
      birdcmd[2] = 0x00;
      break;

    case HEMI_AFT:
      birdcmd[1] = 0x00;
      birdcmd[2] = 0x01;
      break;

    case HEMI_UPPER:
      birdcmd[1] = 0x0c;
      birdcmd[2] = 0x01;
      break;

    case HEMI_LOWER:
      birdcmd[1] = 0x0c;
      birdcmd[2] = 0x00;
      break;

    case HEMI_LEFT:
      birdcmd[1] = 0x06;
      birdcmd[2] = 0x01;
      break;

    case HEMI_RIGHT:
      birdcmd[1] = 0x06;
      birdcmd[2] = 0x00;
      break;
  }

  // Store the rs232tofbbaddr

  temprs232tofbbaddr = rs232tofbbaddr;

  for(addr=displayliststartaddr;addr<=displayliststopaddr;addr++)
  {
    rs232tofbbaddr = addr;

    if(sendSerialCmd(birdcmd,3) != 3)
    {
      rs232tofbbaddr = temprs232tofbbaddr;
      err("Could not send HEMISPHERE command to bird at address ",(int)rs232tofbbaddr);
      return(FALSE);
    }
  }

  rs232tofbbaddr = temprs232tofbbaddr;

  return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////
// PointMode                                                                     //
///////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::PointMode()
{
  short temprs232tofbbaddr;  
  uchar birdcmd = 'B';

  if(!flock_init) { err("Flock not initialized"); return(FALSE); }

  // Store the rs232tofbbaddr

  temprs232tofbbaddr = rs232tofbbaddr;

  if(sendSerialCmd(&birdcmd,1) != 1)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Could not send POINT command to bird at address ",(int)rs232tofbbaddr);
    return(FALSE);
  }

  rs232tofbbaddr = temprs232tofbbaddr;

  mode = POINT;

  return(TRUE);
}


/////////////////////////////////////////////////////////////////////////////////////
// autoConfig                                                                      //
/////////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::autoConfig()
{
  short temprs232tofbbaddr;
  static unsigned char birdchangevaluecmd[] = {'P',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  // Store the rs232tofbbaddr
  temprs232tofbbaddr = rs232tofbbaddr;

  // Initialize the Parameter Bytes
  birdchangevaluecmd[0] = 0;
  birdchangevaluecmd[1] = 0;

  // Get the Number of FBB Addrs
  if (getAddrMode() == 0)
    return(FALSE);

  // Preset the RS232 to Destination address to 1 for
  // this command if the RS232 to FBB command is active

  if(rs232tofbbaddr != 0)
    rs232tofbbaddr = 1;

  // autoconfig command
  temprs232tofbbaddr = rs232tofbbaddr;

  sleep(1);
  birdchangevaluecmd[0] = (uchar) (numSensors + 1);

  if(Change((uchar)50,birdchangevaluecmd,3) != 3)
  {
    rs232tofbbaddr = temprs232tofbbaddr;
    err("Problem with autoconfiguration command.");
    return(FALSE);
  }

  rs232tofbbaddr = temprs232tofbbaddr;

  // get the current system status
  if(getsystemstatus())
    displayflocksys(fbbsystemstatus);
  else
    return(FALSE);
  sleep(1);

  if(!GroupOn()) return(FALSE);
  rs232tofbbaddr = temprs232tofbbaddr;

  sleep(1);
  if(!setmeasurementrate()) return(FALSE);
  rs232tofbbaddr = temprs232tofbbaddr;

  //  ex: values for 144 Hz
  //      birdchangevaluecmd[0] = 0xf4;
  //      birdchangevaluecmd[1] = 0x19;


  return(TRUE);
}

//////////////////////////////////////////////////////////////////////////////////
//  sendSerialCmd     - Send Serial Command to the Bird port                    //
//                                                                              //
//  Prototype in:       serial.h                                                //
//                                                                              //
//  Parameters Passed:  cmd         -   string to send to the serial port       //
//                      cmdsize     -   size of the cmd string (cmd is NOT      //
//                                      NULL terminated, since the data can     //
//                                      be NULL)                                //
//  Return Value:       number of characters transmitted                        //
//                                                                              //
//  Remarks:            Routine will send a string of characters to the serial  //
//                      port.  The string is pointed to by cmd and all          //
//                      characters will be sent upto but NOT including          //
//                      the NULL                                                //
//////////////////////////////////////////////////////////////////////////////////

int sgiFlock::sendSerialCmd(uchar *cmd, short cmdsize)
{
  char cmdbytessent;
  uchar rs232tofbbcmd;

  // Send the RS232 to FBB Prefice Character if non-zero

  if(rs232tofbbaddr > 0)
  {
    if(rs232tofbbaddr <= 15) // pass through command 0-15
      rs232tofbbcmd = (unsigned char)(0xF0 | rs232tofbbaddr);
    else // pass through command 16-31
      rs232tofbbcmd = (unsigned char)(0xE0 | rs232tofbbaddr-16);

    write(comhandle,&rs232tofbbcmd,1);
  }

  cmdbytessent = write(comhandle,cmd,cmdsize);

  ioctl(comhandle,TCFLSH,TIOCFLUSH);  // flush pending I/O chars

  return(cmdbytessent);
}


////////////////////////////////////////////////////////////////////////////////////
//  getAddrMode         Get the current Addressing Mode
//
//  Return Value:       30 if in Expanded Address Mode
//     		        14 if in Normal Address Mode
//			FALSE if an Error Occurs
//
//  Remarks:            The routine gets the Bird Status from the Master
//			and uses bit 12 to determine if the flock is in
//			expanded address mode
////////////////////////////////////////////////////////////////////////////////////
int sgiFlock::getAddrMode()
{
  uchar parameter[2];
  uchar birdgetaddrmode = 0;
  short numbytes;

  // Send the Examine Value Comand with Parameter 0 and wait for a 2
  // byte response
  
  if(!Examine(birdgetaddrmode, parameter, numbytes))
    return(FALSE);

  numfbbaddrs = 14;

  // Check bit 12 in the Status Word...or bit 2 in the second byte
  // and return the proper value

  if(parameter[1] & 0x04)
  {
    print("Flock is setup to the Expanded Addressing Mode");
    numfbbaddrs = 30;
    fbbaddrbits = 0x1f;
    cmdbitshft = 5;
    return(30);
  }
  else
  {
    print("Flock is setup to the Normal Addressing Mode");
    numfbbaddrs = 14;
    fbbaddrbits = 0x0f;
    cmdbitshft = 4;
    return(14);
  }
}


///////////////////////////////////////////////////////////////////////////////////
// matchExamineValue                                                             //
///////////////////////////////////////////////////////////////////////////////////
int sgiFlock::matchExamineValue(ushort* lst, ushort elmt)
{
  int i;

  for(i=1;i<24;i++)
    if(elmt == lst[i])
      return(i);

  return(-1);
}


//
// public access functions
//


boolean sgiFlock::SetMeasurementRate(float rate)
{
  clockSpeed = rate;

  if(!flock_init) {err("Flock not initialized"); return(FALSE); }

  assert(rate>=14.0 && rate<=144.0);

  return(setmeasurementrate());
}



//////////////////////////////////////////////////////////////////////////////////
// SetReadMode                                                                  //
//////////////////////////////////////////////////////////////////////////////////
void sgiFlock::SetReadMode(DataModeType* dmt)
{
  int i;
  uchar startaddr, stopaddr;

  if(!flock_init) {err("Flock not initialized"); return; }

  numDataVals = 0;
  for(i=0;i<numSensors;i++)
  {
    assert(dmt[i]>=POS && dmt[i]<=POSQUATER);
    dataMode[i] = dmt[i];
    numDataVals += (int)getDataSize(i);
  }

  startaddr = displayliststartaddr;
  stopaddr  = displayliststopaddr;

  setdatamode(startaddr, stopaddr);
  dataModeFlag = TRUE;
}


//////////////////////////////////////////////////////////////////////////////////
//  Change         -    Change Command to the Bird port                         //
//                                                                              //
//  Prototype in:       serial.h                                                //
//                                                                              //
//  Parameters Passed:  cmd         -   string to send to the serial port       //
//                      cmdsize     -   size of the cmd string (cmd is NOT      //
//                                      NULL terminated, since the data can     //
//                                      be NULL)                                //
//  Return Value:       number of characters transmitted                        //
//                                                                              //
//  Remarks:            Routine will send a string of characters to the serial  //
//                      port.  The string is pointed to by cmd and all          //
//                      characters will be sent upto but NOT including          //
//                      the NULL                                                //
//////////////////////////////////////////////////////////////////////////////////

int sgiFlock::Change(uchar parm, uchar *data, short cmdsize)
{
  char cmdbytessent;
  uchar birdchangevaluecmd[10];
  int i;

  if(!flock_init) {err("Flock not initialized"); return(FALSE); }

  birdchangevaluecmd[0]='P';
  birdchangevaluecmd[1] = parm;
  for(i=0;i<cmdsize-2;i++)
    birdchangevaluecmd[i+2] = data[i];

  // Send the RS232 to FBB Prefice Character if non-zero

  cmdbytessent = sendSerialCmd(birdchangevaluecmd,cmdsize);

  return(cmdbytessent);
}


//////////////////////////////////////////////////////////////////////////////////
//  Command        -    Command to the Bird port                                //
//                                                                              //
//  Parameters Passed:  cmd         -   string to send to the serial port       //
//                      cmdsize     -   size of the cmd string (cmd is NOT      //
//                                      NULL terminated, since the data can     //
//                                      be NULL)                                //
//  Return Value:       number of characters transmitted                        //
//                                                                              //
//  Remarks:            Routine will send a string of characters to the serial  //
//                      port.  The string is pointed to by cmd and all          //
//                      characters will be sent upto but NOT including          //
//                      the NULL                                                //
//////////////////////////////////////////////////////////////////////////////////

int sgiFlock::Command(uchar cmdchar, uchar *data, short cmdsize)
{
  char cmdbytessent;
  static uchar birdcmd[] = {'P',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int i;

  if(!flock_init) {err("Flock not initialized"); return(FALSE); }

  birdcmd[0] = cmdchar;
  for(i=0;i<cmdsize;i++)
    birdcmd[i+1] = data[i];

  cmdbytessent = write(comhandle,birdcmd,cmdsize);

  return(cmdbytessent);
}



//////////////////////////////////////////////////////////////////////////////////
//  Examine        -    Examine Command from the Bird port                      //
//                                                                              //
//  Parameters Passed:  cmd         -   string to send to the serial port       //
//                      cmdsize     -   size of the cmd string (cmd is NOT      //
//                                      NULL terminated, since the data can     //
//                                      be NULL)                                //
//  Return Value:       number of characters transmitted                        //
//                                                                              //
//  Remarks:            Routine will send a string of characters to the serial  //
//                      port.  The string is pointed to by cmd and all          //
//                      characters will be sent upto but NOT including          //
//                      the NULL                                                //
//////////////////////////////////////////////////////////////////////////////////

boolean sgiFlock::Examine(uchar parm, uchar *data, short& cmdsize)
{
  static uchar birdexaminevaluecmd[] = {'O',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int i;
  short rxchar;
  int index;
  static unsigned short examinevalueparanumber[] =
	   {0,0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,32,33,34,35,36,50};

  // Number of characters in the response for each menu item
  static unsigned short examinevaluerspsize[] =
	   {0,2,2,2,2,2,14,2,1,1,1,1,14,14,1,10,2,1,2,5,14,1,14,5};

  if(!flock_init) {err("Flock not initialized"); return(FALSE); }

  birdexaminevaluecmd[1] = parm;

  if(parm==33 || parm==34 || parm==36 || parm==50)
  {
    if(getAddrMode() == 0)
      return(FALSE);

    if (numfbbaddrs == 14)
    {
      examinevaluerspsize[19] = 5;
      examinevaluerspsize[20] = 14;
      examinevaluerspsize[22] = 14;
      examinevaluerspsize[23] = 5;
    } 
    else
    {
      examinevaluerspsize[19] = 7;
      examinevaluerspsize[20] = 30;
      examinevaluerspsize[22] = 30;
      examinevaluerspsize[23] = 7;  
    }
  }

  if((cmdsize=sendSerialCmd(birdexaminevaluecmd,2)) != 2)
    return(FALSE);

  assert((index = matchExamineValue(examinevalueparanumber, parm)) != -1);

  // Get the n byte response
  for(i=0; i<examinevaluerspsize[index]; i++)
  {
    rxchar = waitforchar();

    data[i] = (uchar) rxchar;
    if (rxchar < 0)
    {
      err("Could not read data back from the Bird");
      return(FALSE);
    }
  }

  cmdsize = examinevaluerspsize[index];
  return(TRUE);
}


//////////////////////////////////////////////////////////////////////////////////
//  waitforchar         -   Wait for a Character from the Serial Port
//
//  Return Value:       returns the receive character if successful,
//                      RXERRORS if recieve errors,
//                      RXTIMEOUT if a time out occurs before a
//                      character is ready
//
//  Remarks:            Routine waits for the TIMEOUTINTICKS period
//                      for a character
//////////////////////////////////////////////////////////////////////////////////
int sgiFlock::waitforchar()
{
  short rxchar;

  // Wait until a character is available
  // ....leave loop if errors or character available

  while ((rxchar = get_serial_char()) == NODATAAVAIL);

  // return if RX errors
  if(rxchar < 0)
    return(RXERRORS);

  // Everything is OK...return the character
  return(rxchar);
}

//////////////////////////////////////////////////////////////////////////////////
//  get_serial_char -   Get 1 Character from the serial port if one is available//
//                                                                              //
//  Prototype in:       serial.h                                                //
//                                                                              //
//  Parameters Passed:  void                                                    //
//                                                                              //
//  Return Value:       returns the                                             //
//                                                                              // 
//  Remarks:            returns the receive character if successful,            //
//                        RXERRORS if receive errors                            //
//////////////////////////////////////////////////////////////////////////////////

int sgiFlock::get_serial_char()
{
  uchar chr;

  if((read(comhandle, &chr,1)) != -1)
  {
    return(chr);
  }
  else
  {
    err("get_serial_char()");
    return(RXERRORS);
  }
}


//////////////////////////////////////////////////////////////////////////////////
//  readbirddata        -   Read Bird Data and convert to 2's complement
//
//  Parameters Passed:  birddata    - pointer to integer array to store data
//			numwords    - number of words (16 bits) to read
//			outputmode  - POINT, CONTINUOUS, or STREAM
//			buttonmode  - 0 if off, 1 if on (send button value)
//
//  Return Value:       TRUE if successful
//			FALSE otherwise
//
//  Remarks:            routine reads a record from the bird, and adjusts
//			the least and most significant Bird bytes into 2's
//			complemented words.
//////////////////////////////////////////////////////////////////////////////////
boolean sgiFlock::readbirddata(short* birddata, short numwords, short outputmode) {
  short i;
  short extrachar=0;
  uchar xx, yy;
  short xxs, yys, xxss, yyss, xy, xys;

  if(fbbgroupdataflg)
    extrachar = 1;

  // Read 2*numbwords characters from the bird, check for errors
  // ...add one character to record length if buttonmode == 1

  if((get_serial_record((uchar *)birddata,(2*numwords)+extrachar)) < 0)
  {
//    if(outputmode == STREAM)
//      sendSerialCmd((uchar *)"B",1);     // stop streaming, via Point cmd 
    err("Could not read record data from the bird");
    return(FALSE);
  }

  //	go though the birddata and make into two's complemented
  //	16 bit integers by:
  //	    - lsbyte << 1
  //	    - word << 1

  for (i=0;i<numwords;i++)
  {
    xx = *(unsigned char *)birddata;
    xxs = (short)xx;
    xxss = xxs & 0x7F;
    yy = *((unsigned char *) birddata+1);
    yys = (short)yy;
    yyss = yys << 7;
    xy = xxss | yyss;
    xys = (xy << 2);
    *birddata++ = xys;
  }
    
  return(TRUE);
}

