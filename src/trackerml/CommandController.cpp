
#include "CommandController.h"
#include "SAXParser.h"
#include "TestSAXHandler.h"
#include "XMFile.h"
//#include "StringStream.h"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace trackerml {
	
	using namespace std;
    
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
        cout 
            << "Usage: tracker-ml {command}" << '\n'
            << "Command:" << '\n';
            
        for (auto command : commands) {
        	cout << '\t' << command << '\n';
        }
        
        /*
            << "\t-h,--help\t\tDisplay this information" << '\n'
            << "\t-o <file>\t\tPlace the output into <file>" << '\n'
            << "If no -o option is given, the program will place the output into a file named out.xm" << '\n'
            ;
            */
    }
    
    void CommandController::doReadprintxminfo(int argc, char *argv[]) {
		struct trackerml::XMHeader header;
		        
		ifstream file(argv[2], ios::binary | ios::in);
		
		file.read((char*)&header, sizeof(header));
		
		file.close();
		
		cout 
			<< "Song name: " << header.songname << '\n'
			<< "Length: " << header.songlength << '\n'
			<< "Restart: " << header.restart << '\n'
			<< "Channels count: " << header.numchannels << '\n'
			<< "Patterns count: " << header.numpatterns << '\n'
			<< "Instruments count: " << header.numinstruments << '\n'
			<< "Tempo: " << header.tempo << '\n'
			<< "BPM: " << header.bpm << '\n';
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
		xm.write(cout);
    }
        
    void CommandController::execute(int argc, char *argv[]) {
		if (argc < 2) {
			doHelp(argc, argv);
			return;
		}
		
		string command = argv[1];
		
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
        */
    }
    
}
