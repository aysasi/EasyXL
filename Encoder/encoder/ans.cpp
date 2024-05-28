#include "ans.h"


DANS D_rANS(int cumuls[], int M, int fq[], int state, int * c_fun) {
    int slot = state % M;
    
    int st = c_fun[slot];
    int previous_state = (state / M) * fq[st] + slot - cumuls[st];
    return { st, previous_state};
}


void decode(int M, int & state_f, int fq[], int cumuls[], vector<int> & bits, int num_symbols, int * c_fun, int16_t * vuelta) {
    
    int state = state_f;
    
    for(int i = 0; i < num_symbols; i++) {
        DANS aux = D_rANS(cumuls, M, fq, state, c_fun);
        state = aux.prev_state;
        vuelta[i] = aux.simbolo;
        while (state < 1 * M) {
            int p = bits[bits.size()-1];
            bits.pop_back();
            state = state*2+p;
        }
    }
    state_f = state;
}


int C_rANS(int code, int previous_state, int cumuls[], int M, int fq[]) {
    int s_count = fq[code];
    int p = (previous_state / s_count);
    int next_state = p * M + cumuls[code] + (previous_state % s_count);
    return next_state;
}


void encode(int16_t code[], int fq[], int cumuls[], int M, int & final_State, int num_symbols, vector<short int> &  bitstream) {
    int state = final_State;
    
    for (int i = 0; i < num_symbols; i++) {
        while (state >= 2 * fq[code[i]]) {
            bitstream.push_back((state % 2));
            //state = state / 2;
            state = state >> 1;
        }
        state = C_rANS(code[i], state, cumuls, M, fq);
    }
    final_State = state;
}


void add_bit(bitStream2 & stream, OneBit bit) {
    if (stream.size > 3 * stream.max_size / 4 ) {
        OneBit * aux = new OneBit[2 * stream.max_size];
        
        for(int i = 0; i < stream.size; i++) {
            aux[i] = stream.list[i];
        }
        
        delete [] stream.list;
        stream.list = aux;
        stream.max_size = 2 * stream.max_size;
    }
    
    stream.list[stream.size] = bit;
    stream.size++;
}


void encode2(int16_t code[], int fq[], int cumuls[], int M, int & final_State, int num_symbols, bitStream2  &  bitstream) {
    int state = final_State;
    
    for (int i = 0; i < num_symbols; i++) {
        while (state >= 2 * fq[code[i]]) {
            //bitstream.push_back((state % 2));
            add_bit(bitstream, {(unsigned short int)(state % 2)});
            //state = state / 2;
            state = state >> 1;
        }
        state = C_rANS(code[i], state, cumuls, M, fq);
    }
    final_State = state;
}


void encodeElem(int16_t elem, int fq[], int cumuls[], int M, int & final_State, vector<short int> &  bitstream) {
    int state = final_State;
    while (state >= 2 * fq[elem]) {
        bitstream.push_back((state % 2));
        //state = state / 2;
        state = state >> 1;
    }
    state = C_rANS(elem, state, cumuls, M, fq);
    final_State = state;
}


void encodeElem2(int16_t elem, int fq[], int * cumuls, int M, int & final_State, bitStream2 &  bitstream) {
    int state = final_State;
    while (state >= 2 * fq[elem]) {
        //bitstream.push_back((state % 2));
        add_bit(bitstream, {(unsigned short int)(state % 2)});
        //state = state / 2;
        state = state >> 1;
    }
    state = C_rANS(elem, state, cumuls, M, fq);
    final_State = state;
}


