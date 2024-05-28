#ifndef block4x4_h
#define block4x4_h


int ZigZag4x4[4*4] = {
    0,  4,  1,  2,
    5,  8, 12,  9,
    6,  3,  7, 10,
    13, 14, 11, 15
};


const double DCTValues4x4[4][4] = {
    {0.25,                  0.25,                   0.25,                   0.25},                      //k = 0
    {0.32664074121909414,   0.13529902503654928,    -0.13529902503654925,   -0.32664074121909414},      //k = 1
    {0.25,                  -0.25,                  -0.25,                  0.25},                      //k = 2
    {0.13529902503654928,   -0.3266407412190942,    0.3266407412190941,     -0.1352990250365493},       //k = 3
};


#endif /* block4x4_h */