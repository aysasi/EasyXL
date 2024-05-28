#ifndef block2x2_h
#define block2x2_h


int ZigZag2x2[2*2] = {
    0, 2,
    1, 3
};


const double DCTValues2x2[2][2] = {
    {0.5,   0.7071067811865476},    //k = 0
    {0.5,   -0.5}                   //k = 1
};


#endif /* block2x2_h */
