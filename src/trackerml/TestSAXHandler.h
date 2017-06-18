
#ifndef TESTSAXHEANDLER_H
#define TESTSAXHEANDLER_H

#include "ISAXHandler.h"

namespace trackerml {
    
    class TestSAXHandler: public ISAXHandler {
        
        public:
            
            void characters(std::string text);
            
            void comment(std::string text);
            
            void endDocument();
            
            void endElement(std::string name);
            
            void error(std::string msg, int lineCount, int charCount);
            
            void processingInstruction(std::string text);
            
            void startDocument();
            
            void startElement(std::string name, std::vector<std::pair <std::string, std::string> > attributes);
            
    };
    
}

#endif /* TESTSAXHEANDLER_H */
