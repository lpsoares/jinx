


#include "sax.h"
#include "xmlsax.h"


#include <iostream>


// do nothing this functions above
SAX2PrintHandlers::~SAX2PrintHandlers(){}
void SAX2PrintHandlers::writeChars(const XMLByte* const toWrite){if(toWrite);}
void SAX2PrintHandlers::writeChars(const XMLByte* const toWrite,const unsigned int count,XMLFormatter* const formatter) {}
void SAX2PrintHandlers::unparsedEntityDecl(const     XMLCh* const name , const   XMLCh* const publicId , const   XMLCh* const systemId  , const   XMLCh* const notationName){}
void SAX2PrintHandlers::notationDecl(const   XMLCh* const name, const XMLCh* const publicId , const XMLCh* const systemId){}
void SAX2PrintHandlers::startDocument() {}
void SAX2PrintHandlers::ignorableWhitespace( const   XMLCh* const chars,const  unsigned int length) {}
void SAX2PrintHandlers::processingInstruction(const  XMLCh* const target, const XMLCh* const data) {}

	
// errors	
void SAX2PrintHandlers::error(const SAXParseException& e) {
    cerr << endl << "Error at file ";
	cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line ";
	cerr << e.getLineNumber();
	cerr << ", char ";
	cerr << e.getColumnNumber();
    cerr << endl << "  Message: ";
	cerr << XMLString::transcode(e.getMessage()) << endl;
}

void SAX2PrintHandlers::fatalError(const SAXParseException& e) {
    cerr << endl << "Fatal Error at file ";
	cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line ";
	cerr << e.getLineNumber();
	cerr << ", char ";
	cerr << e.getColumnNumber();
    cerr << endl << "  Message: ";
	cerr << XMLString::transcode(e.getMessage()) << endl;
}

void SAX2PrintHandlers::warning(const SAXParseException& e) {
    cerr << endl << "Warning at file ";
	cerr << XMLString::transcode(e.getSystemId());
	cerr << ", line ";
	cerr << e.getLineNumber();
	cerr << ", char ";
	cerr << e.getColumnNumber();
    cerr << endl << "  Message: ";
	cerr << XMLString::transcode(e.getMessage()) << endl;
}
