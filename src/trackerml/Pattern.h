#ifndef PATTERN_H
#define PATTERN_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {

    class PatternData {
      public:
        bool hasNote;
        bool hasInstrument;
        bool hasVolume;
        bool hasEffect;
        bool hasEffectValue;
        unsigned char note;
        unsigned char instrument;
        unsigned char volume;
        unsigned char effect;
        unsigned char effectValue;
        
        std::string toString();
    };

    class Pattern {
      private:
        std::vector<std::vector<boost::shared_ptr<PatternData>>> channels;

      public:
        std::vector<std::vector<boost::shared_ptr<PatternData>>> & getChannels();
    };
}

#endif
