#ifndef jpeg_h
#define jpeg_h


struct Image{
    uint16_t width;
    uint16_t height;
    uint16_t bitsPerChannel;
    uint8_t numberChannels;
    uint8_t* matriz;
};


#endif /* jpeg_h */
