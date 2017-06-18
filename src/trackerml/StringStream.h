
#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include "ISAXHandler.h"

#include <string>
#include <tuple>
#include <vector>

namespace trackerml {

    class StringStream {
        
        protected:
        
            std::string buffer; // Could take an istream instead
            unsigned int start;
            unsigned int length;
            unsigned int lineCount;
            unsigned int columnCount;
        
        public:
        
            StringStream();
        
            explicit StringStream(std::string buff);
            
            bool empty();
            
            std::string getBuffer();
            
            unsigned int getColumnCount();
            
            unsigned int getLineCount();
            
            unsigned int getLength();
            
            unsigned int getStart();
            
            char peek();
            
            bool peekCompare(std::string str);
            
            bool peekIn(std::vector<std::string> compareList);
            
            std::string peekString(unsigned int size);
            
            std::string read(unsigned int size);
            
            std::string readUntil(std::vector<std::string> compareList);
            
            std::string readWhile(std::vector<std::string> compareList);
            
            std::string reverseReadWhile(std::vector<std::string> compareList);
            
            void setBuffer(std::string buff);
            
            std::string toString();
            
            static std::string trim(std::string str);
    
    };
    
}

#endif /* STRINGSTREAM_H */
