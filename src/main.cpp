
#include "trackerml/CommandController.h"

#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    trackerml::CommandController commandController;

    commandController.execute(argc, argv);
}
