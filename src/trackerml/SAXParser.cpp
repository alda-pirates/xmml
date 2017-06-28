
#include "SAXParser.h"
#include "SAXHandler.h"
#include "StringStream.h"
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

namespace trackerml {

    using namespace std;

    void SAXParser::setError(string msg) {
        error = true;
        errorMsg = msg;
        errorLine = buffer.getLineCount();
        errorColumn = buffer.getColumnCount();
    }

    void SAXParser::init(string str = "") {
        buffer = StringStream(str);
        error = false;
        errorMsg = "";
        errorLine = 0;
        errorColumn = 0;
    }

    bool SAXParser::parse(string fileName, SAXHandler &hand) {
        string line, read = "", characters = "";

        handler = &hand;

        ifstream file(fileName);

        while (getline(file, line)) {
            read += (read.empty() ? "" : "\n") + line;
        }

        file.close();

        init(read);

        handler->startDocument();

        while (!buffer.empty()) {
            buffer.readWhile({" ", "\n", "\r", "\t"});

            if (buffer.peekCompare("<!--")) {
                if (!readComment()) {
                    break;
                }
            } else if (buffer.peekCompare("<?")) {
                if (!readProcessingInstruction()) {
                    break;
                }
            } else if (buffer.peekCompare("<")) {
                if (!readElement()) {
                    break;
                }
            }

            characters = buffer.readUntil({" ", "\n", "\r", "\t"});
            if (characters.length()) {
                setError("invalid characters");
                break;
            }
        }

        if (error) {
            handler->error(errorMsg, errorLine, errorColumn);
            return false;
        }

        handler->endDocument();

        return true;
    }

    bool SAXParser::readElement() {
        vector<pair<string, string>> attributes;
        string characters, name;

        if (!buffer.peekCompare("<")) {
            setError("expected openning bracket");
            return false;
        }

        buffer.read(1);

        buffer.readWhile({" ", "\n", "\r", "\t"});

        name = buffer.readUntil({" ", "\n", "\r", "\t", "/>", ">"});

        if (name.empty()) {
            setError("expected element name");
            return false;
        }

        buffer.readWhile({" ", "\n", "\r", "\t"});

        attributes = readAttributes();
        if (error) {
            return false;
        }

        buffer.readWhile({" ", "\n", "\r", "\t"});

        if (buffer.peekCompare("/>")) {
            buffer.read(2);
            return true;
        }

        if (!buffer.peekCompare(">")) {
            setError("expected closing bracket");
            return false;
        }

        buffer.read(1);

        handler->startElement(name, attributes);

        characters += buffer.readWhile({" ", "\n", "\r", "\t"});

        while (!buffer.empty()) {
            if (buffer.peekCompare("<!--")) {
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";

                if (!readComment()) {
                    return false;
                }

            } else if (buffer.peekCompare("<?")) {
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";

                if (!readProcessingInstruction()) {
                    return false;
                }

            } else if (buffer.peekCompare("</" + name + ">")) {
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";

                buffer.read(("</" + name + ">").length());

                handler->endElement(name);

                return true;

            } else if (buffer.peekCompare("<")) {
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";

                if (!readElement()) {
                    return false;
                }

            } else {
                characters += buffer.read(1);
            }

            characters += buffer.readWhile({" ", "\n", "\r", "\t"});
        }

        return true;
    }

    pair<string, string> SAXParser::readAttribute() {
        pair<string, string> attr = make_pair("", "");

        get<0>(attr) =
            buffer.readUntil({" ", "\n", "\r", "\t", "=", ">", "/>"});

        buffer.readWhile({" ", "\n", "\r", "\t"});

        if (buffer.getLength() >= 1 && buffer.peekCompare("=")) {
            buffer.read(1);

            buffer.readWhile({" ", "\n", "\r", "\t"});

            switch (buffer.peek()) {
            case '\'':

                buffer.read(1);
                get<1>(attr) = buffer.readUntil({"'"});

                if (!buffer.peekCompare("'")) {
                    setError("expected end quote attribute");
                }

                buffer.read(1);

                break;

            case '"':

                buffer.read(1);
                get<1>(attr) = buffer.readUntil({"\""});

                if (!buffer.peekCompare("\"")) {
                    setError("expected end quote attribute");
                }

                buffer.read(1);

                break;

            default:
                get<1>(attr) =
                    buffer.readUntil({" ", "\n", "\r", "\t", ">", "/>"});
            }
        }

        return attr;
    }

    vector<pair<string, string>> SAXParser::readAttributes() {
        vector<pair<string, string>> attrs;
        pair<string, string> attr;

        attr = readAttribute();
        if (error) {
            return attrs;
        }

        while (!get<0>(attr).empty()) {
            attrs.push_back(attr);
            attr = readAttribute();
            if (error) {
                return attrs;
            }

            buffer.readWhile({" ", "\n", "\r", "\t"});
        }

        return attrs;
    }

    bool SAXParser::readComment() {
        string lastRead = "";

        buffer.read(4);

        while (!buffer.empty() && lastRead != "-->") {
            lastRead += buffer.read(1);
            if (lastRead.length() > 3) {
                lastRead = lastRead.substr(lastRead.length() - 3, 3);
            }
        }

        if (lastRead != "-->") {
            setError("expected comment end");
            return false;
        } else {
            return true;
        }
    }

    bool SAXParser::readProcessingInstruction() {
        string lastRead = "";

        buffer.read(2);

        while (!buffer.empty() && lastRead != "?>") {
            lastRead += buffer.read(1);
            if (lastRead.length() > 3) {
                lastRead = lastRead.substr(lastRead.length() - 2, 2);
            }
        }

        if (lastRead != "?>") {
            setError("expected processing instruction end");
            return false;
        } else {
            return true;
        }
    }
}
