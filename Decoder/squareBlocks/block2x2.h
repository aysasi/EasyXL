//
//  block2x2.h
//  JPEGDecoder
//
//  Created by Alejandro Ysasi Cillero on 25/4/24.
//

#ifndef block2x2_h
#define block2x2_h

int ZigZag2x2[2*2] = {
    0, 2,
    1, 3
};

const double IDCTValues2x2[2][2] = {
    {1, 1}, // k = 0
    {1, -1.0} // k = 1
};

#endif /* block2x2_h */
