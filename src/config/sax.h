/* FAZER */

#ifndef __sax_h
#define __sax_h

#include "xmlsax.h"

// Includes XML
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

using namespace std;

// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
	XERCES_CPP_NAMESPACE_USE
#endif

class Configuration;
class SAX2PrintHandlers : public DefaultHandler, private XMLFormatTarget {
	
	public:

    SAX2PrintHandlers(const char* const encodingName, const XMLFormatter::UnRepFlags unRepFlags, const bool	expandNamespaces, Configuration* conf );
    ~SAX2PrintHandlers();

	Configuration* conf;

    void writeChars(const XMLByte* const toWrite);
    void writeChars(const XMLByte* const toWrite, const unsigned int count, XMLFormatter* const formatter);
    void endDocument();
    void endElement(const XMLCh* const uri,const XMLCh* const localname, const XMLCh* const qname);
    void characters(const XMLCh* const chars, const unsigned int length);
    void ignorableWhitespace(const   XMLCh* const chars, const unsigned int length);
    void processingInstruction(const   XMLCh* const target, const XMLCh* const data);
    void startDocument();
    void startElement( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes& attributes);
    void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);
    void notationDecl(const XMLCh* const name, const XMLCh* const publicId, const XMLCh* const systemId);
    void unparsedEntityDecl( const   XMLCh* const name, const XMLCh* const publicId, const XMLCh* const systemId , const XMLCh* const notationName );

private :

	char computer_name[256];

	bool localComputer; // in the case that the parse is inside the especific computer it allows read the parameters
	bool foundComputer; // specifies if a computer was found in the table

	XMLFormatter    fFormatter;
	bool			fExpandNS ;
};

#endif
