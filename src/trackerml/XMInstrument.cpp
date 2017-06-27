
#include <cstring>
#include <iostream>
#include <cstdint>
#include <string>

#include "stream.h"
#include "XMInstrument.h"
#include "XMSample.h"

namespace trackerml {

    using namespace std;
    
    XMInstrument::XMInstrument() { }

    void XMInstrument::setName(string name) {
        this->name = name;
    }

    void XMInstrument::addSample(XMSample sample) {}

    void XMInstrument::setSample(size_t n, XMSample sample) {}

    void XMInstrument::removeSample(XMSample sample) {}

    size_t XMInstrument::samplesCount() { return samples.size(); }

    void XMInstrument::write(ostream &os) {
        writeHeader(os);
        
        /*
        if (header.numsamples > 0) {
            os.write((char *)&extHeader, sizeof(extHeader));
        }

        for (auto sample : samples) {
            const XMSampleHeader sampleHeader = sample.getHeader();
            const uint8_t *data = sample.getData();

            os.write((char *)&sampleHeader, sizeof(sampleHeader));

            os.write((char *)data, sampleHeader.samplesize);
        }
        */
    }
    
    void XMInstrument::writeHeader(ostream &os) {
    }
    
    void XMInstrument::readHeader(istream &is) {
    }
}
