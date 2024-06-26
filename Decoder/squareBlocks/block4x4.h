#ifndef block4x4_h
#define block4x4_h

int ZigZag4x4[4*4] = {
    0,  4,  1,  2,
    5,  8, 12,  9,
    6,  3,  7, 10,
    13, 14, 11, 15
};

const double IDCTValues4x4[4][4] = {
    {1.0,   1.3065629648763766,     1.0000000000000002,     0.5411961001461971},    //k=0
    {1.0,   0.5411961001461971,     -1.0,                   -1.3065629648763768},   //k=1
    {1.0,   -0.541196100146197,     -1.0000000000000002,    1.3065629648763764},    //k=2
    {1.0,   -1.3065629648763766,    0.9999999999999998,     -0.5411961001461972},   //k=3

};

#endif /* block4x4_h */
