
#ifndef XMPATTERN_H
#define XMPATTERN_H

#include <cstdint>

namespace trackerml {

    using namespace std;

    struct XMPatternHeader {
        uint32_t size;    // (dword) Pattern header size
        uint8_t packtype; // (byte) Packing type (always 0)
        uint16_t numrows; // (word) Number of rows in pattern (1..256)
        uint16_t
            datasize; // (word) Packed patterndata size. This is zero if the
        // pattern is completely empty and no pattern data follows!
    };

    /*
     A simle packing scheme is adopted, so that the patterns do not become TOO
    large: Since the MSB in the note value is never used, it is used for the
    compression. If the bit is set, then the other bits are interpreted as
    follows:

    bit 0 set (00000001): Note byte follows
        1 set (00000010): Instrument byte follows
        2 set (00000100): Volume column byte follows
        3 set (00001000): Effect byte follows
        4 set (00010000): Effect data byte follows

    For example, if the note and the volume are set, but not the instrument
    neather
    the effects
    then first byte will be 0x80 + 0x01 + 0x04 = 0x85, where 0x80 indicate the
    packing scheme is used,
    0x01 indicate the note is present and the 0x04 indicate the volume is
    present.
    Then the next byte will be
    the value for the note and the second one will be the volume.
    */
    struct XMPatternData {
        uint8_t note; // (byte) Note (0-71, 0 = C-0). Actually note numbers are
                      // 1-96,
        // 97 being the key-off note
        uint8_t instrument;  // (byte) Instrument (0-128). More like 1-128.
        uint8_t volume;      // (byte) Volume column byte (see below)
        uint8_t effect;      // (byte) Effect type
        uint8_t effectparam; // (byte) Effect parameter
    };

    class XMPattern {
      public:
      private:
        XMPatternHeader header;
        XMPatternData *data;
    };
}

#endif
