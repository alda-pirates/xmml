
#include "ISAXHandler.h"
#include "SAXParser.h"
#include "StringStream.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

namespace trackerml {

    void SAXParser::setError(std::string msg) {
        error = true;
        errorMsg = msg;
        errorLine = buffer.getLineCount();
        errorColumn = buffer.getColumnCount();
    }
    
    void SAXParser::init(std::string str = "") {
            buffer = StringStream(str);
            error = false;
            errorMsg = "";
            errorLine = 0;
            errorColumn = 0;
        }
    
    bool SAXParser::parse(std::string fileName, ISAXHandler &hand) {
        
        std::string line, read = "", characters = "";
        
        handler = &hand;
        
        std::ifstream file(fileName);
        
        while (getline(file, line)) {
            
            read += (read.empty() ? "" : "\n") + line;
            
        }
        
        file.close();
        
        init(read);
        
        handler->startDocument();
        
        while (! buffer.empty() ) {
            
            buffer.readWhile({" ", "\n", "\r", "\t"});
            
            if (buffer.peekCompare("<!--")) {
                if (! readComment()) {
                    break;
                }
            } else if (buffer.peekCompare("<?")) {
                if (! readProcessingInstruction()) {
                    break;
                }
            } else if (buffer.peekCompare("<")) {
                if (! readElement( )) {
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
        std::vector<std::pair <std::string, std::string> > attributes;
        std::string characters, name;
        
        if (! buffer.peekCompare("<")) {
            setError("expected openning bracket");
            return false;
        }
        
        buffer.read(1);
            
        buffer.readWhile({" ", "\n", "\r", "\t"});
        
        name = buffer.readUntil({" ", "\n", "\r", "\t", "/>", ">"});
        
        if ( name.empty() ) {
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
        
        if (! buffer.peekCompare(">")) {
            setError("expected closing bracket");
            return false;
        }
        
        buffer.read(1);
        
        handler->startElement(name, attributes);
            
        characters += buffer.readWhile({" ", "\n", "\r", "\t"});
        
        while (! buffer.empty()) {
            if (buffer.peekCompare("<!--")) {
                
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";
                
                if (! readComment()) {
                    return false;
                }
                
            } else if (buffer.peekCompare("<?")) {
                
                characters = StringStream::trim(characters);
                if (!characters.empty()) {
                    handler->characters(characters);
                }
                characters = "";
                
                if (! readProcessingInstruction()) {
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
                
                
                if (! readElement()) {
                    return false;
                }
                
            } else {
                
                characters += buffer.read(1);
                
            }
            
            characters += buffer.readWhile({" ", "\n", "\r", "\t"});
            
        }
        
        return true;
    }

    std::pair<std::string, std::string> SAXParser::readAttribute() {
        std::pair<std::string, std::string> attr = std::make_pair("", "");
        
        std::get<0>(attr) =  buffer.readUntil({" ", "\n", "\r", "\t", "=", ">", "/>"});
        
        buffer.readWhile({" ", "\n", "\r", "\t"});
        
        if (buffer.getLength() >= 1 && buffer.peekCompare("=")) {
            
            buffer.read(1);
        
            buffer.readWhile({" ", "\n", "\r", "\t"});
            
            switch (buffer.peek()) {
                
                case '\'':
            
                    buffer.read(1);
                    std::get<1>(attr) = buffer.readUntil({"'"});
                    
                    if (!buffer.peekCompare("'")) {
                        setError("expected end quote attribute");
                    }
                    
                    buffer.read(1);
                    
                    break;
                    
                case '"':
            
                    buffer.read(1);
                    std::get<1>(attr) = buffer.readUntil({"\""});
                    
                    if (!buffer.peekCompare("\"")) {
                        setError("expected end quote attribute");
                    }
                    
                    buffer.read(1);
                    
                    break;
                    
                default:
                    std::get<1>(attr) = buffer.readUntil({" ", "\n", "\r", "\t", ">", "/>"});
                    
            }
        }
        
        return attr;
    }
    
    std::vector<std::pair <std::string, std::string> > SAXParser::readAttributes() {
        std::vector<std::pair <std::string, std::string> > attrs;
        std::pair<std::string, std::string> attr;
        
        attr = readAttribute();
        if (error) {
            return attrs;
        }
        
        while ( ! std::get<0>(attr).empty() ) {
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
        std::string lastRead = "";
        
        buffer.read(4);
        
        while (!buffer.empty() && lastRead != "-->") {
            
            lastRead +=  buffer.read(1);
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
        std::string lastRead = "";
        
        buffer.read(2);
        
        while (!buffer.empty() && lastRead != "?>") {
            
            lastRead +=  buffer.read(1);
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
