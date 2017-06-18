
#include "CommandController.h"
#include "SAXParser.h"
#include "TestSAXHandler.h"
#include "XMFile.h"
//#include "StringStream.h"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace trackerml {
    
    void CommandController::doCompile(int argc, char *argv[]) {
    	/*

        if (source.empty()) {
                
            doHelpCommand();
            
        }

        trackerml::SAXParser parser;
        trackerml::TestSAXHandler handler;
        
        parser.parse(source, handler);*/
        
    }
    
    void CommandController::doDebug(int argc, char *argv[]) {
    }
    
    void CommandController::doHelp(int argc, char *argv[]) {
        std::cout 
            << "Usage: tracker-ml {command}" << std::endl
            << "Command:" << std::endl;
            
        for (auto command : commands) {
        	std::cout << '\t' << command << std::endl;
        }
        
        /*
            << "\t-h,--help\t\tDisplay this information" << std::endl
            << "\t-o <file>\t\tPlace the output into <file>" << std::endl
            << "If no -o option is given, the program will place the output into a file named out.xm" << std::endl
            ;
            */
    }
    
    void CommandController::doReadprintxminfo(int argc, char *argv[]) {
		struct trackerml::XMHeader header;
		        
		std::ifstream file(argv[2], std::ios::binary | std::ios::in);
		
		file.read((char*)&header, sizeof(header));
		
		file.close();
		
		std::cout 
			<< "Song name: " << header.songname << std::endl
			<< "Length: " << header.songlength << std::endl
			<< "Restart: " << header.restart << std::endl
			<< "Channels count: " << header.numchannels << std::endl
			<< "Patterns count: " << header.numpatterns << std::endl
			<< "Instruments count: " << header.numinstruments << std::endl
			<< "Tempo: " << header.tempo << std::endl
			<< "BPM: " << header.bpm << std::endl;
    }
    
    void CommandController::doTestwritexmtostdout(int argc, char *argv[]) {
		trackerml::XMFile xm;
		xm.setSongName("Houlala");
		xm.setSongLength(1);
		xm.setRestartPosition(0);
		xm.setChannelsCount(4);
		xm.setPatternsCount(0);
		xm.setInstrumentsCount(0);
		xm.setTempo(3);
		xm.setBpm(250);
		//xm.setPatterns();
		xm.write(std::cout);
    }
        
    void CommandController::execute(int argc, char *argv[]) {
		if (argc < 2) {
			doHelp(argc, argv);
			return;
		}
		
		std::string command = argv[1];
		
		if (command == "compile") {
			doCompile(argc, argv);
			return;
			
		} else if (command == "help") {
			doHelp(argc, argv);
			return;
			
		} else if (command == "debug") {
			doDebug(argc, argv);
			return;
			
		} else if (command == "readprintxminfo") {
			doReadprintxminfo(argc, argv);
			return;
			
		} else if (command == "testwritexmtostdout") {
			doTestwritexmtostdout(argc, argv);
			return;
			
		}
		
		doHelp(argc, argv);
		return;
		
		
		/*
        int i, command = DEBUG;
        
        std::string arg, source, output = "out.xm";
        
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
        */
    }
    
}
