
#ifndef __domerror_h
#define __domerror_h

#include <iostream>
#include <stdlib.h>

#include <string>

#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>

// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

//#include "xmlConvert.h"

using namespace std;

class DOMTreeErrorReporter : public ErrorHandler {
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DOMTreeErrorReporter() :
       fSawErrors(false)
    {
    }

    ~DOMTreeErrorReporter()
    {
    }

    // -----------------------------------------------------------------------
    //  Implementation of the error handler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& toCatch);
    void error(const SAXParseException& toCatch);
    void fatalError(const SAXParseException& toCatch);
    void resetErrors();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};

inline bool DOMTreeErrorReporter::getSawErrors() const
{
    return fSawErrors;
}

#endif
