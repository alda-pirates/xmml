
#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <vector>
#include <string>

namespace trackerml {

    class CommandController {
        
        public:
        
    	std::vector<std::string> commands = {
    		"compile", 
    		"help",
    		"debug",
    		"readprintxminfo",
    		"testwritexmtostdout"
    	};
        
        void execute(int argc, char *argv[]);
        
        void doCompile(int argc, char *argv[]);
        
        void doHelp(int argc, char *argv[]);
        
        void doDebug(int argc, char *argv[]);
        
        void doReadprintxminfo(int argc, char *argv[]);
        
        void doTestwritexmtostdout(int argc, char *argv[]);
    };
    
}

#endif /* COMMANDCONTROLLER_H */
