

#include "sax.h"
#include "xmlsax.h"
#include <unistd.h>


#include <iostream>

#ifdef XERCES_CPP_NAMESPACE_USE
SAX2PrintHandlers::SAX2PrintHandlers( const   char* const              encodingName                                    , const XMLFormatter::UnRepFlags unRepFlags									, const bool					 expandNamespaces									, Configuration* conf	) :
    fFormatter   (        encodingName, 0 , this  , XMLFormatter::NoEscapes , unRepFlags    ),	fExpandNS ( expandNamespaces ) {

	this->conf = conf;
	this->localComputer = false;
    this->foundComputer = false;

	gethostname(this->computer_name,256);
		
}
#else
SAX2PrintHandlers::SAX2PrintHandlers( const char* const encodingName, const XMLFormatter::UnRepFlags unRepFlags
		, const bool expandNamespaces, Configuration* conf	) :
    	fFormatter(encodingName, this, XMLFormatter::NoEscapes, unRepFlags ),fExpandNS(expandNamespaces) {
			
	this->conf = conf;
	this->localComputer = false;
	this->foundComputer = false;
			
	gethostname(this->computer_name,256);

}
#endif

void SAX2PrintHandlers::characters(const XMLCh* const chars, const unsigned int length) {
	// could be used to read characters betwen tags
}


void SAX2PrintHandlers::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const    qname, const   Attributes&		attributes) {

	unsigned int index;
	unsigned int len;

	len = attributes.getLength();
	
	// finding the tag computer in the configuration file
	if(!strcmp(XMLString::transcode(localname),"computer")) {
		
		for(index=0;index<len;index++) {
			if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"name")) {
				if(!strcmp(XMLString::transcode(attributes.getValue(index)),this->computer_name)) {
					
					this->localComputer=true; 	// indicating that we start to read the conf now
					this->foundComputer=true;	// means that the computer was find in the conf file
					
					#ifdef DEBUG
						cout << "reading :" << computer_name ;
					#endif
					
				}
			}
		}
	
	} else
		
	if(this->localComputer) {
		
		if(!strcmp(XMLString::transcode(localname),"window")) {
			
			conf->computer.viewports[conf->window].width = 640;			//
			conf->computer.viewports[conf->window].height = 480;		//
			conf->computer.viewports[conf->window].fullscreen = false;	//
			
			conf->computer.viewports[conf->window].frustum[0] = -1.0; 	//left
			conf->computer.viewports[conf->window].frustum[1] =  1.0; 	//right
			conf->computer.viewports[conf->window].frustum[2] = -1.0; 	//bottom
			conf->computer.viewports[conf->window].frustum[3] =  1.0; 	//top
			conf->computer.viewports[conf->window].frustum[4] =  1.5; 	//left
			
			conf->computer.viewports[conf->window].direction[0] =  0; 	//X
			conf->computer.viewports[conf->window].direction[1] =  0; 	//Y
			conf->computer.viewports[conf->window].direction[2] = -1; 	//Z
			
			conf->computer.viewports[conf->window].up[0] =  0; 			//X
			conf->computer.viewports[conf->window].up[1] =  1; 			//Y
			conf->computer.viewports[conf->window].up[2] =  0; 			//Z
			
			conf->computer.viewports[conf->window].position[0] =  0; 	//pos X
			conf->computer.viewports[conf->window].position[1] =  0; 	//pos Y
			
			conf->computer.viewports[conf->window].stereoMode=0;		// Mono
			conf->computer.viewports[conf->window].projectionMode=0;	// Planar
			
			conf->computer.inputDevices[0].type=0;
			
			//strcpy(conf->data[conf->window][Configuration::VWAND],"0");
		
			for (index = 0; index < len; index++) {
		
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"id")) {
					strcpy(conf->computer.viewports[conf->window].name,XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "reading window :" << conf->data[conf->window][Configuration::WINDOW] << endl;;
					#endif
				}
			}
		}
		
		#ifdef DEBUG
			cout << "Window in " << conf->window << endl;
		#endif
		
		int mode;
		mode=0;
	
		// Size
		if(!strcmp(XMLString::transcode(localname),"size")) {
			
			
			
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"width")) {
					conf->computer.viewports[conf->window].width =
						(unsigned short int)atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " width size is :"
							 << conf->computer.viewports[conf->window].width << endl;
					#endif
				} else
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"height")) {
					conf->computer.viewports[conf->window].height =
						(unsigned short int)atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " height size is :"
							 << conf->computer.viewports[conf->window].height << endl;
					#endif
				} else 
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"fullscreen")) {
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"TRUE")) {
						conf->computer.viewports[conf->window].fullscreen = true;
						#ifdef DEBUG
							cout << "Window " << conf->window << " is running in fullscreen :"
								 << conf->computer.viewports[conf->window].fullscreen << endl;
						#endif
					}
				}
			}
					 
		} else 
	
		// frustum
		if(!strcmp(XMLString::transcode(localname),"frustum")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"left")) {
					conf->computer.viewports[conf->window].frustum[0] =
						atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " frustum left is :"
							 << conf->computer.viewports[conf->window].width << endl;
					#endif
				} else
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"right")) {
					conf->computer.viewports[conf->window].frustum[1] =
						atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " frustum right is :"
							 << conf->computer.viewports[conf->window].height << endl;
					#endif
				} else 
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"bottom")) {
					conf->computer.viewports[conf->window].frustum[2] =
						atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " frustum bottom is :"
							 << conf->computer.viewports[conf->window].height << endl;
					#endif
				} else 
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"top")) {
					conf->computer.viewports[conf->window].frustum[3] =
						atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " frustum top is :"
							 << conf->computer.viewports[conf->window].height << endl;
					#endif
				} else 
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"near")) {
					conf->computer.viewports[conf->window].frustum[4] =
						atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " frustum near is :"
							 << conf->computer.viewports[conf->window].height << endl;
					#endif
				} 
			}
					 
		} else 
		
		
		// Screen
		if(!strcmp(XMLString::transcode(localname),"screen")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"dir")) {
					conf->computer.viewports[conf->window].direction[0] =
						atof(strtok(XMLString::transcode(attributes.getValue(index))," "));
					conf->computer.viewports[conf->window].direction[1] =
						atof(strtok(NULL," "));
					conf->computer.viewports[conf->window].direction[2] =
						atof(strtok(NULL," "));
					#ifdef DEBUG
						cout << "Window " << conf->window << " direction is :"
							 << conf->computer.viewports[conf->window].direction[0] << " "
							 << conf->computer.viewports[conf->window].direction[1] << " "
							 << conf->computer.viewports[conf->window].direction[2] << " "
							 << endl;
					#endif
				} else
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"up")) {
					conf->computer.viewports[conf->window].up[0] =
						atof(strtok(XMLString::transcode(attributes.getValue(index))," "));
					conf->computer.viewports[conf->window].up[1] =
						atof(strtok(NULL," "));
					conf->computer.viewports[conf->window].up[2] =
						atof(strtok(NULL," "));
					#ifdef DEBUG
						cout << "Window " << conf->window << " direction is :"
							 << conf->computer.viewports[conf->window].up[0] << " "
							 << conf->computer.viewports[conf->window].up[1] << " "
							 << conf->computer.viewports[conf->window].up[2] << " "
							 << endl;
					#endif
				} 
			}
					 
		} else 
	
		
		// Position
		if(!strcmp(XMLString::transcode(localname),"position")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"x")) {
					conf->computer.viewports[conf->window].position[0] =
						(unsigned short int)atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " position X is :"
							 << conf->computer.viewports[conf->window].position[0] << endl;
					#endif
				} else
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"y")) {
					conf->computer.viewports[conf->window].position[1] =
						(unsigned short int)atof(XMLString::transcode(attributes.getValue(index)));
					#ifdef DEBUG
						cout << "Window " << conf->window << " position Y is :"
							 << conf->computer.viewports[conf->window].position[1] << endl;
					#endif
				} 
			}
					 
		} else 
		
		// For stereo	
		if(!strcmp(XMLString::transcode(localname),"view")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"mode")) {
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"mono")) {	
						conf->computer.viewports[conf->window].stereoMode=0;
						#ifdef DEBUG
							cout << "Computer in mono mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"stereo")) {
						conf->computer.viewports[conf->window].stereoMode=1;
						#ifdef DEBUG
							cout << "Computer in stereo mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"right")) {
						conf->computer.viewports[conf->window].stereoMode=2;
						#ifdef DEBUG
							cout << "Computer in right eye mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"left")) {
						conf->computer.viewports[conf->window].stereoMode=3;
						#ifdef DEBUG
							cout << "Computer in left eye mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"auto")) {
						conf->computer.viewports[conf->window].stereoMode=4;
						#ifdef DEBUG
							cout << "Computer in left eye mode" << endl;
						#endif
					}
				}
			}
		} else 
	
		// For spherical
		if(!strcmp(XMLString::transcode(localname),"projection")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"mode")) {

					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"planar")) {
						conf->computer.viewports[conf->window].projectionMode=0;
						#ifdef DEBUG
							cout << "Computer in planar mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"vertical")) {
						conf->computer.viewports[conf->window].projectionMode=1;
						#ifdef DEBUG
							cout << "Computer in vertical mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"horizontal")) {
						conf->computer.viewports[conf->window].projectionMode=2;
						#ifdef DEBUG
							cout << "Computer in horizontal mode" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"spherical")) {
						conf->computer.viewports[conf->window].projectionMode=3;
						#ifdef DEBUG
							cout << "Computer in spherical mode" << endl;
						#endif
					}
				}
			}
			
		} else 
		
		// For wand
		//mode=0;
		if(!strcmp(XMLString::transcode(localname),"wand")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"mode")) {
					mode=1;
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"no")) {
						conf->computer.inputDevices[0].type=0;
						#ifdef DEBUG
							cout << "Computer without Wand" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"spaceball")) {
						conf->computer.inputDevices[0].type=1;
						#ifdef DEBUG
							cout << "Computer with spaceball" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"tracker")) {
						conf->computer.inputDevices[0].type=2;
						#ifdef DEBUG
							cout << "Computer with tracker" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"intertrax")) {
						conf->computer.inputDevices[0].type=3;
						#ifdef DEBUG
							cout << "Computer with intertrax" << endl;
						#endif
					} else 
					if(!strcmp(XMLString::transcode(attributes.getValue(index)),"glove")) {
						conf->computer.inputDevices[0].type=4;
						#ifdef DEBUG
							cout << "Computer with glove" << endl;
						#endif
					}
				}
			}
			if(!mode) {
				conf->computer.inputDevices[0].type=1;
			}
		} else 
		
		// For Joystick
		if(!strcmp(XMLString::transcode(localname),"joystick")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"device")) {
					strcpy(conf->computer.navigationDevice[0].device,XMLString::transcode(attributes.getValue(index)));
				}
			}
		} else
		
		// For Joystick Maps
		if(!strcmp(XMLString::transcode(localname),"map")) {
			for (index = 0; index < len; index++) {
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"source")) {
					strcpy(conf->computer.navigationDevice[0].device,XMLString::transcode(attributes.getValue(index)));
				} else 
				if(!strcmp(XMLString::transcode(attributes.getLocalName(index)),"destiny")) {
					strcpy(conf->computer.navigationDevice[0].device,XMLString::transcode(attributes.getValue(index)));
				}
			}
		}
		

	
	}


}

void SAX2PrintHandlers::endElement(const XMLCh* const uri,const XMLCh* const localname,const XMLCh* const qname) {
		
	if(!strcmp(XMLString::transcode(localname),"computer")) {
		this->localComputer=false; // not more in computer tag
	}
		
	if(localComputer) {
		if(!strcmp(XMLString::transcode(localname),"window")) {
			conf->window++;	// finishing a new window
		}
	}
}

void SAX2PrintHandlers::endDocument(){
	if(!this->foundComputer) {
		cerr << "Computer : " << this->computer_name << " not found in database" << endl;
	}
}
