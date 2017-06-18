
#include "StringStream.h"

#include <iostream>
#include <vector>
#include <tuple>

namespace trackerml {

    StringStream::StringStream() : buffer("") { 
        start = 0;
        length = 0;
        lineCount = 0;
        columnCount = 0;
    }
        
    StringStream::StringStream(std::string buff) {
        setBuffer(buff);
        lineCount = 0;
        columnCount = 0;
    }
    
    bool StringStream::empty() {
        return length <= 0;
    }
    
    std::string StringStream::getBuffer() {
        return buffer;
    }
    
    unsigned int StringStream::getColumnCount() {
        return columnCount;
    }
    
    unsigned int StringStream::getLineCount() {
        return lineCount;
    }
    
    unsigned int StringStream::getLength() {
        return length;
    }
    
    unsigned int StringStream::getStart() {
        return start;
    }
    
    char StringStream::peek() {
        if (empty()) {
            return 0;
        } 
        return buffer[start];
    }
    
    bool StringStream::peekCompare(std::string str) {
        if (empty() || length < str.length()) {
            return false;
        }
        
        return buffer.substr(start, str.length()) == str;
    }   
    
    bool StringStream::peekIn(std::vector<std::string> compareList) {
        
        if (empty()) {
            return false;
        }
        
        for (auto str: compareList) {
            if (length >= str.length() && buffer.substr(start, str.length()) == str) {
                return true;
            }
        }
        
        return false;
    }
    
    std::string StringStream::peekString(unsigned int size = 1) {
        if (empty() || length < size) {
            return "";
        }
        
        return buffer.substr(start, size);
    }
    
    std::string StringStream::read(unsigned int size = 1) {
        std::string read = "";
        
        if (empty()) {
            return 0;
        }
        
        while (size > 0) {
            read += buffer[start];
            start++;
            length--;
            columnCount++;
            if (buffer[start] == '\n') {
                lineCount++;
                columnCount = 0;
            }
            size--;
        }
        
        return read;
    }
    
    std::string StringStream::readUntil(std::vector<std::string> compareList) {
        std::string read = "";
        bool stop;
        
        if (empty()) {
            return read;
        }
        
        for (auto str: compareList) {
            if (length >= str.length() && buffer.substr(start, str.length()) == str) {
                return read;
            }
        }
        
        stop = false;
        while (! stop) {
        
            if (empty()) {
                stop = true;
            } else {
                for (auto str: compareList) {
                    if (length >= str.length() && buffer.substr(start, str.length()) == str) {
                        stop = true;
                        break;
                    }
                }
            } 
            
            if (! stop) {
                read += buffer[start];
                start++;
                length--;
                columnCount++;
                if (buffer[start] == '\n') {
                    lineCount++;
                    columnCount = 0;
                }
            }
        }
        
        return read;
    }
    
    std::string StringStream::readWhile(std::vector<std::string> compareList) {
        bool stop = false;
        std::string read = "";
        
        while (!stop) {
        
            if (empty()) {
                stop = true;
                
            } else {
                
                stop = true;
                for (auto str: compareList) {
                    if (length >= str.length() && buffer.substr(start, str.length()) == str) {
                        stop = false;
                        read += str;
                        start += str.length();
                        length -= str.length();
                        break;
                    }
                }
                
            } 
        }
        
        for (auto ch:read) {
            columnCount++;
            if (ch == '\n') {
                lineCount++;
                columnCount = 0;
            }
        }
        
        return read;
    }
    
    std::string StringStream::reverseReadWhile(std::vector<std::string> compareList) {
        bool stop = false;
        std::string read = "";
        
        while (!stop) {
            
            if (empty()) {
                stop = true;
                
            } else {
                
                stop = true;
                for (auto str: compareList) {
                    if (
                		length >= str.length() && 
                		buffer.substr(start+length-str.length(),str.length()) == str
                    ) {
                        stop = false;
                        read = str + read;
                        length -= str.length();
                        break;
                    }
                }
                
            } 
        }
        
        return read;
    }
    
    void StringStream::setBuffer(std::string buff) {
        buffer = buff;
        start = 0;
        length = buff.length();
    }
    
    std::string StringStream::toString() {
        return buffer.substr(start, length);
    }
    
    std::string StringStream::trim(std::string str) {
        StringStream buff(str);
        
        buff.readWhile({" ", "\n", "\r", "\t"});
        buff.reverseReadWhile({" ", "\n", "\r", "\t"});
        
        return buff.toString();
    } 
    
}
