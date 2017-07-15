
#include "CommandController.h"
#include "SAXParser.h"
#include "TestSAXHandler.h"
#include "XMFile.h"
#include "XMSample.h"
//#include "StringStream.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
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

        trackerml::XMFile song;

        ifstream is(argv[2], ios::binary | ios::in);

        song.read(is);
        
        cout << "<audioml>\n" ;

        cout << "\t<title>" << song.getSongName() << "</title>\n"
             << "\t<length>" << song.getSongLength() << "</length>\n"
             << "\t<restart>" << song.getRestartPosition() << "</restart>\n"
             << "\t<channels-count>" << song.getChannelsCount() << "</channels-count>\n"
             << "\t<frequency-table>" 
             << ((int)song.getFrequencyTableType() ? "linear" : "amiga") << "</frequency-table>\n" 
             << "\t<bpm>" << song.getBpm() << "</bpm>\n"
             << "\t<tempo>" << song.getTempo() << "</tempo>\n\n";

        cout << "\t<song-structure>";
        int i = 0;
        for (auto n : song.getPatternsTable()) {
            if (!(i % 8)) {
                cout << "\n\t\t";
            }
            cout << setw(3) << '#' << (int)n << " ";
            i++;
        }
        cout << "\n\t</song-structure>\n\n";

        i = 0;
        for (auto pattern : song.getPatterns()) {
            int channelsCount = pattern.getChannelsCount();
            int j = 0;
            cout << "\t<pattern id=\"" << i << "\">";
            
            for (auto data : pattern.getData()) {
                if (!(j % channelsCount)) {
                    cout << "\n\t\t";
                }
                cout << data.toString() << " ";
                j++;
            }
            cout << "\n\t</pattern>\n";
            i++;
        }
        
        cout << "</audioml>";
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
