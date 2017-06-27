
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <string>

#include "XMFile.h"
#include "XMInstrument.h"
#include "XMPattern.h"
#include "stream.h"

namespace trackerml {

    using namespace std;

    XMFile::XMFile() {
        version = 0x104;
        setFrequencyTableType(FrequencyTableType::linear);
    }
    
    string XMFile::getSongName() {
        return songName;
    }
    
    uint16_t XMFile::getRestartPosition() {
        return restartPosition;
    }
    
    uint16_t XMFile::getChannelsCount() {
        return channelsCount;
    }
    
    uint16_t XMFile::getSongLength() {
        return patternsTable.size();
    }
    
    uint16_t XMFile::getTempo() {
        return tempo;
    }
    
    uint16_t XMFile::getBpm() {
        return bpm;
    }
    
    vector<uint8_t> XMFile::getPatternsTable( ) {
        return patternsTable;
    }
    
    FrequencyTableType XMFile::getFrequencyTableType() {
        return frequencyTableType;
    }
    
    vector<XMInstrument> XMFile::getInstruments() {
        return instruments;
    }
    
    vector<XMPattern> XMFile::getPatterns() {
        return patterns;
    }

    void XMFile::setSongName(string songName) {
        this->songName = songName;
    }

    void XMFile::setRestartPosition(uint16_t pos) { 
        restartPosition = pos; 
    }

    void XMFile::setChannelsCount(uint16_t count) {
        channelsCount = count;
    }

    void XMFile::setTempo(uint16_t tempo) { 
        this->tempo = tempo; 
    }

    void XMFile::setBpm(uint16_t bpm) { 
        this->bpm = bpm; 
    }
    
    void XMFile::setFrequencyTableType(FrequencyTableType type) {
        frequencyTableType = type;
    }

    void XMFile::addInstrument(XMInstrument instrument) {
        instruments.push_back(instrument);
    }

    void XMFile::write(ostream &os) {
        writeHeader(os);
        
        for (auto instrument : instruments) {
            instrument.write(os);
        }
    }
    
    void XMFile::read(istream &is) {
        readHeader(is);
    }
    
    void XMFile::writeHeader(ostream &os) {
        ByteOrder order = ByteOrder::littleEndian;
        
        streamWriteString(os, "Extended Module: ", 17);
        streamWriteString(os, songName, 20);
        streamWriteInt(os, 0x1a, 1, order); // Padding
        streamWriteString(os, "TrackerML", 20);
        streamWriteInt(os, version, 2, order); // Version
        streamWriteInt(os, 276, 4, order); // Header size
        streamWriteInt(os, max(1, (const int)patternsTable.size()), 2, order); // Song length
        streamWriteInt(os, restartPosition, 2, order);
        streamWriteInt(os, channelsCount, 2, order);
        streamWriteInt(os, (const int)patterns.size(), 2, order);
        streamWriteInt(os, max(1, (const int)instruments.size()), 2, order);
        streamWriteInt(os, (int)frequencyTableType, 2, order);
        streamWriteInt(os, tempo, 2, order);
        streamWriteInt(os, bpm, 2, order);
        
        unsigned int i;
        for (i = 0; i < patternsTable.size() && i < 256; i++) {
            streamWriteInt(os, patternsTable[i], 1, order);
        }
        
        // Pad patterns table with 0's up to 256 bytes
        for (; i < 256; i++) {
            streamWriteInt(os, 0, 1, order);
        }
    }
    
    void XMFile::readHeader(istream &is) {
        ByteOrder order = ByteOrder::littleEndian;
        
        is.seekg(0x11);
        this->songName = streamReadString(is, 20);
        
        is.seekg(0x3a);
        version = streamReadInt(is, 2, order);
        
        is.seekg(0x40);
        
        int songLength = streamReadInt(is, 2, order);
        
        restartPosition = streamReadInt(is, 2, order);
        channelsCount = streamReadInt(is, 2, order);
        
        int patternsCount = streamReadInt(is, 2, order);
        int instrumentsCount = streamReadInt(is, 2, order);
        
        frequencyTableType = (FrequencyTableType)streamReadInt(is, 2, order);
        tempo = streamReadInt(is, 2, order);
        bpm = streamReadInt(is, 2, order);
        
        for (int i = 0; i < songLength; i++) {
            patternsTable.push_back(streamReadInt(is, 1, order));
        }
    }
}
