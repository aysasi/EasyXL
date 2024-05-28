#ifndef ans_h
#define ans_h

#include <stdint.h>
#include "fileReader.h"

struct DANS_t{
    int simbolo;
    int prev_state;
};
typedef DANS_t DANS;


DANS D_rANS(int cumuls[], int M, int fq[], int state, int * c_fun);


void decode_elems(int M, int & state_f, int fq[], int cumuls[], FileReader & file_reader, int num_symbols, int * c_fun, double * block);


void decode_num_elems(int M, int & state_f, int fq[], int cumuls[], FileReader & file_reader, int num_symbols, int * c_fun, int * numero_elementos);

#endif
