#ifndef SONG_H
#define SONG_H

#include "Instrument.h"
#include "Pattern.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {

    class Song {
      private:
        std::string name;
        int channelsCount;
        int songLength;
        int restartPosition;
        int tempo;
        int bpm;
        std::vector<int> patternsTable;
        std::vector<boost::shared_ptr<Instrument>> instruments;
        std::vector<boost::shared_ptr<Pattern>> patterns;

      public:
        std::string getName();
        int getChannelsCount();
        int getSongLength();
        int getRestartPosition();
        int getTempo();
        int getBpm();
        std::vector<int> & getPatternsTable();
        std::vector<boost::shared_ptr<Instrument>> & getInstruments();
        std::vector<boost::shared_ptr<Pattern>> & getPatterns();

        void setName(std::string name);
        void setSongLength(int length);
        void setRestartPosition(int pos);
        void setChannelsCount(int count);
        void setTempo(int tempo);
        void setBpm(int bpm);
    };
}

#endif 
