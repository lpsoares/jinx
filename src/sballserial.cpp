/*
 * sballserial.c -- Machine/OS dependent serial I/O code for Libsball
 *
 *  Copyright 1997 (unix code)  John E. Stone (j.stone@acm.org)
 *                 (Win32 code) Mike Connell  (M.Connell@swansea.ac.uk)  
 *
 *  $Id: sballserial.cpp,v 1.1.1.1 2004/05/05 21:15:31 lsoares Exp $
 *
 */

#include <iostream.h>

//using namespace std;

#if !defined(WIN32) && !defined(_MSC_VER)

/*
 * Unix serial I/O code by John Stone
 */

#define _POSIX_SOURCE 1



#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "sballserial.h" /* protos and types for this file */




typedef struct {
  int fd;        /* serial port device file descriptor */
} commstruct;

int sball_comm_open(char * commname, SBallCommHandle * commhandleptr) {
  struct termios sballtermio;
  commstruct * comm;

  *commhandleptr = NULL;

  comm = (commstruct *) malloc(sizeof(commstruct));
  if (comm == NULL)
    return -1;

  comm->fd = open(commname, O_RDWR | O_NONBLOCK | O_NOCTTY);

	if (comm->fd == -1) {
		free(comm);
		return -1;  /* failed open of comm port */
	}
	tcgetattr(comm->fd, &sballtermio);

#if 0
  /* TIOCEXCL exclusive access by this process */
#if defined(TIOCEXCL)
  if (ioctl(comm->fd, TIOCEXCL) < 0) {
    return -1; /* couldn't get exclusive use of port */
  }
#endif
#endif

  sballtermio.c_lflag = 0;
  sballtermio.c_lflag = 0;
  sballtermio.c_iflag = IGNBRK | IGNPAR;
  sballtermio.c_oflag = 0;
  sballtermio.c_cflag = CREAD | CS8 | CLOCAL | HUPCL;
  sballtermio.c_cc[VEOL] = '\r';
  sballtermio.c_cc[VERASE] = 0;
  sballtermio.c_cc[VKILL] = 0;
  sballtermio.c_cc[VMIN] = 0;
  sballtermio.c_cc[VTIME] = 0;

  /* use of baud rate in cflag is deprecated according to the */
  /* single unix spec, also doesn't work in IRIX > 6.2        */
  cfsetispeed(&sballtermio, B9600);
  cfsetospeed(&sballtermio, B9600);

  tcsetattr(comm->fd, TCSAFLUSH, &sballtermio);

  *commhandleptr = (SBallCommHandle) comm;
 
  return 0;
}

int sball_comm_write(SBallCommHandle commhandle, char * buf) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return write(comm->fd, buf, strlen(buf));
}

int sball_comm_read(SBallCommHandle commhandle, char * buf, int sz) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return read(comm->fd, buf, sz);
}

int sball_comm_close(SBallCommHandle * commhandleptr) {
  commstruct * comm = (commstruct *) (*commhandleptr);

  if (comm == NULL)
    return -1;

  close(comm->fd);

  free(*commhandleptr);
  *commhandleptr = NULL;

  return 0;
}

/* end of unix code */

#else

/* 
 * Win32 serial code by Mike Connell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "sballserial.h" /* protos and types for this file */
#include "serial.h"    /* Win32 specific serial stuff */

typedef T_serial commstruct; /* all the serial port data */

int sball_comm_open(char * commname, SBallCommHandle * commhandleptr) {
  commstruct * serial;
  int port;

  *commhandleptr = NULL;

  serial = malloc(sizeof(commstruct));
  if (serial == NULL)
    return -1;

  /* translate com port name to appropriate integer */
  if (sscanf(commname,"%d",&port)!=1) {
    free(serial);
    return -1; /* print some error message here if desired */
  }

  if (F_spacesball_serial_start(serial, port, 9600, 1, FALSE, "SpaceOrb")) {
    *commhandleptr = serial;
    return 0;
  }
  else {
    free(serial);
    return -1;
  }
}

int sball_comm_write(SBallCommHandle commhandle, char * buf) {
  commstruct * serial = (commstruct *) commhandle;
  int len;

  if (serial == NULL) 
    return -1;

  len=strlen(buf);

  if (F_spacesball_serial_write(serial,buf,len))
    return len;
  else
    return 0;
}

int sball_comm_read(SBallCommHandle commhandle, char * buf, int sz) {
  commstruct * serial = (commstruct *) commhandle;

  if (serial == NULL) 
    return -1;

  return F_spacesball_serial_read(serial,buf,sz);
}

int sball_comm_close(SBallCommHandle * commhandleptr) {
  commstruct * serial = (commstruct *) *commhandleptr;

  if (serial == NULL)
    return -1;

  F_spacesball_serial_close_connection(serial);

  free(*commhandleptr);
  *commhandleptr = NULL;
 
  return 0;
}

/* end of Win32 code */

#endif 


