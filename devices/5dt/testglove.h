
#include "fglove.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h> // for Sleep
#else
#include <unistd.h>  // for usleep
#endif

class Glove {
	public:

	char    *szPort;
	fdGlove *pGlove;
	bool     showraw;
	int      glovetype;
	char 	*szType;
	
	Glove();
	~Glove();
	
	void faz( );

};
