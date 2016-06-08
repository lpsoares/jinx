#include "general.h"

#include <time.h> // for nanosleep

void breath() {
	
	struct timespec req; // used for nanosleep
	struct timespec rem; // used for nanosleep
	req.tv_sec=0;
	req.tv_nsec=0;
	nanosleep(&req,&rem); // this is a critical point in the program and could use too much processament otherwise

}
