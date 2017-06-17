
#include "trackerml/CommandController.h"
#include "trackerml/SAXParser.h"
#include "trackerml/TestSAXHandler.h"

#include <fstream>
#include <iostream>

namespace trackerml {
    
    void CommandController::doCompileCommand(string source, string output) {

        if (source.empty()) {
                
            doHelpCommand();
            
        }

        trackerml::SAXParser parser;
        trackerml::TestSAXHandler handler;
        
        parser.parse(source, handler);
        
    }
    
    void CommandController::doDebugCommand() {
    }
    
    void CommandController::doHelpCommand() {
        cout 
            << "Usage: tracker-ml [options] file" << endl
            << "Options:" << endl
            << "\t-h,--help\t\tDisplay this information" << endl
            << "\t-o <file>\t\tPlace the output into <file>" << endl
            << "If no -o option is given, the program will place the output into a file named out.xm" << endl
            ;
    }
        
    void CommandController::execute(int argc, char *argv[]) {

        int i, command = DEBUG;
        
        string arg, source, output = "out.xm";
        
        for (i = 1; i < argc; ++i) {
            
            arg = argv[i];
            
            if ((arg == "-h") || (arg == "--help")) {
                
                command = HELP;
                
            } else if ((arg == "-o")) {
                
                if (i + 1 < argc) { 
                    
                    output = argv[++i]; 
                    
                    command = COMPILE;
                    
                } else {
                    
                    doHelpCommand();
                    return;
                }
                
            } else if ((arg == "--debug")) {
                
                command = DEBUG;
                
            } else if (source.empty()) {
                
                source = arg;
                
            }
            
        } 
        
        switch (command) {
            case COMPILE:
                doCompileCommand(source, output);
                break;
                
            case HELP:
                doHelpCommand();
                break;
                
            case DEBUG:
                doDebugCommand();
        }
    }
    
}
