#ifndef ans_h
#define ans_h


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


struct DANS_t{
    int simbolo;
    int prev_state;
};
typedef struct DANS_t DANS;


struct OneBit_t {
    unsigned int bit : 1;
};
typedef struct OneBit_t OneBit;


struct bitStream2_t {
    int max_size;
    int size;
    OneBit * list;
};
typedef struct bitStream2_t bitStream2;


DANS D_rANS(int cumuls[], int M, int fq[], int state, int * c_fun);
void decode(int M, int & state, int fq[], int cumuls[], vector<int> & bits, int num_symbols, int * c_fun, int16_t * vuelta);

int C_rANS(int code, int previous_state, int cumuls[], int M, int fq[]);
void encode(int16_t code[], int fq[], int cumuls[], int M, int & final_State, int num_symbols, vector<short int> &  bitstream);
void encodeElem(int16_t elem, int fq[], int cumuls[], int M, int & final_State, vector<short int> &  bitstream);

void encodeElem2(int16_t elem, int fq[], int * cumuls, int M, int & final_State, bitStream2 &  bitstream);


void encode2(int16_t code[], int fq[], int cumuls[], int M, int & final_State, int num_symbols, bitStream2  &  bitstream);


void add_bit(bitStream2 & stream, OneBit bit);

#endif
