
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstring>

#include "XMFile.h"

namespace trackerml {
	
	using namespace std;
        
    XMFile::XMFile() {
        strncpy((char *) header.id, "Extended Module: ", 17);
        
        header.padding_0x1a = 0x1a;
        
        strncpy((char *) header.trackername, "TrackerML", strlen("TrackerML"));
        
        header.version = 0x104;
        header.size = 276;
        header.flags = 1; // Linear freq. table
    }
    
    void XMFile::setSongName(string songname) {
    	strncpy((char *) header.songname, songname.c_str(), sizeof(header.songname));
    }
    
    void XMFile::setSongLength(uint16_t length) {
    	header.songlength = length;
    }
    
    void XMFile::setRestartPosition(uint16_t pos) {
    	header.restart = pos;
    }
    
    void XMFile::setChannelsCount(uint16_t count) {
    	header.numchannels = count;
    }
    
    void XMFile::setPatternsCount(uint16_t count) {
    	header.numpatterns = count;
    }
    
    void XMFile::setInstrumentsCount(uint16_t count) {
    	header.numinstruments = count;
    }
    
    void XMFile::setTempo(uint16_t tempo) {
    	header.tempo = tempo;
    }
    
    void XMFile::setBpm(uint16_t bpm) {
    	header.bpm = bpm;
    }
    
    void XMFile::setPatterns(uint8_t orders[256]) {
    	strncpy((char *) header.orders, (char *) orders, 256);
    }
    
    void XMFile::write(ostream & os) {
    	// Write header
    	os.write((char *)   header.id, sizeof(header.id));
    	os.write((char *)   header.songname, sizeof(header.songname));
    	os.write((char *) & header.padding_0x1a, sizeof(header.padding_0x1a));
    	os.write((char *)   header.trackername, sizeof(header.trackername));
    	os.write((char *) & header.version, sizeof(header.version));
    	os.write((char *) & header.size, sizeof(header.size));
    	os.write((char *) & header.songlength, sizeof(header.songlength));
    	os.write((char *) & header.restart, sizeof(header.restart));
    	os.write((char *) & header.numchannels, sizeof(header.numchannels));
    	os.write((char *) & header.numpatterns, sizeof(header.numpatterns));
    	os.write((char *) & header.numinstruments, sizeof(header.numinstruments));
    	os.write((char *) & header.flags, sizeof(header.flags));
    	os.write((char *) & header.tempo, sizeof(header.tempo));
    	os.write((char *) & header.bpm, sizeof(header.bpm));
    	os.write((char *)   header.orders, sizeof(header.orders));
    }
}
