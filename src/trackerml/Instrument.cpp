#include "Instrument.h"
#include "Sample.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {
    std::string Instrument::getName() {
        return this->name;
    }
    void Instrument::setName(std::string name) {
        this->name = name;
    }
    std::vector<boost::shared_ptr<Sample>> & Instrument::getSamples() {
        return this->samples;
    }
}
