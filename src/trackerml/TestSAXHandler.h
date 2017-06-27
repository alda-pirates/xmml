
#ifndef TESTSAXHEANDLER_H
#define TESTSAXHEANDLER_H

#include "SAXHandler.h"

namespace trackerml {

    using namespace std;

    class TestSAXHandler : public SAXHandler {

      public:
        void characters(string text);

        void comment(string text);

        void endDocument();

        void endElement(string name);

        void error(string msg, int lineCount, int charCount);

        void processingInstruction(string text);

        void startDocument();

        void startElement(string name, vector<pair<string, string>> attributes);
    };
}

#endif /* TESTSAXHEANDLER_H */
