
#include "x3dsound.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

Sound2::Sound2() {
	
	this->spatialize=true;
	this->location[0]=0;
	this->location[1]=0;
	this->location[2]=0; 
	
	this->audio=NULL;
	
	this->maxFront=-1;
	
	this->intensity=1;
	
}

AudioClip::AudioClip() {
	
	strcpy(this->url[0],"");
	
	this->vel[0]=0;
	this->vel[1]=0;
	this->vel[2]=0;
	
}

void *AudioClip::read(DOMNode *node) {

	#ifdef DEBUG1
		cout << "\t\t\t\tReading (AudioClip)" << endl;	// Message about debuging
	#endif

	
#ifdef LINUX

	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *tmp1;				// just to read the temporary tokens
	int couter_url=0;		// in the case of many texture files in one specification


	attributes = node->getAttributes ();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "url")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->url[couter_url],tmp1+1,((int)(strlen(tmp1)))-2);
					this->url[couter_url][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->url[couter_url],tmp1);
				}
				
				#ifdef DEBUG
					cout << " Audio [" << couter_url << "]: <" << this->url[couter_url] << ">" << endl;	
				#endif
				
				tmp1 = strtok(NULL," ");
				
				if(strcmp(this->url[couter_url],"")) {
					couter_url++;
				}
			}
			#ifdef DEBUG
				cout << "Sound File Name : " << url[0] << endl;
			#endif
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "description")) {
			strcpy(description,XMLString::transcode(attributes->item(i)->getNodeValue()));
			#ifdef DEBUG
				cout << "description : " << description << endl;
			#endif
		} 

		
	}
	
	if(!strcmp(this->url[0],"")) {
		cout << "Audio Clip not defined " << endl;
	}

#endif
	
	return(NULL);
	
}



void AudioClip::follow() {
}

void Sound2::follow() {
}

void *Sound2::read(DOMNode *node) {

#ifdef LINUX

	DOMNodeList *children;				// variable to hold the node children
	DOMNamedNodeMap *attributes;		//
	attributes = node->getAttributes();	//
	
	char* value;
	char* valueName;
	
	unsigned int i;

	for (i = 0; i < attributes->getLength(); i++) {
		DOMNode *current;
		current = attributes->item (i);
		valueName = XMLString::transcode (current->getNodeName ());
		value = XMLString::transcode (current->getNodeValue ());
		if (!strcmp (valueName, "spatialize")) {
			if(!strcasecmp(value, "false")) {
				#ifdef DEBUG
				cout << "spatialize : " << value << endl;
				#endif
				this->spatialize=false;
			}
		} else
		if (!strcmp(valueName, "location")) {
			this->location[0] = atof(strtok(value," "));
			this->location[1] = atof(strtok(NULL," "));
			this->location[2] = atof(strtok(NULL," "));
			#ifdef DEBUG
			cout << " Location X: " << location[0] <<
					" Location Y: " << location[1] <<
					" Location Z: " << location[2] << endl;
			#endif
		} else
		if ( (!strcmp(valueName, "maxBack")) || (!strcmp(valueName, "maxFront")) ) {
			this->maxFront = atof(value);
			#ifdef DEBUG
			cout << "MaxFront : " << this->maxFront << endl;
			#endif
		}
		if ( (!strcmp(valueName, "intensity")) ) {
			this->intensity = atof(value);
			#ifdef DEBUG
			cout << "Volume : " << this->intensity << endl;
			#endif
		}

	}
	
	children = node->getChildNodes ();
	if (children != NULL) {
		for (i = 0; i < children->getLength(); i++) {			
			if (!strcmp(XMLString::transcode(children->item(i)->getNodeName()),"AudioClip")) {
				this->audio = new AudioClip();
				this->audio->read(children->item(i));
			}
		}
	}
#endif	
	return(NULL);
	
}



int x3d::serializeSound(Sound* temp_sound) {
	
#ifdef LINUX

	#ifdef DEBUG
		cout << "Serializing the Sound" << endl;
	#endif
	
	this->sound = temp_sound;
	this->countSound=0;	//count how many sound node there are
		
	//serializeSoundNode(this->doc);
	serializeSoundNode(this->parser->getDocument());
	
	for(unsigned int f=0;f<this->countSound;f++) {
		if( (sound2[f]->audio!=NULL) ) {
			if(sound2[f]->spatialize) {
				sound->loadSample(sound2[f]->audio->url[0],sound2[f]->location,
				sound2[f]->audio->vel,sound2[f]->maxFront, ((int)(sound2[f]->intensity*255)) );
			} else {
				sound->setSound(sound2[f]->audio->url[0]);
			}
		}
	}
#endif
	
	return(1);
}

void x3d::serializeSoundNode(DOMNode * node) {

#ifdef LINUX
				
	unsigned int i; 				//* variable to counter

	switch (node->getNodeType ()) {

	case (DOMNode::DOCUMENT_NODE):
		DOMNodeList *nodes;
		nodes = node->getChildNodes ();
		if (nodes != NULL) {
			for (i = 0; i < nodes->getLength (); i++) {
				serializeSoundNode(nodes->item(i));
			}
		}
		break;

	case (DOMNode::ELEMENT_NODE):
	
		if (!strcmp (XMLString::transcode (node->getNodeName ()), "Sound")) {
			this->sound2[this->countSound] = new Sound2();
			this->sound2[this->countSound]->read(node);
			if(this->countSound > SOUND_MAX) {
				cerr << "Error, to much sound nodes" << endl;
			}
			else {
				this->countSound++;
			}
		}
							
		DOMNodeList *children;
		children = node->getChildNodes ();
		if (children != NULL){
			for (unsigned int i = 0; i < children->getLength (); i++) {
				serializeSoundNode(children->item (i));			
			}
		}
			
		break;
	}
#endif
}
