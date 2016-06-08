////////////////////////////////////////////////////////////////////////////////////
// Defs.h                                                                         //
//                                                                                //
// Written by Greg Schmidt                                                        //
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
////////////////////////////////////////////////////////////////////////////////////

#ifndef __DEFS_H__
#define __DEFS_H__

#include <math.h>

#define TRUE		1	
#define FALSE		0	
#define ON              1
#define OFF             0
#undef  PI
#define PI		3.141592654
#define R2D		180.0/PI
#define D2R		PI/180.0
#undef	MINFLOAT
#define MINFLOAT	-1.0e18
#undef	MAXFLOAT
#define	MAXFLOAT	1.0e18
#undef  EPSILON		
#define EPSILON		1.0e-7
#define MAXSTR          256
#define XX              0
#define YY              1
#define ZZ              2
#define TT              3

typedef unsigned char   uchar;
typedef unsigned char	boolean;
typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef unsigned short  ushort;
typedef char            *string;
typedef unsigned char   *ustring;

//const long      MAXLONG = 10;


#endif
