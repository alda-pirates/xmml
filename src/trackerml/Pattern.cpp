#include "Pattern.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace trackerml {

    std::string PatternData::toString() { return ""; }

    std::vector<std::vector<boost::shared_ptr<PatternData>>> & Pattern::getChannels() {
        return this->channels;
    }
}
