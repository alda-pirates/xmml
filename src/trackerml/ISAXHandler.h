
#ifndef ISAXHANDLER_CPP
#define ISAXHANDLER_CPP

#include <string>
#include <tuple>
#include <vector>

namespace trackerml {
    
    class ISAXHandler {
        public:
        
        virtual void characters(std::string text)=0;
        virtual void comment(std::string text)=0;
        virtual void endDocument()=0;
        virtual void endElement(std::string name)=0;
        virtual void error(std::string msg, int lineCount, int charCount)=0;
        virtual void processingInstruction(std::string text)=0;
        virtual void startDocument()=0;
        virtual void startElement(std::string name, std::vector<std::pair <std::string, std::string> > attributes)=0;
    };
    
}

#endif /* ISAXHANDLER_CPP */
