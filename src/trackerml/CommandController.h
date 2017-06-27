
#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <string>
#include <vector>

namespace trackerml {

    using namespace std;

    class CommandController {
      public:
        vector<string> commands = {"compile", "help", "debug", "testread",
                                   "testwrite"};

        void execute(int argc, char *argv[]);

        void doCompile(int argc, char *argv[]);

        void doHelp(int argc, char *argv[]);

        void doDebug(int argc, char *argv[]);

        void doTestread(int argc, char *argv[]);

        void doTestwrite(int argc, char *argv[]);
    };
}

#endif /* COMMANDCONTROLLER_H */
