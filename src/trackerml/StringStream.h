
#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include "ISAXHandler.h"

#include <string>
#include <tuple>
#include <vector>

namespace trackerml {
	
	using namespace std;

    class StringStream {
        
        protected:
        
            string buffer; // Could take an istream instead
            unsigned int start;
            unsigned int length;
            unsigned int lineCount;
            unsigned int columnCount;
        
        public:
        
            StringStream();
        
            explicit StringStream(string buff);
            
            bool empty();
            
            string getBuffer();
            
            unsigned int getColumnCount();
            
            unsigned int getLineCount();
            
            unsigned int getLength();
            
            unsigned int getStart();
            
            char peek();
            
            bool peekCompare(string str);
            
            bool peekIn(vector<string> compareList);
            
            string peekString(unsigned int size);
            
            string read(unsigned int size);
            
            string readUntil(vector<string> compareList);
            
            string readWhile(vector<string> compareList);
            
            string reverseReadWhile(vector<string> compareList);
            
            void setBuffer(string buff);
            
            string toString();
            
            static string trim(string str);
    
    };
    
}

#endif /* STRINGSTREAM_H */
