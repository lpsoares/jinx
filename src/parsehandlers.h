#ifndef __parsehandlers_h
#define __parsehandlers_h

#include <xercesc/sax/HandlerBase.hpp>

// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

#include "wand/wand.h"


#define POS 1
#define ANG 2
#define FIN 3
#define BUT 4

#define VW 0
#define VX 1
#define VY 2
#define VZ 3

class PParseHandlers : public HandlerBase {

	public :
	
    PParseHandlers();
    ~PParseHandlers();

	int data_type;
	int xyz_value;
	
	float posx,posy,posz;
	float angx,angy,angz;
	bool  lft,mid,rgt;
	float quaternion[4];
	int type;
	
	float fingers[20]; // this is used just in case of gloves
	
	int gettype() {
		return type;
	}
	
	float getquaternionw() {
		return quaternion[0];
	}
	
	
	float getquaternionx() {
		return quaternion[1];
	}
	
	
	float getquaterniony() {
		return quaternion[2];
	}
	
	float getquaternionz() {
		return quaternion[3];
	}
	
	bool getlft() {
		return lft;
	}
	
	bool getmid() {
		return mid;
	}
	
	bool getrgt() {
		return rgt;
	}
	
	float *getfingers(){
		return fingers;
	}
	
	float getposx() {
		return posx;
	}
	
	float getposy() {
		return posy;
	}
	
	float getposz() {
		return posz;
	}
	
	float getangx() {
		return angx;
	}
	
	float getangy() {
		return angy;
	}
	
	float getangz() {
		return angz;
	}
	
	
    unsigned int getElementCount() const {
        return fElementCount;
    }

    unsigned int getAttrCount() const {
        return fAttrCount;
    }

    unsigned int getCharacterCount() const {
        return fCharacterCount;
    }

    bool getSawErrors() const {
        return fSawErrors;
    }

    unsigned int getSpaceCount() const {
        return fSpaceCount;
    }
	
    void startElement(const XMLCh* const name, AttributeList& attributes);
	
	void endElement(const XMLCh* const name, AttributeList& attributes);
	
    void characters(const XMLCh* const chars, const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
    void resetDocument();

	void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);

    private:

	unsigned int    fElementCount;
    unsigned int    fAttrCount;
    unsigned int    fCharacterCount;
    
    unsigned int    fSpaceCount;
    bool            fSawErrors;
};


#endif
