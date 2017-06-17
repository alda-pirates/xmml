
#include "trackerml/TestSAXHandler.h"

#include <iostream>
#include <tuple>
#include <vector>

namespace trackerml {
    
    using namespace std;
            
    void TestSAXHandler::characters(string text) {
        cout << text << endl;
    }
    
    void TestSAXHandler::comment(string text) {}
    
    void TestSAXHandler::endDocument() {
    }
    
    void TestSAXHandler::endElement(string name) {
        cout << "</" + name + ">" << endl;
    }
    
    void TestSAXHandler::error(string msg, int lineCount, int charCount) {
        cerr << lineCount << ":" << charCount << ": error:" + msg << endl; 
    }
    
    void TestSAXHandler::processingInstruction(string text) {}
    
    void TestSAXHandler::startDocument() {
    }
    
    void TestSAXHandler::startElement(string name, vector<pair <string, string> > attributes) {
        cout << "<" + name + "";
        for (auto attr: attributes) {
            cout << " " + (get<0>(attr)) + "=\"" + (get<1>(attr)) + "\"";
        }
        cout << ">" << endl;
    }
    
}
