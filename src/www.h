
#ifndef __www_h
#define __www_h

#include "WWWLib.h"			      /* Global Library Include file */
#include "WWWMIME.h"				    /* MIME parser/generator */
#include "WWWNews.h"				       /* News access module */
#include "WWWHTTP.h"				       /* HTTP access module */
#include "WWWFTP.h"
#include "WWWFile.h"
#include "WWWGophe.h"
#include "WWWInit.h"

// It was necessary in the orad (RedHat 8.0)
#define __ASSEMBLER__

using namespace std;

class WWW {
	
	public:
	
	WWW();
	~WWW();
	
	int getfile(char *name, char *path);
	char cache[64];

#ifdef LINUX
	HTRequest *request;
#endif

	char *complete(char *filename);
	
};

#endif
