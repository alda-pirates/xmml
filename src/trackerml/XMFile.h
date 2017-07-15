
#ifndef XMFILE_H
#define XMFILE_H

#include "XMInstrument.h"
#include "XMPattern.h"
#include <iostream>
#include <string>
#include <vector>

namespace trackerml {

    using namespace std;

    enum struct FrequencyTableType { amiga, linear };

    class XMFile {
      private:
        string songName;
        int channelsCount;
        int restartPosition;
        int tempo;
        int bpm;
        vector<int> patternsTable;
        FrequencyTableType frequencyTableType;
        int version;

        vector<XMInstrument> instruments;
        vector<XMPattern> patterns;

        void writeHeader(ostream &os);
        pair<int, int> readHeader(istream &is);

      public:
        XMFile();

        string getSongName();
        int getRestartPosition();
        int getChannelsCount();
        int getSongLength();
        int getTempo();
        int getBpm();
        vector<int> getPatternsTable();
        FrequencyTableType getFrequencyTableType();
        vector<XMInstrument> getInstruments();
        vector<XMPattern> getPatterns();

        void setSongName(string songname);
        void setRestartPosition(int pos);
        void setChannelsCount(int count);
        void setTempo(int tempo);
        void setBpm(int bpm);
        void setPatternsTable(vector<int> table);
        void setFrequencyTableType(FrequencyTableType type);

        void addInstrument(XMInstrument instrument);
        void setInstrument(int n, XMInstrument instrument);
        void removeInstrument(XMInstrument instrument);
        int instrumentsCount();

        void addPattern(XMPattern pattern);
        void setPattern(int n, XMPattern pattern);
        void removePattern(XMPattern pattern);
        int patternsCount();

        void write(ostream &os);
        void read(istream &is);
    };
}

#endif /* XMFILE_H */
