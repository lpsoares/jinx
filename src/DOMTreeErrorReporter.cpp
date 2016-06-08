
#include <xercesc/sax/SAXParseException.hpp>
#include "DOMTreeErrorReporter.hpp"

#include <iostream>

#include <stdlib.h>

#include <string>

void DOMTreeErrorReporter::warning(const SAXParseException&)
{
    //
    // Ignore all warnings.
    //
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
    fSawErrors = true;
    cerr << "Error reading XML file \"" << StrX(toCatch.getSystemId())
		 << "\", line " << toCatch.getLineNumber()
		 << ", column " << toCatch.getColumnNumber()
         << "\n   Message: " << StrX(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
    fSawErrors = true;
    cerr << "Fatal Error reading XML file \"" << StrX(toCatch.getSystemId())
		 << "\", line " << toCatch.getLineNumber()
		 << ", column " << toCatch.getColumnNumber()
         << "\n   Message: " << StrX(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::resetErrors()
{
    // No-op in this case
}


