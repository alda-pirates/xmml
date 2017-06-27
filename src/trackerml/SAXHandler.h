
#ifndef SAXHANDLER_CPP
#define SAXHANDLER_CPP

#include <string>
#include <tuple>
#include <vector>

namespace trackerml {
	
	using namespace std;
    
    class SAXHandler {
        public:
        
        virtual void characters(string text)=0;
        virtual void comment(string text)=0;
        virtual void endDocument()=0;
        virtual void endElement(string name)=0;
        virtual void error(string msg, int lineCount, int charCount)=0;
        virtual void processingInstruction(string text)=0;
        virtual void startDocument()=0;
        virtual void startElement(string name, vector<pair <string, string> > attributes)=0;
    };
    
}

#endif /* SAXHANDLER_CPP */
