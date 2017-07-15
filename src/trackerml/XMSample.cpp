
#include "XMSample.h"
#include "stream.h"
#include <iostream>
#include <string>
#include <vector>

namespace trackerml {

    using namespace std;

    XMSample::XMSample()
        : loopStart(0), loopLength(0), volume(0x40), fineTune(0),
          loopType(SampleLoopType::noLoop), sampleRate(SampleRate::rate8hz),
          panning(0x80), relativeNote(0), name("") {}

    int XMSample::getLoopStart() { return loopStart; }
    int XMSample::getLoopLength() { return loopLength; }
    int XMSample::getVolume() { return volume; }
    int XMSample::getFineTune() { return fineTune; }
    SampleLoopType XMSample::getLoopType() { return loopType; }
    SampleRate XMSample::getSampleRate() { return sampleRate; }
    int XMSample::getPanning() { return panning; }
    int XMSample::getRelativeNote() { return relativeNote; }
    string XMSample::getName() { return name; }
    vector<int> XMSample::getData() { return data; }

    void XMSample::setLoopStart(int start) { loopStart = start; }
    void XMSample::setLoopLength(int length) { loopLength = length; }
    void XMSample::setVolume(int volume) { this->volume = volume; }
    void XMSample::setFineTune(int finetune) { this->fineTune = fineTune; }
    void XMSample::setLoopType(SampleLoopType type) { loopType = type; }
    void XMSample::setSampleRate(SampleRate rate) { sampleRate = rate; }
    void XMSample::setPanning(int panning) { this->panning = panning; }
    void XMSample::setRelativeNote(int relativeNote) {
        this->relativeNote = relativeNote;
    }
    void XMSample::setName(string name) { this->name = name; }
    void XMSample::setData(vector<int> data) { this->data = data; }

    void XMSample::write(ostream &os) {
        ByteOrder byteOrder = ByteOrder::littleEndian;

        if (SampleRate::rate8hz == sampleRate && data.size() < 3) {
            streamWriteInt(os, 3, 4, byteOrder);
        } else if (SampleRate::rate16hz == sampleRate && data.size() < 6) {
            streamWriteInt(os, 6, 4, byteOrder);
        } else {
            streamWriteInt(os, data.size(), 4, byteOrder);
        }

        streamWriteInt(os, loopStart, 4, byteOrder);
        streamWriteInt(os, loopLength, 4, byteOrder);
        streamWriteInt(os, volume, 1, byteOrder);

        if (fineTune < 0) {
            streamWriteInt(os, fineTune + 256, 1, byteOrder);
        } else {
            streamWriteInt(os, fineTune, 1, byteOrder);
        }

        streamWriteInt(os, ((int)sampleRate << 4) + (int)loopType, 1,
                       byteOrder);
        streamWriteInt(os, panning, 1, byteOrder);

        if (relativeNote < 0) {
            streamWriteInt(os, relativeNote + 256, 1, byteOrder);
        } else {
            streamWriteInt(os, relativeNote, 1, byteOrder);
        }

        streamWriteInt(os, 0, 1, byteOrder);

        int old = 0;

        for (auto sample : data) {
            if (sample - old < 0) {
                streamWriteInt(os, sample - old + 256, 1, byteOrder);
            } else {
                streamWriteInt(os, sample - old, 1, byteOrder);
            }
            old = sample;
        }

        int fillCount = 0;

        if (SampleRate::rate8hz == sampleRate && data.size() < 3) {
            // Minimum 3 samples
            fillCount = 3 - data.size();
        } else if (SampleRate::rate16hz == sampleRate && data.size() < 6) {
            // Minimum 6 samples
            fillCount = 6 - data.size();
        }

        for (int i = 0; i < fillCount; i++) {
            streamWriteInt(os, 0, 1, byteOrder);
        }
    }

    void XMSample::read(istream &is) {
        ByteOrder byteOrder = ByteOrder::littleEndian;

        int dataSize = streamReadInt(is, 4, byteOrder);

        loopStart = streamReadInt(is, 4, byteOrder);
        loopLength = streamReadInt(is, 4, byteOrder);
        volume = streamReadInt(is, 1, byteOrder);

        int fineTune = streamReadInt(is, 1, byteOrder);
        if (fineTune > 127) {
            fineTune -= 256;
        }
        this->fineTune = fineTune;

        int loopFlags = streamReadInt(is, 1, byteOrder);

        loopType = (SampleLoopType)(loopFlags & 0x0F);

        if ((loopFlags >> 4) & 1) {
            sampleRate = SampleRate::rate16hz;
        } else {
            sampleRate = SampleRate::rate8hz;
        }

        panning = streamReadInt(is, 1, byteOrder);

        int relativeNote = streamReadInt(is, 1, byteOrder);
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
            int val = streamReadInt(is, 1, byteOrder);
            if (val > 127) {
                val -= 256;
            }
            data.push_back(val + old);
            old += val;
        }
    }
}
