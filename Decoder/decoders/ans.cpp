#include <stdint.h>
#include "ans.h"

#include <iostream>

DANS D_rANS(int cumuls[], int M, int fq[], int state, int * c_fun) {
    int slot = state % M;
    int st = c_fun[slot];
    int previous_state = (state / M) * fq[st] + slot - cumuls[st];
    return { st, previous_state};
}


void decode_elems(int M, int & state_f, int fq[], int cumuls[], FileReader & file_reader, int num_symbols, int * c_fun, double * block) {
    int state = state_f;
    
    for(int i = 0; i < num_symbols; i++) {
        DANS aux = D_rANS(cumuls, M, fq, state, c_fun);
        state = aux.prev_state;
        block[i] = (double)(aux.simbolo - 900);
        while (state < M) {
            uint8_t p = file_reader.read_bits(1);
            //std::cout << 1 << std::endl;
            state = (state << 1) | p; //state*2+p
        }
    }
    state_f = state;
}


void decode_num_elems(int M, int & state_f, int fq[], int cumuls[], FileReader & file_reader, int num_symbols, int * c_fun, int * numero_elementos) {
    int state = state_f;
    
    for(int i = 0; i < num_symbols; i++) {
        DANS aux = D_rANS(cumuls, M, fq, state, c_fun);
        state = aux.prev_state;
        numero_elementos[i] = aux.simbolo;
        while (state < M) {
            uint8_t p = file_reader.read_bits(1);
            state = (state << 1) | p;
        }
    }
    state_f = state;
}
