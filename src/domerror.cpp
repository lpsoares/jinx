
#include "domerror.h"

void DOMTreeErrorReporter::warning(const SAXParseException&) {
    // Ignore all warnings
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch) {
    fSawErrors = true;
    cerr << "Error reading XML file \""; cerr << XMLString::transcode(toCatch.getSystemId());
	cerr << "\", line "; cerr << toCatch.getLineNumber();
	cerr << ", column "; cerr << toCatch.getColumnNumber();
    cerr << endl << "   Message: "; cerr << XMLString::transcode(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch) {
    fSawErrors = true;
    cerr << "Fatal Error reading XML file \""; cerr << XMLString::transcode(toCatch.getSystemId());
	cerr << "\", line "; cerr << toCatch.getLineNumber();
	cerr << ", column "; cerr << toCatch.getColumnNumber();
    cerr << endl << "   Message: "; cerr << XMLString::transcode(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::resetErrors() {
    // No-op in this case
}
