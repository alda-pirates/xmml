
#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include "trackerml/ISAXHandler.h"

#include <string>
#include <tuple>
#include <vector>

namespace trackerml {
    
    using namespace std;

    class StringStream {
        
        protected:
        
            string buffer; // Could take an istream instead
            int start;
            int length;
            int lineCount;
            int columnCount;
        
        public:
        
            StringStream();
        
            StringStream(string buff);
            
            bool empty();
            
            string getBuffer();
            
            int getColumnCount();
            
            int getLineCount();
            
            int getLength();
            
            int getStart();
            
            char peek();
            
            bool peekCompare(string str);
            
            bool peekIn(vector<string> compareList);
            
            string peekString(int size);
            
            string read(int size);
            
            string readUntil(vector<string> compareList);
            
            string readWhile(vector<string> compareList);
            
            string reverseReadWhile(vector<string> compareList);
            
            void setBuffer(string buff);
            
            string toString();
            
            static string trim(string str);
    
    };
    
}

#endif /* STRINGSTREAM_H */
