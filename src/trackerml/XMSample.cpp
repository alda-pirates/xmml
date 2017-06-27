
#include <cstring>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "stream.h"
#include "XMSample.h"

namespace trackerml {

    using namespace std;
    
    XMSample::XMSample() :
        loopStart(0),
        loopLength(0),
        volume(0x40),
        fineTune(0),
        loopType(SampleLoopType::noLoop),
        sampleRate(SampleRate::rate8hz),
        panning(0x80),
        relativeNote(0),
        name("") {
    }
    
    uint32_t XMSample::getLoopStart() { return loopStart; }
    uint32_t XMSample::getLoopLength() { return loopLength; }
    uint8_t XMSample::getVolume() { return volume; }
    int8_t XMSample::getFineTune() { return fineTune; }
    SampleLoopType XMSample::getLoopType() { return loopType; }
    SampleRate XMSample::getSampleRate() { return sampleRate; }
    uint8_t XMSample::getPanning() { return panning; }
    int8_t XMSample::getRelativeNote() { return relativeNote; }
    string XMSample::getName() { return name; }
    vector<uint8_t> XMSample::getData() { return data; }
    
    void XMSample::setLoopStart(uint32_t start) { loopStart = start; }
    void XMSample::setLoopLength(uint32_t length) { loopLength = length; }
    void XMSample::setVolume(uint8_t volume) { this->volume = volume; }
    void XMSample::setFineTune(int8_t finetune) { this->fineTune = fineTune; }
    void XMSample::setLoopType(SampleLoopType type) { loopType = type; }
    void XMSample::setSampleRate(SampleRate rate) { sampleRate = rate; }
    void XMSample::setPanning(uint8_t panning) { this->panning = panning; }
    void XMSample::setRelativeNote(int8_t relativeNote) { this->relativeNote = relativeNote; }
    void XMSample::setName(string name) { this->name = name; }
    void XMSample::setData(vector<uint8_t> data) { this->data = data; }
    
    void XMSample::write(ostream &os) {
    }
    void XMSample::read(istream &is) {
        readHeader(is);
    }
    void XMSample::writeHeader(ostream &os) {
}
    
    void XMSample::readHeader(istream &is) {
        ByteOrder order = ByteOrder::littleEndian;
        
        int dataSize = streamReadInt(is, 4, order);
        
        loopStart = streamReadInt(is, 4, order);
        loopLength = streamReadInt(is, 4, order);
        volume = streamReadInt(is, 1, order);
        
        int fineTune = streamReadInt(is, 1, order);
        if (fineTune > 127) {
            fineTune -= 256; 
        }
        this->fineTune = fineTune;
        
        int loopFlags = streamReadInt(is, 1, order);
        
        loopType = (SampleLoopType)(loopFlags & 0x0F);
        
        if ((loopFlags >> 4) & 1) {
            sampleRate = SampleRate::rate16hz;
        } else {
            sampleRate = SampleRate::rate8hz;
        }
        
        panning = streamReadInt(is, 1, order);
        
        int relativeNote = streamReadInt(is, 1, order);
        if (relativeNote > 127) {
            relativeNote -= 256; 
        }
        if (relativeNote > 95) {
            relativeNote = 95;
        }
        if (relativeNote < -96) {
            relativeNote = -96;
        }
        this->relativeNote = relativeNote;
        
        is.seekg(1, ios::cur);
        
        this->name = streamReadString(is, 22);
        
        int old = 0;
        
        for (int i = 0; i < dataSize; i++) {
            int val = streamReadInt(is, 1, order);
            if (val > 127) {
                val -= 256;
            }
            data.push_back(val + old);
            old += val;
        }
    }
}
