
#include <iostream>
#include <string>
#include <stdint.h>

namespace trackerml {
    
    using namespace std;

    class XMFile {
        protected:
        
        struct XMHeader* header;
        
        
        public:
        
        XMFile() {
            int i;
            string id = "Extended Module: ";
            
            this->header = new XMHeader;
            
            strncpy(this->header->id, "Extended Module: ", 17);
            
            this->header->padding_0x1a = 0x1a;
            
            strncpy(this->header->id, "TrackerML", strlen("TrackerML"));
            
            this->header->version = 0x104;
            this->header->size = 276;
            this->header->flag = 1; // Linear freq. table
        }
        
        void setSongName(string songname) {
        	strncpy(header->songname, songname, sizeof(header->songname));
        }
        
        void setSongLength(uint16_t length) {
        	header->songlength = length;
        }
        
        void setRestartPosition(uint16_t pos) {
        	header->restart = pos;
        }
        
        void setChannelsCount(uint16_t count) {
        	header->numchannels = count;
        }
        
        void setPatternsCount(uint16_t count) {
        	header->numpatterns = count;
        }
        
        void setInstrumentsCount(uint16_t count) {
        	header->numinstruments = count;
        }
        
        void setTempo(uint16_t tempo) {
        	header->tempo = tempo;
        }
        
        void setBpm(uint16_t bpm) {
        	header->bpm = bpm;
        }
        
        void setPatterns(uint8_t orders[256]) {
        	str::copy(orders, orders + 256, header->orders);
        }
        
        void write(ostream& os) {
        	os.write((char *)header->id, sizeof(header->id));
        	os.write(header->songname, sizeof(header->songname));
        	os.write((char *)header->padding_0x1a, sizeof(header->padding_0x1a));
        	os.write(header->trackername, sizeof(header->trackername));
        	os.write((char *)header->version, sizeof(header->version));
        	os.write((char *)header->size, sizeof(header->size));
        	os.write((char *)header->songlength, sizeof(header->songlength));
        	os.write((char *)header->restart, sizeof(header->restart));
        	os.write((char *)header->numchannels, sizeof(header->numchannels));
        	os.write((char *)header->numpatterns, sizeof(header->numpatterns));
        	os.write((char *)header->numinstruments, sizeof(header->numinstruments));
        	os.write((char *)header->flags, sizeof(header->flags));
        	os.write((char *)header->tempo, sizeof(header->tempo));
        	os.write((char *)header->bpm, sizeof(header->bpm));
        	os.write(header->orders, sizeof(header->orders));
        }
    };
    
}
