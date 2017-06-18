#include <iostream>
#include <sndfile.h>
using namespace std;

/**
 * Convert an audio file to mono format (input in first arg, output in second)
 */
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << "usage: " << argv[0] << " input.wav output.wav" << endl;
        return 1;
    }
    // read whole file
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    sndfile = sf_open(argv[1], SFM_READ, &sfinfo);
    if (sndfile == 0)
    {
        cerr << "could not open audio file " << argv[1] << endl;
        exit(1);
    }
    cout << "input" << endl;
    cout << " frames: " << sfinfo.frames << endl;
    cout << " samplerate: " << sfinfo.samplerate << endl;
    cout << " channels: " << sfinfo.channels << endl;
    cout << " format: " << hex << sfinfo.format << dec << endl;
    float *audioIn = new float[sfinfo.channels * sfinfo.frames];
    sf_read_float(sndfile, audioIn, sfinfo.channels * sfinfo.frames);
    // mixdown
    float *audioOut = new float[sfinfo.frames];
    for(int i = 0; i < sfinfo.frames; i++)
    {
        audioOut[i] = 0;
        for(int j = 0; j < sfinfo.channels; j++)
            audioOut[i] += audioIn[i*sfinfo.channels + j];
        audioOut[i] /= sfinfo.channels;
    }
    sf_close(sndfile);
    // write output
    int frames = sfinfo.frames;    
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfinfo.channels = 1;
    sndfile = sf_open(argv[2], SFM_WRITE, &sfinfo);
    sf_write_float(sndfile, audioOut, frames);
    cout << "output" << endl;
    cout << " frames: " << frames << endl;
    cout << " samplerate: " << sfinfo.samplerate << endl;
    cout << " channels: " << sfinfo.channels << endl;
    cout << " format: " << hex << sfinfo.format << dec << endl;
    sf_close(sndfile);
    // free memory
    delete[] audioIn;
    delete[] audioOut;
    return 0;
}


