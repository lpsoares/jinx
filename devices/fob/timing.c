////////////////////////////////////////////////////////////////////////////////////
// timing.h                                                                       //
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

#include <sys/time.h>
#include <sys/types.h>
#include <time.h>


//
// prototypes
//
double u_realTime();
int realTime();


//
// function that returns real time elapsed in u-secs from the last call to it
//
double u_realTime()
{
  struct timeval tp;
  static int u_secs = -1;
  static int secs = -1;
  int u_s, s;
  double delta;

  gettimeofday(&tp);

  s   = tp.tv_sec;
  u_s = tp.tv_usec;

  if(secs < 0)
  { 
    secs = s; u_secs = u_s; delta = 0.;
  }
  else
  {
    delta = (double)((s - secs)*1000000.) + (double)(u_s - u_secs);
    secs = s; u_secs = u_s;
  }

//  printf("dDelta = %lf\n", delta);
  return delta;
}


//
// functions that returns real time elapsed in secs from the last call to it
// (the old method)
// 
int realTime() // returns real time in sec
{
  long int nw;
  struct tm *tmstruct;
  static int secs = -1;
  int s, delta;

  time(&nw);
  tmstruct = localtime(&nw);

  s = tmstruct->tm_sec + tmstruct->tm_min * 60 + tmstruct->tm_hour * 3600;

  if(secs < 0)
  { 
    secs = s; delta = 0;
  }
  else
  {
    if(s < secs) // flipped to a new day
      delta = s + 86400 - secs;
    else
      delta = s - secs;
    secs = s;
  }

//  printf("Delta = %d\n", secs);
  return delta;
}




