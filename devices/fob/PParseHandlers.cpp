// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include "PParse.hpp"

// ---------------------------------------------------------------------------
//  PParseHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
PParseHandlers::PParseHandlers() :

    fElementCount(0)
    , fAttrCount(0)
    , fCharacterCount(0)
    , fSpaceCount(0)
{
}

PParseHandlers::~PParseHandlers()
{
}


// ---------------------------------------------------------------------------
//  PParseHandlers: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void PParseHandlers::startElement(const   XMLCh* const    name
                                    ,       AttributeList&  attributes)
{
    fElementCount++;
    fAttrCount += attributes.getLength();
	
	cout << XMLString::transcode(name) << endl;
	
}

void PParseHandlers::characters(  const   XMLCh* const    chars
								    , const unsigned int    length)
{
	cout << XMLString::transcode(chars) << endl;
    fCharacterCount += length;
}

void PParseHandlers::ignorableWhitespace( const   XMLCh* const chars
										    , const unsigned int length)
{
    fSpaceCount += length;
}

void PParseHandlers::resetDocument()
{
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}


// ---------------------------------------------------------------------------
//  PParseHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void PParseHandlers::error(const SAXParseException& e)
{
    cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void PParseHandlers::fatalError(const SAXParseException& e)
{
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void PParseHandlers::warning(const SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

