////////////////////////////////////////////////////////////////////////////////////
// extra.h                                                                        //
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

#ifndef __EXTRA_H__
#define __EXTRA_H__

#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sgtty.h>
#include <termios.h>
#include <unistd.h>

#define ON      1
#define OFF     0

#define TIONREAD FIONREAD
#define RXERRORS -2
#define NODATAAVAIL -1
#define XOFF 0x13

#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4

//
//   Number Conversions
//
#define DTR (float)(3.141593/180.0)                 /* degrees to radians */
#define FTW (float)32768.0                          /* float to word integer */
#define WTF (float)(1.0/32768.0)                    /* float to word integer */
#define ANGK (float)(180.0/32768.0)                 /* integer to degrees */
#define POSK36 (float)(36.0/32768.0)                /* integer to inches */
#define POSK72 (float)(72.0/32768.0)                /* integer to inches */
#define POSK144 (float)(144.0/32768.0)              /* integer to inches ER Controller */

typedef enum
{
  POINT,
  CONTINUOUS,
  STREAM
}
ModeType;

typedef enum
{
  SRT36,
  SRT72,
  ERT
}
HardwareType;

typedef enum
{
  POS,
  ANGLE,
  MATRIX,
  QUATER,
  POSANGLE,
  POSMATRIX,
  POSQUATER
}
DataModeType;

typedef enum
{
  HEMI_FORWARD,
  HEMI_AFT,
  HEMI_LEFT,
  HEMI_RIGHT,
  HEMI_UPPER,
  HEMI_LOWER
}
HemisphereType;

#define OPENPARAMS O_RDWR

//
//  Bird Timing definitions
//
#define DEFAULTCRYSTALFREQ (float) 20.00        /* CPU Clock Frequency */
#define DEFAULTCLOCKVALUE (float) 0.40          /* 8/DEFAULTCRYSTALFREQ */


#endif // __EXTRA_H__

