
#ifndef block8x8_h
#define block8x8_h

int ZigZag8x8[8*8] = {
    0,      8,      1,      2,      9,      16,     24,     17,
    10,     3,      4,      11,     18,     25,     32,     40,
    33,     26,     19,     12,     5,      6,      13,     20,
    27,     34,     41,     48,     56,     49,     42,     35,
    28,     21,     14,     7,      15,     22,     29,     36,
    43,     50,     57,     58,     51,     44,     37,     30,
    23,     31,     38,     45,     52,     59,     60,     53,
    46,     39,     47,     54,     61,     62,     55,     63
};

const double IDCTValues8x8[8][8] = {
    {1.0, 1.3870398, 1.306563, 1.1758756, 1.0, 0.785695, 0.5411961, 0.2758994}, //k=0
    {1.0, 1.1758756, 0.5411961, -0.2758994, -1.0, -1.3870398, -1.306563, -0.785695}, //k=1
    {1.0, 0.785695, -0.5411961, -1.3870398, -1.0, 0.2758994, 1.306563, 1.1758756}, //k=2
    {1.0, 0.2758994, -1.306563, -0.785695, 1.0, 1.1758756, -0.5411961, -1.3870398}, //k=3
    {1.0, -0.2758994, -1.306563, 0.785695, 1.0, -1.1758756, -0.5411961, 1.3870398}, //k=4
    {1.0, -0.785695, -0.5411961, 1.3870398, -1.0, -0.2758994, 1.306563, -1.1758756}, //k=5
    {1.0, -1.1758756, 0.5411961, 0.2758994, -1.0, 1.3870398, -1.306563, 0.785695}, //k=6
    {1.0, -1.3870398, 1.306563, -1.1758756, 1.0, -0.785695, 0.5411961, -0.2758994}  //k=7
};


#endif
