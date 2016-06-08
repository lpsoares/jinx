
#include "xmlBase.h"

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

#include <iostream>

using namespace std;

// This is necessare for xerces 2.3.0
#ifdef XERCES_CPP_NAMESPACE_USE
	XERCES_CPP_NAMESPACE_USE
#endif

xmlBase::xmlBase() {
	// Initialize the XML4C2 system
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		cerr << "Error during initialization of XML :" << endl;
		cerr << XMLString::transcode(toCatch.getMessage()) << endl;
	}
}

xmlBase::~xmlBase() {
	XMLPlatformUtils::Terminate(); // Call the termination method
}
