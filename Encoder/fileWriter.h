#ifndef file_writer_h
#define file_writer_h

#include <fstream>


using namespace std;


struct BitCode {
    BitCode() = default; // undefined state, must be initialized at a later time
    BitCode(uint32_t code_, uint8_t numBits_)
    : code(code_), numBits(numBits_) {}
    uint32_t code;       // JPEG's Huffman codes are limited to 16 bits
    uint8_t  numBits;    // number of valid bits
};


struct BitWriter {

    ofstream myFile;
    
    void link_file(const char* filename){
        myFile.open(filename, std::ios_base::out | std::ios_base::binary);
    }
    
    void terminate(){
        myFile.close();
    }
    
    void output(unsigned char byte) {
        myFile << byte;
    }
    
    
    struct BitBuffer {
        int32_t data    = 0;
        uint8_t numBits = 0;
    } buffer;
    

    BitWriter& operator<<(const BitCode& data) {
        buffer.numBits += data.numBits;
        buffer.data   <<= data.numBits;
        buffer.data    |= data.code;
        
        while (buffer.numBits >= 8) {
            buffer.numBits -= 8;
            auto oneByte = uint8_t(buffer.data >> buffer.numBits);
            output(oneByte);
            
            if (oneByte == 0xFF)
                output(0);
        }
        
        return *this;
    }
    

    void flush() {
        *this << BitCode(0x7F, 7);
    }
    
    
    void clean() {
        buffer.numBits = 0;
        buffer.data = 0;
    }
    
    
    BitWriter& operator<<(uint8_t oneByte) {
        output(oneByte);
        return *this;
    }
    
    // write an array of bytes
    template <typename T, int Size>
    BitWriter& operator<<(T (&manyBytes)[Size]) {
        for (auto c : manyBytes)
            output(c);
        return *this;
    }
    
    // start a new JFIF block
    void addMarker(uint8_t id, uint16_t length) {
        output(0xFF); output(id);     // ID, always preceded by 0xFF
        output(uint8_t(length >> 8)); // length of the block (big-endian, includes the 2 length bytes as well)
        output(uint8_t(length & 0xFF));
    }
};


#endif /* fileWriter_h */
