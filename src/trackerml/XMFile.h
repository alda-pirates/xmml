
#ifndef XMFILE_H
#define XMFILE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "XMInstrument.h"
#include "XMPattern.h"

namespace trackerml {

    using namespace std;
    
    enum struct FrequencyTableType { amiga, linear };

    class XMFile {
      private:
        string songName;
        uint16_t channelsCount;
        uint16_t restartPosition;
        uint16_t tempo;
        uint16_t bpm;
        vector<uint8_t> patternsTable;
        FrequencyTableType frequencyTableType;  
        uint16_t version;
        
        vector<XMInstrument> instruments;
        vector<XMPattern> patterns;
        
        void writeHeader(ostream &os);
        void readHeader(istream &is);

      public:
        XMFile();
        
        string getSongName();
        uint16_t getRestartPosition();
        uint16_t getChannelsCount();
        uint16_t getSongLength();
        uint16_t getTempo();
        uint16_t getBpm();
        vector<uint8_t> getPatternsTable( );
        FrequencyTableType getFrequencyTableType();
        vector<XMInstrument> getInstruments();
        vector<XMPattern> getPatterns();
        
        void setSongName(string songname);
        void setRestartPosition(uint16_t pos);
        void setChannelsCount(uint16_t count);
        void setTempo(uint16_t tempo);
        void setBpm(uint16_t bpm);
        void setPatternsTable(vector<uint8_t> table);
        void setFrequencyTableType(FrequencyTableType type);

        void addInstrument(XMInstrument instrument);
        void setInstrument(size_t n, XMInstrument instrument);
        void removeInstrument(XMInstrument instrument);
        size_t instrumentsCount();

        void addPattern(XMPattern pattern);
        void setPattern(size_t n, XMPattern pattern);
        void removePattern(XMPattern pattern);
        size_t patternsCount();

        void write(ostream &os);
        void read(istream &is);
    };
}

#endif /* XMFILE_H */
