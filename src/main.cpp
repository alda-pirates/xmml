
//#include "trackerml/StringStream.h"
#include "trackerml/XMFile.h"
//#include "trackerml/CommandController.h"

//#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/*
class IBase {
    public:
        virtual void doStuff() {
        }
};

void doStuff(IBase &thing) {
    thing.doStuff();
}
*/

int main(int argc, char *argv[]) {
    /*
    class : public IBase {
        public:
            void doStuff() {
                cout << "Anonymous class 1 call!" << endl;
            }
    } thing;
    
    doStuff(thing);
    
    class : public IBase {
        public:
            void doStuff() {
                cout << "Anonymous class 2 call!" << endl;
            }
    } thing2;
    
    doStuff(thing2);
    */
    /*
    string test = "";
    
    int i, j, n = 2;
    
    for (n = 4; n <= 30; n++) {
        
        test += n + ":\n";
        
        for (i = 3; i <= n; i++) {
            for (j = 3; j <= n; j++) {
                if ((j % 2 && i == (3*j+1)/2) || (j % 2 == 0 && i == j/2)) {
                    test += "1";
                } else {
                    test += "0";
                }
            }
        }
        
        test += "\n\n";
        
        for (i = 3; i <= n; i++) {
            for (j = 3; j <= n; j++) {
                if ((j % 2 && i == (3*j+1)/2) || (j % 2 == 0 && i == j/2)) {
                    test += "1";
                } else {
                    test += "0";
                }
            }
            test += "\n";
        }
        test += "\n";
    }
    */
    
    //struct trackerml::XMHeader header;
            
    //ifstream file("./songs/Untitled.xm", ios::binary | ios::in);
    
    //file.read((char*)&header, sizeof(header));
    
    //cout << std::hex << header.flags << endl;
    
    //file.close();
    
    //trackerml::CommandController commandController;
    
    //commandController.execute(argc, argv);
    
    trackerml::XMFile xm;
    xm.setSongName("Houlala");
    xm.setSongLength(8);
    xm.setRestartPosition(2);
    xm.setChannelsCount(4);
    xm.setPatternsCount(8);
    xm.setInstrumentsCount(4);
    xm.setTempo(225);
    xm.setBpm(3);
    //xm->setPatterns();
    xm.write(cout);
        
} 
