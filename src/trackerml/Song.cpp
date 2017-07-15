#include "Instrument.h"
#include "Pattern.h"
#include "Song.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {
    std::string Song::getName() {
        return this->name;
    }
    int Song::getChannelsCount() {
        return this->channelsCount;
    }
    int Song::getRestartPosition() {
        return this->restartPosition;
    }
    int Song::getSongLength() {
        return this->songLength;
    }
    int Song::getTempo() {
        return this->tempo;
    }
    int Song::getBpm() {
        return this->bpm;
    }
    std::vector<int> & Song::getPatternsTable() {
        return this->patternsTable;
    }
    std::vector<boost::shared_ptr<Instrument>> & Song::getInstruments() {
        return this->instruments;
    }
    std::vector<boost::shared_ptr<Pattern>> & Song::getPatterns() {
        return this->patterns;
    }
    void Song::setName(std::string name) {
        this->name = name;
    }
    void Song::setSongLength(int length) {
        this->songLength = length;
    }
    void Song::setRestartPosition(int pos) {
        this->restartPosition = pos;
    }
    void Song::setChannelsCount(int count) {
        this->channelsCount = count;
    }
    void Song::setTempo(int tempo) {
        this->tempo = tempo;
    }
    void Song::setBpm(int bpm) {
        this->bpm = bpm;
    }
}
