
#include    <xercesc/sax/HandlerBase.hpp>

class PParseHandlers : public HandlerBase {

	public :
	
    PParseHandlers();
    ~PParseHandlers();

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

