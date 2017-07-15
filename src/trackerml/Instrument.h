#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "Sample.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {
    class Instrument {
      private:
        std::string name;
        std::vector<boost::shared_ptr<Sample>> samples;

      public:
        std::string getName();
        void setName(std::string name);
        std::vector<boost::shared_ptr<Sample>> & getSamples();
    };
}

#endif
