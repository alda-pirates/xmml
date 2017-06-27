
#ifndef STREAM_H
#define STREAM_H

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <string>

namespace trackerml {
    
    enum struct ByteOrder { bigEndian, littleEndian };
    
    using namespace std;
    
    void streamWriteString(ostream &os, string str, unsigned int size);
    
    string streamReadString(istream &is, unsigned int size);
    
    void streamWriteInt(ostream &os, int n, int size, ByteOrder order);
    
    int streamReadInt(istream &is, int size, ByteOrder order);

}

#endif
