
#ifndef XMSAMPLE_H
#define XMSAMPLE_H
#include <iostream>
#include <string>
#include <vector>

namespace trackerml {

    using namespace std;

    enum SampleLoopType { noLoop, forward, pingPong };

    enum class SampleRate { rate8hz, rate16hz };

    class XMSample {
      private:
        int loopStart;
        int loopLength;
        int volume;
        int fineTune;
        SampleLoopType loopType;
        SampleRate sampleRate;
        int panning;
        int relativeNote;
        string name;

        vector<int> data;

      public:
        XMSample();
        int getLoopStart();
        int getLoopLength();
        int getVolume();
        int getFineTune();
        SampleLoopType getLoopType();
        SampleRate getSampleRate();
        int getPanning();
        int getRelativeNote();
        string getName();
        vector<int> getData();
        void setLoopStart(int start);
        void setLoopLength(int length);
        void setVolume(int volume);
        void setFineTune(int finetune);
        void setLoopType(SampleLoopType type);
        void setSampleRate(SampleRate rate);
        void setPanning(int panning);
        void setRelativeNote(int relativenote);
        void setName(string name);
        void setData(vector<int> data);
        void write(ostream &os);
        void read(istream &is);
    };
}

#endif
