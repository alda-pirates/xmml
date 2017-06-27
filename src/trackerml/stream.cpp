
#include <algorithm>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <string>

#include "stream.h"

namespace trackerml {
    
    using namespace std;
    
    void streamWriteString(ostream &os, string str, unsigned int size) {
        unsigned int i;
        for (i = 0; i < size && i < str.size(); i++) {
            os.put(str[i]);
        }
        for (; i < size; i++) {
            os.put(0);
        }
    }
    
    string streamReadString(istream &is, unsigned int size) {
        char songName[17];
        is.read(songName, 17);
        return string(songName);
    }
    
    void streamWriteInt(ostream &os, int n, int size, ByteOrder order) {
        for (int i = 0; i < size; i++) {
            if (ByteOrder::littleEndian == order) {
                os.put( 0xff & (n >> (i << 3)) );
            } else {
                os.put( 0xff & (n >> ((size - i - 1) << 3)) );
            }
        }
    }
    
    int streamReadInt(istream &is, int size, ByteOrder order) {
        int n = 0;
        for (int i = 0; i < size; i++) {
            if (ByteOrder::littleEndian == order) {
                n += is.get() << (i << 3);
            } else {
                n += is.get() << ((size - i - 1) << 3);
            }
        }
        return n;
    }

}
