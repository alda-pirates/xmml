
#include "CommandController.h"
#include "SAXParser.h"
#include "TestSAXHandler.h"
#include "XMFile.h"
#include "XMSample.h"
//#include "StringStream.h"

#include <algorithm>
#include <cstring>
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

    void CommandController::doDebug(int argc, char *argv[]) {}

    void CommandController::doHelp(int argc, char *argv[]) {
        cout << "Usage: tracker-ml {command}" << '\n' << "Command:" << '\n';

        for (auto command : commands) {
            cout << '\t' << command << '\n';
        }

        /*
      << "\t-h,--help\t\tDisplay this information" << '\n'
      << "\t-o <file>\t\tPlace the output into <file>" << '\n'
      << "If no -o option is given, the program will place the output into a
      file
      named out.xm" << '\n'
      ;
      */
    }

    void CommandController::doTestread(int argc, char *argv[]) {
        
        trackerml::XMSample sample;
        /*
        trackerml::XMFile xmFile;
        
        ifstream is(argv[2], ios::binary | ios::in);
        
        xmFile.read(is);

        cout << "Song name: " << xmFile.getSongName() << '\n'
             << "Length: " << xmFile.getSongLength() << '\n'
             << "Restart: " << xmFile.getRestartPosition() << '\n'
             << "Channels count: " << xmFile.getChannelsCount() << '\n'
             << "Frequency table: " << ((int)xmFile.getFrequencyTableType() ? "linear" : "amiga") << "\n"
             << "BPM: " << xmFile.getBpm() << '\n'
             << "Tempo: " << xmFile.getTempo() << "\n\n";
             
        cout << "Patterns:\n";
        for (auto n : xmFile.getPatternsTable()) {
          cout << "  " << to_string(n) << '\n';
        }
        */
        //sample.read(is);
    }
    
    void CommandController::doTestwrite(int argc, char *argv[]) {
        trackerml::XMInstrument instrumentKick;
        instrumentKick.setName("Kick");
        
        trackerml::XMInstrument instrumentSnare;
        instrumentSnare.setName("Snare");

        trackerml::XMFile xm;
        xm.setSongName("Houlala");
        xm.setRestartPosition(0);
        xm.setChannelsCount(4);
        xm.setTempo(3);
        xm.setBpm(250);

        xm.addInstrument(instrumentKick);
        xm.addInstrument(instrumentSnare);

        // xm.setPatternsTable();

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

        } else if (command == "testread") {
            doTestread(argc, argv);
            return;

        } else if (command == "testwrite") {
            doTestwrite(argc, argv);
            return;
        }

        doHelp(argc, argv);
    }
}
