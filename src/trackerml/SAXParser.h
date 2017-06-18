
#ifndef SAXPARSER_H
#define SAXPARSER_H

#include "StringStream.h"
#include "ISAXHandler.h"

namespace trackerml {

    class SAXParser {
        
        protected:
            
            StringStream buffer;
            
            bool error = false;
            
            std::string errorMsg;
            
            int errorLine = 0;
            
            int errorColumn = 0;
            
            void setError(std::string msg);
        
        public:
        
            ISAXHandler *handler;
            
            void init(std::string str);
            
            bool parse(std::string fileName, ISAXHandler &hand);
            
            bool readElement();
            
            std::pair<std::string, std::string> readAttribute();
            
            std::vector<std::pair <std::string, std::string> > readAttributes();
            
            bool readComment();
            
            bool readProcessingInstruction();
    
    };
    
}

#endif /* SAXPARSER_H */
