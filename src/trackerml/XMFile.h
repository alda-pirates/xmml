
#ifndef XMFILE_H
#define XMFILE_H

#include <iostream>
#include <string>
#include <cstdint>

namespace trackerml {
    
    using namespace std;
    
    struct XMHeader {
        int8_t   id[17];                   // (char)  ID text: 'Extended Module: '
        int8_t   songname[20];             // (char)  Module name, padded with zeroes
        int8_t   padding_0x1a;             // (char)  Character 0x1a is required here
        int8_t   trackername[20];          // (char)  Tracker name
        uint16_t version;                  // (word)  Version number, hi-byte major and low-byte minor, current version is 0x104
        uint32_t size;                     // (dword) Header size (calculated from this offset)
        uint16_t songlength;               // (word)  Song length (in patten order table)
        uint16_t restart;                  // (word)  Restart position
        uint16_t numchannels;              // (word)  Number of channels (2,4,6,8,10,...,32)
        uint16_t numpatterns;              // (word)  Number of patterns (max 256)
        uint16_t numinstruments;           // (word)  Number of instruments (max 128)
        uint16_t flags;                    // (word)  Flags: bit 0: 0 = Amiga frequency table (see below) 1 = Linear frequency table
        uint16_t tempo;                    // (word)  Default tempo (speed in MilkyTracker)
        uint16_t bpm;                      // (word)  Default BPM
        uint8_t  orders[256];              // (byte)  Pattern order table 
    };

    
    struct XMPatternHeader {

        uint32_t size;         // (dword) Pattern header size
        uint8_t  packtype;     // (byte) Packing type (always 0)
        uint16_t numrows;      // (word) Number of rows in pattern (1..256)
        uint16_t datasize;     // (word) Packed patterndata size. This is zero if the pattern is completely empty and no pattern data follows!
     
    };
    
    /*
     A simle packing scheme is adopted, so that the patterns do not become TOO
large: Since the MSB in the note value is never used, it is used for the
compression. If the bit is set, then the other bits are interpreted as follows:

    bit 0 set (00000001): Note byte follows
        1 set (00000010): Instrument byte follows
        2 set (00000100): Volume column byte follows
        3 set (00001000): Effect byte follows
        4 set (00010000): Effect data byte follows
    
    For example, if the note and the volume are set, but not the instrument neather the effects
    then first byte will be 0x80 + 0x01 + 0x04 = 0x85, where 0x80 indicate the packing scheme is used,
    0x01 indicate the note is present and the 0x04 indicate the volume is present. Then the next byte will be
    the value for the note and the second one will be the volume. 
    */
    struct XMPatternData {
        uint8_t note;               // (byte) Note (0-71, 0 = C-0). Actually note numbers are 1-96, 97 being the key-off note
        uint8_t instrument;         // (byte) Instrument (0-128). More like 1-128.
        uint8_t volume;             // (byte) Volume column byte (see below)
        uint8_t effect;             // (byte) Effect type
        uint8_t effectparam;        // (byte) Effect parameter
    };
    
    struct XMPattern {
        XMPatternHeader header;
        XMPatternData *data;
    };
    
    struct XMInstrumentHeader {
        uint32_t size; // (dword) Instrument header size
        int8_t name[22]; // (char) Instrument name
        uint8_t type; // (byte) Instrument type (always 0). This seems pretty random, don't assume it's zero.
        uint16_t numsamples; // (word) Number of samples in instrument
    };
    
    // If the number of samples > 0, then the this will follow:
    struct XMInstrumentHeaderExtended {
        uint32_t size; // (dword) Sample header size
        uint8_t samplenote[96]; // (byte) Sample number for all notes
        uint8_t volumeenvelopepoints[48]; // (byte) Points for volume envelope
        uint8_t panningenvelopepoints[48]; // (byte) Points for panning envelope
        uint8_t numvolumepoints; // (byte) Number of volume points
        uint8_t numpanningpoints; // (byte) Number of panning points
        uint8_t volumesustain; // (byte) Volume sustain point
        uint8_t volumeloopstart; // (byte) Volume loop start point
        uint8_t volumeloopend; // (byte) Volume loop end point
        uint8_t panningsustain; // (byte) Panning sustain point
        uint8_t panningloopstart; // (byte) Panning loop start point
        uint8_t panningloopend; // (byte) Panning loop end point
        uint8_t volumetype; // (byte) Volume type: bit 0: On; 1: Sustain; 2: Loop
        uint8_t panningtype; // (byte) Panning type: bit 0: On; 1: Sustain; 2: Loop
        uint8_t vibratotype; // (byte) Vibrato type
        uint8_t vibratosweep; // (byte) Vibrato sweep
        uint8_t vibratodepth; // (byte) Vibrato depth
        uint8_t vibratorate; // (byte) Vibrato rate
        uint16_t volumefadeout; // (word) Volume fadeout
        uint16_t reserved; // (word) Reserved
    };
    
    struct XMSampleHeader {
        uint32_t samplesize;// (dword) Sample length
        uint32_t sampleloopstart;// (dword) Sample loop start
        uint32_t samplelooplength;// (dword) Sample loop length
        uint8_t volume;// (byte) Volume
        int8_t finetune;// (byte) Finetune (signed byte -16..+15)
        uint8_t type;// (byte) Type: Bit 0-1: 0 = No loop, 1 = Forward loop, 2 = Ping-pong loop; 4: 16-bit sampledata
        uint8_t panning;// (byte) Panning (0-255)
        uint8_t relativenote;// (byte) Relative note number (signed byte)
        uint8_t reserved;// (byte) Reserved
        int8_t samplename[22];// (char) Sample name
    };
    
    struct XMSample {
        XMSampleHeader header;
        uint8_t* data; // (byte) The samples are stored as delta values. To convert to real data:
        /*
        old=0;
        for i=1 to len
            new=sample[i]+old;
            sample[i]=new;
            old=new;
            **/
    };
    
    struct XMInstrument {
        XMInstrumentHeader header;
        XMInstrumentHeaderExtended ext;
        XMSample* samples;
    };

    class XMFile {
        protected:
        
            XMHeader* header;
        
        
        public:
        
            XMFile();
        
		    void setSongName(string songname);
		    
		    void setSongLength(uint16_t length);
		    
		    void setRestartPosition(uint16_t pos);
		    
		    void setChannelsCount(uint16_t count);
		    
		    void setPatternsCount(uint16_t count);
		    
		    void setInstrumentsCount(uint16_t count);
		    
		    void setTempo(uint16_t tempo);
		    
		    void setBpm(uint16_t bpm);
		    
		    void setPatterns(uint8_t orders[256]);
		    
		    void write(ostream& os);
    };
    
}

#endif /* XMFILE_H */
