
#ifndef XMINSTRUMENT_H
#define XMINSTRUMENT_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "XMSample.h"

namespace trackerml {
    using namespace std;


    struct XMInstrumentHeader {
        uint32_t size;   // (dword) Instrument header size
        int8_t name[22]; // (char) Instrument name
        uint8_t type;    // (byte) Instrument type (always 0). This seems pretty
                         // random, don't assume it's zero.
        uint16_t numsamples; // (word) Number of samples in instrument
    };

    // If the number of samples > 0, then the this will follow:
    struct XMInstrumentHeaderExtended {
        uint32_t size;          // (dword) instrument sample header size
        uint8_t samplenote[96]; // (byte) Sample number for all notes
        uint8_t volumeenvelopepoints[48];  // (byte) Points for volume envelope
        uint8_t panningenvelopepoints[48]; // (byte) Points for panning envelope
        uint8_t numvolumepoints;           // (byte) Number of volume points
        uint8_t numpanningpoints;          // (byte) Number of panning points
        uint8_t volumesustain;             // (byte) Volume sustain point
        uint8_t volumeloopstart;           // (byte) Volume loop start point
        uint8_t volumeloopend;             // (byte) Volume loop end point
        uint8_t panningsustain;            // (byte) Panning sustain point
        uint8_t panningloopstart;          // (byte) Panning loop start point
        uint8_t panningloopend;            // (byte) Panning loop end point
        uint8_t
            volumetype; // (byte) Volume type: bit 0: On; 1: Sustain; 2: Loop
        uint8_t
            panningtype; // (byte) Panning type: bit 0: On; 1: Sustain; 2: Loop
        uint8_t vibratotype;    // (byte) Vibrato type
        uint8_t vibratosweep;   // (byte) Vibrato sweep
        uint8_t vibratodepth;   // (byte) Vibrato depth
        uint8_t vibratorate;    // (byte) Vibrato rate
        uint16_t volumefadeout; // (word) Volume fadeout
        uint16_t reserved;      // (word) Reserved
    };

    class XMInstrument {
      private:
        string name;
        //XMInstrumentHeader header;
        //XMInstrumentHeaderExtended extHeader;
        vector<XMSample> samples;
        
        void writeHeader(ostream &os);
        void readHeader(istream &is);

      public:
        XMInstrument();
        void setName(string name);
        void addSample(XMSample sample);
        void setSample(size_t n, XMSample sample);
        void removeSample(XMSample sample);
        size_t samplesCount();
        void write(ostream &os);
    };
}

#endif
