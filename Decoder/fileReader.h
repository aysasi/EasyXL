#ifndef file_reader_h
#define file_reader_h

#include <stdio.h>
#include <stdint.h>
#include <fstream>

typedef unsigned char ubyte;

struct FileReader{
    
    std::ifstream file;
    
    void link_file(const char* filename){
        file.open(filename, std::ios::binary | std::ios::in);
    }
    
    bool file_open(){
        return file.is_open();
    }
    
    void readUByte(ubyte * byte_char, size_t size) {
        file.read((char*)byte_char, size);
    }
    
    struct BitBuffer {
        uint32_t data    = 0;
        uint8_t numBits = 0;
    } buffer;
    
    void clean() {
        buffer.data = 0;
        buffer.numBits = 0;
    }
    
    int read_bits(int num_bits) {
        while (num_bits > buffer.numBits) {
            ubyte frame;
            file.read((char*)&frame, sizeof(char));
            buffer.data = buffer.data << 8;
            buffer.data = buffer.data | (uint8_t)frame;
            buffer.numBits += 8;
            
            if ((char)frame == '\xff')
                file.read((char*)&frame, sizeof(frame));
        }
        
        int aux = (1 << num_bits) - 1;
        aux = aux << (buffer.numBits - num_bits);
        aux = aux & buffer.data;
        aux = aux >> (buffer.numBits - num_bits);
        
        buffer.numBits -= num_bits;
        
        return aux;
    }
};

#endif
