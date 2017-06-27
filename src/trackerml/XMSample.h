
#ifndef XMSAMPLE_H
#define XMSAMPLE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace trackerml {

    using namespace std;
    
    enum SampleLoopType : uint8_t {
        noLoop,
        forward,
        pingPong
    };
    
    enum class SampleRate : uint8_t  {
        rate8hz,
        rate16hz
    };

    class XMSample {
      private:
        uint32_t loopStart;
        uint32_t loopLength;
        uint8_t volume;
        int8_t fineTune;
        SampleLoopType loopType;
        SampleRate sampleRate;
        uint8_t panning;
        int8_t relativeNote;
        string name;
        
        vector<uint8_t> data; 
        
        void writeHeader(ostream &os);
        void readHeader(istream &is);

      public:
        XMSample();
        uint32_t getLoopStart();
        uint32_t getLoopLength();
        uint8_t getVolume();
        int8_t getFineTune();
        SampleLoopType getLoopType();
        SampleRate getSampleRate();
        uint8_t getPanning();
        int8_t getRelativeNote();
        string getName();
        vector<uint8_t> getData();
        void setLoopStart(uint32_t start);
        void setLoopLength(uint32_t length);
        void setVolume(uint8_t volume);
        void setFineTune(int8_t finetune);
        void setLoopType(SampleLoopType type);
        void setSampleRate(SampleRate rate);
        void setPanning(uint8_t panning);
        void setRelativeNote(int8_t relativenote);
        void setName(string name);
        void setData(vector<uint8_t> data);
        void write(ostream &os);
        void read(istream &is);
    };
}

#endif
