
#ifndef block_h
#define block_h

#include <string.h>


struct pairZeros{
    int posIni = 0;
    int posFin = 0;
};


struct squaredBlock{
    double * dct_block;
    struct pairZeros * pos_zeros;
    int num_elems;
    int num_zeros;
};


struct canalPorBloques_t{
    struct squaredBlock * bloques;  //tenemos 256 bloques 16x16 en un bloque grande de 256x256
    double * matrix;
    int distribution[2048];  // elementos de los bloques 8x8
    int cumuls[2048+1];
    int cont_block;
    int * numElems; //número de elementos de cada bloque 16x16
    int * numZeros; //número de ceros de cada bloque 16x16
    int * zeros_distribution;  //en cada bloque dct hay 256 elementos
    int M_zeros;
    int M_num;
} canalPorBloques_default = {nullptr, nullptr, {0}, {0}, 0, nullptr, nullptr, nullptr, 0, 0};
typedef struct canalPorBloques_t canalPorBloques;


void initialize_block(canalPorBloques & block, int size) {
    int numBlocks = (256 / size); numBlocks *= numBlocks;
    
    block.bloques = new squaredBlock[numBlocks];
    /*for (int i = 0; i < numBlocks; i++) {
        block.bloques[i] = void;
        //block.bloques[i] = { nullptr, 0, -1, 0};
    }
    */
    
    block.matrix = new double[size * size];
    memset(block.matrix, 0, (size * size) * sizeof(double));
    
    block.numElems = new int[numBlocks];
    memset(block.numElems, 0, numBlocks * sizeof(int));
    
    block.numZeros = new int[numBlocks];
    memset(block.numZeros, 0, numBlocks * sizeof(int));
    
    block.zeros_distribution = new int[(size * size) + 1];
    memset(block.zeros_distribution, 0, ((size * size) + 1) * sizeof(int));
     
}


void delete_block(canalPorBloques * block, int size) {
    int numBlocks = (256 / size); numBlocks *= numBlocks;
    
    for (int i = 0; i < numBlocks; i++) {
        delete [] block->bloques[i].dct_block;
        block->bloques[i].dct_block = nullptr;
        delete [] block->bloques[i].pos_zeros;
        block->bloques[i].pos_zeros = nullptr;
    }
    
    delete [] block->bloques; block->bloques = nullptr;
    delete [] block->matrix; block->matrix = nullptr;
    delete [] block->numElems; block->numElems = nullptr;
    delete [] block->numZeros; block->numZeros = nullptr;
    delete [] block->zeros_distribution; block->zeros_distribution = nullptr;
}

#endif
