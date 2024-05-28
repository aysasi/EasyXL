#ifndef image_encoder_h
#define image_encoder_h

#include <iostream>

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using  int16_t =          short;
using  int32_t =          int;


bool writeXLImage(const void* pixels, unsigned short width, unsigned short height, int blockSize, bool xybcolorspace, int quality, char * filename);


#endif
