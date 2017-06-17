
#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <string>

namespace trackerml {
    
    using namespace std;

    class CommandController {
        
        public:
        
        const static int COMPILE = 0, HELP = 1, DEBUG = 2;
        
        void execute(int argc, char *argv[]);
        
        void doCompileCommand(string source, string output);
        
        void doHelpCommand();
        
        void doDebugCommand();
    };
    
}

#endif /* COMMANDCONTROLLER_H */
