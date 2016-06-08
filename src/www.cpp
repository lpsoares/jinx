
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>


#include "www.h"

#ifdef LINUX
PRIVATE int printer(const char * fmt, va_list pArgs) {
    return (vfprintf(stdout, fmt, pArgs));
}

PRIVATE int tracer(const char * fmt, va_list pArgs) {
    return (vfprintf(stderr, fmt, pArgs));
}

int terminate_handler_libwww(HTRequest * request, HTResponse * response, void * param, int status) {
	
	#ifdef DEBUG
		HTPrint("Load resulted in status %d\n", status);
	#endif
		
    /* we're not handling other requests */
	HTEventList_stopLoop ();

	return(HT_OK);
	
}
#endif

WWW::WWW() {
		
	struct stat st;
	
	strcpy(this->cache,".cache/");
	
	if (stat(cache, &st)) {
		cout << "Creating cache diretory" << endl;
		
		mode_t mode;
		
		mode = 0755;
		
		mkdir(this->cache,mode);
	}
	
#ifdef LINUX

	this->request = NULL;


	/* Initiate W3C Reference Library with a client profile */
	//HTProfile_newNoCacheClient("Jinx","1.0");
	HTProfile_newPreemptiveClient("Jinx","1.0");
    
	#ifdef DEBUG 
		HTSetTraceMessageMask("sop");
	#endif

	/* We don't wany any progress notification or other user stuff */
	HTAlert_setInteractive(NO);
		
	/* Need our own trace and print functions */
	HTPrint_setCallback(printer);
	HTTrace_setCallback(tracer);

	/* Add our own filter to terminate the application */
	HTNet_addAfter(terminate_handler_libwww, NULL, NULL, HT_ALL, HT_FILTER_LAST);

	/* Set the timeout for how long we are going to wait for a response */
	HTHost_setEventTimeout(10000);
#endif

}

WWW::~WWW() {
	
#ifdef LINUX
	HTProfile_delete();
	
#endif
}


char *WWW::complete(char *filename) {
	
	char *complete_filename = new char[256];
	strcpy(complete_filename,this->cache);
	
	strcat(complete_filename,filename);
	
	return(complete_filename);
	
}


int WWW::getfile(char *name, char *path) {
	
	char *outputfile = new char[256];
	char *getme = new char[256];
	
	strcpy(getme,path);
	
	strcat(getme,name);
	
	strcpy(outputfile,this->cache);
	strcat(outputfile,name);
	
	FILE * fp = NULL;
	
#ifdef LINUX
	if (getme && *getme) {
		
		if(this->request==NULL) {
			this->request = HTRequest_new();
		}
		
		if (getme && outputfile && request) {
			
			#ifdef DEBUG
				cout << "Getting the file " << outputfile << endl;
			#endif
						
			// Check if file exists. 
			if (access(outputfile, F_OK) != -1) {
				
				#ifdef DEBUG
					cout << "File " << outputfile << " already exist" << endl;
				#endif
				
	    		//HTAlertCallback * prompt = HTAlert_find(HT_A_CONFIRM);
	    		//if (prompt) {
					//if ((*prompt)(request, HT_A_CONFIRM, HT_MSG_FILE_REPLACE, NULL,
			      		//NULL, NULL) != YES)
		    		//return NO;
	    		//}
				//return 0;
				// This is wrong actualy the software should check if the file is new and then replace
			} else 	if ((fp = fopen(outputfile, "wb")) == NULL) { // If replace then open the file 
	    		//HTRequest_addError(request, ERR_NON_FATAL, NO, HTERR_NO_FILE,(char *) outputfile, strlen(outputfile),"HTLoadToFile"); 
				cerr << "Not possible to open the file for writing" << endl;
				return 0;
			} else {

				// Set the output stream and start the request 
				HTRequest_setOutputFormat(this->request, WWW_SOURCE);
				//HTRequest_setOutputStream(this->request, HTFWriter_new(this->request, fp, NO));
				HTRequest_setOutputStream(this->request, HTFWriter_new(this->request, fp, YES));
				if( HTLoadAbsolute(getme, this->request)==YES) {
					#ifdef DEBUG 
						cout << "Accepted" << endl;
					#endif
				} else {
					#ifdef DEBUG 
						cout << "Not Accepted" << endl;
					#endif
				}
		
				// Go into the event loop...
        		HTEventList_loop(this->request);
		
				long size;
				size = ftell(fp);
								
				if(fclose(fp)) {
					cout << "Problems with fclose" << endl;
				}
				
				// This is probably because the file is missing in http site
				if(size==0) {
					
					#ifdef DEBUG
						cout << "Removing file : " << outputfile << endl;
					#endif
					
					unlink(outputfile);
					
					#ifdef DEBUG
						cout << "Removed!" << endl;
					#endif
					
				}
					
			}
			
    	} else {
			cout << "Some problem reading internet file" << endl;
		}

		HTRequest_delete(request);
		this->request=NULL;

	} 
#endif
	
    return 0;

}
