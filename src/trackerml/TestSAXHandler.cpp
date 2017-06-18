
#include "TestSAXHandler.h"

#include <iostream>
#include <tuple>
#include <vector>

namespace trackerml {
            
    void TestSAXHandler::characters(std::string text) {
        std::cout << text << std::endl;
    }
    
    void TestSAXHandler::comment(std::string text) {}
    
    void TestSAXHandler::endDocument() {
    }
    
    void TestSAXHandler::endElement(std::string name) {
        std::cout << "</" + name + ">" << std::endl;
    }
    
    void TestSAXHandler::error(std::string msg, int lineCount, int charCount) {
        std::cerr << lineCount << ":" << charCount << ": error:" + msg << std::endl; 
    }
    
    void TestSAXHandler::processingInstruction(std::string text) {}
    
    void TestSAXHandler::startDocument() {
    }
    
    void TestSAXHandler::startElement(std::string name, std::vector<std::pair <std::string, std::string> > attributes) {
        std::cout << "<" + name + "";
        for (auto attr: attributes) {
            std::cout << " " + (std::get<0>(attr)) + "=\"" + (std::get<1>(attr)) + "\"";
        }
        std::cout << ">" << std::endl;
    }
    
}
