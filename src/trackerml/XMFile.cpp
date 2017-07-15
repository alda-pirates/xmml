
#include "XMFile.h"
#include "XMInstrument.h"
#include "XMPattern.h"
#include "stream.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace trackerml {

    using namespace std;

    XMFile::XMFile() {
        this->version = 0x104;
        this->setFrequencyTableType(FrequencyTableType::linear);
    }

    string XMFile::getSongName() { return this->songName; }

    int XMFile::getRestartPosition() { return this->restartPosition; }

    int XMFile::getChannelsCount() { return this->channelsCount; }

    int XMFile::getSongLength() { return this->patternsTable.size(); }

    int XMFile::getTempo() { return this->tempo; }

    int XMFile::getBpm() { return this->bpm; }

    vector<int> XMFile::getPatternsTable() { return this->patternsTable; }

    FrequencyTableType XMFile::getFrequencyTableType() {
        return this->frequencyTableType;
    }

    vector<XMInstrument> XMFile::getInstruments() { return this->instruments; }

    vector<XMPattern> XMFile::getPatterns() { return this->patterns; }

    void XMFile::setSongName(string songName) { this->songName = songName; }

    void XMFile::setRestartPosition(int pos) { this->restartPosition = pos; }

    void XMFile::setChannelsCount(int count) { this->channelsCount = count; }

    void XMFile::setTempo(int tempo) { this->tempo = tempo; }

    void XMFile::setBpm(int bpm) { this->bpm = bpm; }

    void XMFile::setFrequencyTableType(FrequencyTableType type) {
        this->frequencyTableType = type;
    }

    void XMFile::addInstrument(XMInstrument instrument) {
        this->instruments.push_back(instrument);
    }

    void XMFile::write(ostream &os) {
        this->writeHeader(os);

        /*
        for (auto instrument : instruments) {
            instrument.write(os);
        }
        */
    }

    void XMFile::read(istream &is) {
        pair<int, int> countsPair = this->readHeader(is);

        int patternsCount = get<0>(countsPair);
        int instrumentsCount = get<1>(countsPair);

        for (int i = 0; i < patternsCount; i++) {
            XMPattern pattern;
            pattern.setChannelsCount(this->getChannelsCount());
            pattern.read(is);
            this->patterns.push_back(pattern);
        }

        for (int i = 0; i < instrumentsCount; i++) {
            XMInstrument instrument;
            instrument.read(is);
            this->instruments.push_back(instrument);
        }
    }

    void XMFile::writeHeader(ostream &os) {
        ByteOrder byteOrder = ByteOrder::littleEndian;

        streamWriteString(os, "Extended Module: ", 17);
        streamWriteString(os, this->songName, 20);
        streamWriteInt(os, 0x1a, 1, byteOrder); // Padding
        streamWriteString(os, "TrackerML", 20);
        streamWriteInt(os, this->version, 2, byteOrder); // Version
        streamWriteInt(os, 276, 4, byteOrder);           // Header size
        streamWriteInt(os, max(1, (const int)this->patternsTable.size()), 2,
                       byteOrder); // Song length
        streamWriteInt(os, this->restartPosition, 2, byteOrder);
        streamWriteInt(os, this->channelsCount, 2, byteOrder);
        streamWriteInt(os, (const int)this->patterns.size(), 2, byteOrder);
        streamWriteInt(os, max(1, (const int)this->instruments.size()), 2,
                       byteOrder);
        streamWriteInt(os, (int)this->frequencyTableType, 2, byteOrder);
        streamWriteInt(os, this->tempo, 2, byteOrder);
        streamWriteInt(os, this->bpm, 2, byteOrder);

        unsigned int i;
        for (i = 0; i < this->patternsTable.size() && i < 256; i++) {
            streamWriteInt(os, this->patternsTable[i], 1, byteOrder);
        }

        // Pad patterns table with 0's up to 256 bytes
        for (; i < 256; i++) {
            streamWriteInt(os, 0, 1, byteOrder);
        }
    }

    pair<int, int> XMFile::readHeader(istream &is) {
        ByteOrder byteOrder = ByteOrder::littleEndian;

        is.seekg(0x11);
        this->songName = streamReadString(is, 20);

        is.seekg(0x3a);
        this->version = streamReadInt(is, 2, byteOrder);

        is.seekg(0x40);

        int songLength = streamReadInt(is, 2, byteOrder);

        this->restartPosition = streamReadInt(is, 2, byteOrder);
        this->channelsCount = streamReadInt(is, 2, byteOrder);

        int patternsCount = streamReadInt(is, 2, byteOrder);
        int instrumentsCount = streamReadInt(is, 2, byteOrder);

        this->frequencyTableType =
            (FrequencyTableType)streamReadInt(is, 2, byteOrder);
        this->tempo = streamReadInt(is, 2, byteOrder);
        this->bpm = streamReadInt(is, 2, byteOrder);

        for (int i = 0; i < songLength; i++) {
            this->patternsTable.push_back(streamReadInt(is, 1, byteOrder));
        }

        if (songLength < 256) {
            is.seekg(256 - songLength, ios::cur);
        }

        return make_pair(patternsCount, instrumentsCount);
    }
}
