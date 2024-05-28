#include <iostream>

#include "imageEncoder.h"
#include "colorspaces.h"
#include "fileWriter.h"
#include "encoder/ans.h"
#include "block.h"

#include "squareBlocks/block4x4.h"
#include "squareBlocks/block8x8.h"
#include "squareBlocks/block16x16.h"
#include "squareBlocks/block32x32.h"
#include "squareBlocks/block64x64.h"
#include "squareBlocks/block128x128.h"

//#include "squareBlocks/block2x2.h"
//#include "squareBlocks/block256x256.h"


pixel_cs (*color_transform) (int, int, int, int, pixel_cs (*)(double, double, double)) = nullptr;
pixel_cs (*xybsystem) (double, double, double) = nullptr;

int bitsNumElems;
int bitsFinalState;


void VarDCT(double block[], uint8_t stride, uint8_t size, const double * DCTMatrix) {
    double ** blocksPointers = new double * [size];
    
    for (int i = 0; i < size; i++) {
        blocksPointers[i] = &block[i * stride];
    }

    
    double * blocks_rets = new double[size];
    double * blocks = new double[size];
    
    for (int i = 0; i < size; i++) {
        blocks[i] = *blocksPointers[i];
    }
    
    for (int num_block = 0; num_block < size; num_block++) {
        double aux = 0.0;
        for (int pos = 0; pos < size; pos++) {
            aux += blocks[pos] * DCTMatrix[num_block * size + pos];
        }
        blocks_rets[num_block] = aux;
    }
        
    for (int i = 0; i < size; i++){
        *blocksPointers[i] = blocks_rets[i];
    }

    delete [] blocksPointers;
    delete [] blocks_rets;
    delete [] blocks;

}


void encodePBlock256(canalPorBloques * canal, uint8_t size, const double * DCTMatrix, const int ZigZag[]) {
    double * blockarray = (double*) canal->matrix;

    for (int offset = 0; offset < size; offset++)
        VarDCT(blockarray + offset * size, 1, size, DCTMatrix);
    
    for (int offset = 0; offset < size; offset++)
        VarDCT(blockarray + offset, size, size, DCTMatrix);

    int posNonZero = -1;
    
    int ini = -1;
    int fin = -1;
    
    struct pairZeros * zeros = new pairZeros[size * size];
    int num_zeros = 0;
    int num_elems = 0;
    
    
    int16_t * closest = new int16_t[size * size];  //closest integer
    memset(closest, 0, (size * size) * sizeof(int16_t));
    for (auto i = 0; i < (size * size); i++) {
        double value = blockarray[ZigZag[i]];
        closest[i] = int(value + (value >= 0 ? +0.5 : -0.5)); // round to nearest integer
        if (closest[i] != 0) {
            posNonZero = i;
            canal->distribution[closest[i] + 900]++;
            num_elems++;
            if (ini != -1) {
                zeros[num_zeros] = {ini, fin};
                canal->zeros_distribution[ini]++;
                canal->zeros_distribution[fin]++;
                ini = -1;
                fin = -1;
                num_zeros++;
            }
        }
        
        else {  //en la posicion ZigZag[i] hay un cero
            if (ini == -1)
                ini = i;
            fin = i;
        }
        
    }

    squaredBlock aux = { new int16_t[num_elems], zeros, num_elems, num_zeros};

    int j = num_elems-1;
    for (int i = posNonZero; i >= 0; i--){
        if (closest[i] != 0) {
            aux.dct_block[j] = closest[i] + 900;
            j--;
        }

    }

    canal->bloques[canal->cont_block] = aux;
    canal->numElems[canal->cont_block] = num_elems;
    canal->numZeros[canal->cont_block] = num_zeros;
    canal->M_zeros += 2 * num_zeros;
    canal->M_num += num_elems;
    canal->cont_block++;
    
    delete [] closest;
}


void encodeNumElemsSection16(int numElems[], uint8_t size, BitWriter & bitWriter) {
    int M_num = 0;
    int num_bloques = 256 / size; num_bloques *= num_bloques;
    int * distribution_num = new int [(size * size) + 1]; // = {0};  //distribución del numero de elementos a codificar en cada bloque
    memset(distribution_num, 0, ((size * size) + 1) * sizeof(int));
    
    //int16_t elementos[256] = {0};
    int16_t * elementos = new int16_t[num_bloques];
    memset(elementos, 0, num_bloques * sizeof(int16_t));
    
    int min = size * size;
    int max = 0;
    int max_dist = 0;
    
    for(int i = num_bloques - 1; i >= 0; i--){
        distribution_num[numElems[i]]++;
        if (distribution_num[numElems[i]] > max_dist)
            max_dist = distribution_num[numElems[i]];
        if (numElems[i] > max)
            max = numElems[i];
        if (numElems[i] < min)
            min = numElems[i];
        elementos[num_bloques - 1 - i] = numElems[i];  //lista que se va a codificar
        M_num++;
    }
    
    int tamanio = (size * size) + 2;
    int * cumuls = new int [tamanio];
    memset(cumuls, 0, tamanio * sizeof(int));
    
    for (int i= 1; i < tamanio; i++)
        cumuls[i] = cumuls[i-1] + distribution_num[i-1];
    
    int finalState = M_num;

    bitStream2 bitstream = {1000, 0, new OneBit[1000]};

    encode2(elementos, distribution_num, cumuls, M_num, finalState, num_bloques, bitstream);
    
    int num_bits = 0;
    while (max_dist){
        num_bits++;
        max_dist = max_dist >> 1;
    }
    
    bitWriter << BitCode(min, bitsNumElems);
    bitWriter << BitCode(max, bitsNumElems);
    bitWriter << BitCode(num_bits, 4);
    for (int i = min; i <= max; i++)
        bitWriter << BitCode(distribution_num[i], num_bits);
    
    
    int maxSize = 256 / size; maxSize *= maxSize;  // numero de bloques size*size que hay en un bloque de 256*256
    bitWriter << BitCode(finalState - maxSize, bitsFinalState);
    
    for (int i = bitstream.size -1; i >=0 ; i--)
        bitWriter << BitCode(bitstream.list[i].bit, 1);
    
    delete [] cumuls;
    delete [] elementos;
    delete [] distribution_num;
    delete [] bitstream.list;
}


void encodeElemsSection(BitWriter & bitWriter, int max_size, int bits_distribution, int * distribution, int finalState, bitStream2 & bitstream, bool zeros) {
    //zeros distribution
    int max_dist = 0;
    int min = max_size;
    int max = 0;
    for (int i = 0; i < max_size; i++){
        if (distribution[i] > max_dist)
            max_dist = distribution[i];
        if (distribution[i] != 0 && i < min)
            min = i;
        if (distribution[i] != 0 && i > max)
            max = i;
    }
    
    int num_bits = 0;
    while (max_dist){
        num_bits++;
        max_dist = max_dist >> 1;
    }
    
    bitWriter << BitCode(min, bits_distribution);
    bitWriter << BitCode(max, bits_distribution);
    bitWriter << BitCode(num_bits, 5);
    
    if (zeros) {
        for (int i = min; i <= max; i++)
            bitWriter << BitCode(distribution[i], num_bits);
    }
    
    else {
        num_bits++;
        int i = min;
        
        while (i <= max) {
            if (distribution[i] != 0) {
                bitWriter << BitCode(distribution[i], num_bits);
                i++;
            }
            
            else { //contar el número de ceros
                int num_ceros = 0;
                while (distribution[i] == 0 && num_ceros < ((1 << (num_bits - 1)) - 1)) {
                    num_ceros++;
                    i++;
                }
                int aux = (1 << (num_bits - 1));
                aux |= num_ceros;
                bitWriter << BitCode(aux, num_bits);
            }
        }
    }
    
    
    bitWriter << BitCode(finalState, 18);
    
    for (int i = bitstream.size -1; i >=0 ; i--)
        bitWriter << BitCode(bitstream.list[i].bit, 1);
    delete [] bitstream.list;
    bitstream.list = NULL;
    
}


void encodePBitstream(canalPorBloques & canal, BitWriter & bitWriter, uint8_t size) {
    
    int num_bloques = (256 / size); num_bloques *= num_bloques;
    
    encodeNumElemsSection16(canal.numElems, size, bitWriter);  //codifica el número de elementos a decodificar de cada bloque 16x16
    encodeNumElemsSection16(canal.numZeros, size, bitWriter);  //codifica el número de ceros a decodificar de cada bloque 16x16
    
    int * cumuls = new int[(size * size) + 2];
    memset(cumuls, 0, ((size * size) + 2) * sizeof(int));
    
    for (int i = 1; i < ((size * size) + 2); i++) {
        cumuls[i] = cumuls[i-1] + canal.zeros_distribution[i-1];
    }
    
    int finalState = canal.M_zeros;
    bitStream2 bitstream = {10000, 0, new OneBit[10000]};
    
    for (int i = num_bloques - 1; i >= 0 ; i--) {
        squaredBlock aux = canal.bloques[i];
        for (int j = aux.num_zeros-1; j >= 0; j--) {
            encodeElem2(aux.pos_zeros[j].posFin, canal.zeros_distribution, cumuls, canal.M_zeros, finalState, bitstream);  //codifica la posición final de cada secuencia de ceros
            encodeElem2(aux.pos_zeros[j].posIni, canal.zeros_distribution, cumuls, canal.M_zeros, finalState, bitstream); //codifica la posición inicial de cada secuencia de ceros
        }
        delete [] canal.bloques[i].pos_zeros;
        canal.bloques[i].pos_zeros = NULL;
    }
    delete [] cumuls;
    
    encodeElemsSection(bitWriter, (size * size) + 1, bitsNumElems, canal.zeros_distribution, finalState, bitstream, true);  //codifica la distribución
        
    //CODIFICAR ELEMENTOS BLOQUES 8X8
    
    cumuls = new int[2049];
    memset(cumuls, 0, 2049 * sizeof(int));

    for (int i= 1; i <= 2048; i++) {
        cumuls[i] = cumuls[i-1] + canal.distribution[i-1];
    }

    finalState = canal.M_num;
    bitstream = {10000, 0, new OneBit[10000]};
    
    for (int i = num_bloques - 1; i >= 0 ; i--) {
        squaredBlock aux = canal.bloques[i];
        for (int j = aux.num_elems-1; j >= 0; j--) {
            encodeElem2(aux.dct_block[j], canal.distribution, cumuls, canal.M_num, finalState, bitstream);
        }
        delete [] canal.bloques[i].dct_block;
        canal.bloques[i].dct_block = NULL;
    }
    delete [] cumuls;
    
    
    encodeElemsSection(bitWriter, 2048, 12, canal.distribution, finalState, bitstream, false);
    
}


bool writeXLImage(const void* pixels_, unsigned short width, unsigned short height, int blockSize, bool xybcolorspace, int quality, char * filename) {
    
    if (width <= 0 || height <= 0) {
        cout << "Tamaño imagen inválido" << endl;
        return false;
    }
    if (quality > 4 || quality < 0 || quality == 3) {
        cout << "Parámetro calidad no válido" << endl;
        return false;
    }
    
    cout << endl;
    
    if (xybcolorspace) {
        cout << "Espacio de color: XYB" << endl;
        color_transform = rgb2xyb;
        switch (quality) {
            case 1:
                xybsystem = sistema256;
                break;
            case 2:
                xybsystem = sistema512;
                break;
            default:
                quality = 4;
                xybsystem = sistema1024;
                break;
        }
        cout << "Calidad: " << quality << endl;
        cout << "BlockSize: " << (unsigned int)blockSize << endl;
    }
    
    else {
        color_transform = rgb2ycbcr; //aqui el argumento (funcion) sistema da igual
        cout << "Espacio de color: YCbCr" << endl;
        cout << "Calidad: " << quality << endl;
        cout << "BlockSize: " << (unsigned int)blockSize << endl;
    }
    
    cout << endl;
    
    BitWriter bitWriter; bitWriter.link_file(filename);
    
    // ////////////////////////////////////////
    // JFIF headers
    const uint8_t HeaderJfif[2+2+16] =
    { 0xFF,0xD8,         // SOI marker (start of image)
        0xFF,0xE0,         // JFIF APP0 tag
        0,16,              // length: 16 bytes (14 bytes payload + 2 bytes for this length field)
        'J','F','I','F',0, // JFIF identifier, zero-terminated
        1,1,               // JFIF version 1.1
        0,                 // no density units specified
        0,1,0,1,           // density: 1 pixel "per pixel" horizontally and vertically
        0,0 };             // no thumbnail (size 0 x 0)
    bitWriter << HeaderJfif;
    
    bitWriter.addMarker(0xC0, 2+8); // length: 6 bytes general info + 3 per channel + 2 bytes for this length field + byte tamaño bloque + byte espacio de color y calidad
    
    // 8 bits per channel
    bitWriter << 0x08
    // image dimensions (big-endian)
    << (height >> 8) << (height & 0xFF)
    << (width  >> 8) << (width  & 0xFF);
    
    bitWriter << 3; //three channels

    bitWriter << blockSize; //three channels
    
    bitWriter << ((((int) xybcolorspace) << 4) | quality);
    
    
    
    
    bitWriter.addMarker(0xDA, 1); //START OF SCAN
    auto pixels = (const uint8_t*)pixels_;

    
    double * DCTMatrix = nullptr; // = new double [blockSize * blockSize];
    int * ZigZag = nullptr;

    /*
    if (blockSize == 256) {
        ZigZag = ZigZag256x256;
        DCTMatrix = (double *) &DCTValues256x256;

        bitsNumElems = 17;
        bitsFinalState = 3;
    }   */
    
    if (blockSize == 128) {
        ZigZag = ZigZag128x128;
        DCTMatrix = (double *) &DCTValues128x128;
        
        bitsNumElems = 15;
        bitsFinalState = 3;
    }
    
    else if (blockSize == 64) {
        ZigZag = ZigZag64x64;
        DCTMatrix = (double *) &DCTValues64x64;
        
        bitsNumElems = 13;
        bitsFinalState = 5;
    }
    
    else if (blockSize == 32) {
        ZigZag = ZigZag32x32;
        DCTMatrix = (double *) &DCTValues32x32;
        
        bitsNumElems = 11;
        bitsFinalState = 7;
    }
    
    else if (blockSize == 16) {
        ZigZag = ZigZag16x16;
        DCTMatrix = (double *) &DCTValues16x16;
        
        bitsNumElems = 9;
        bitsFinalState = 9;
        
        
    }
    else if (blockSize == 8) {
        ZigZag = ZigZag8x8;
        DCTMatrix = (double *) &DCTValues8x8;
        
        bitsNumElems = 7;
        bitsFinalState = 11;
        
    }
    
    else if (blockSize == 4) { //block
        ZigZag = ZigZag4x4;
        DCTMatrix = (double *) &DCTValues4x4;
        
        bitsNumElems = 5;
        bitsFinalState = 13;
    }
    
    /*
    else if (blockSize == 2) { //block
        ZigZag = ZigZag2x2;
        DCTMatrix = (double *) &DCTValues2x2;
        
        bitsNumElems = 3;
        bitsFinalState = 15;
    }   */
    
    else
        return false;
    
    
    for (int X_256 = 0; X_256 < height; X_256 += 256) {
        for (int Y_256 = 0; Y_256 < width; Y_256 += 256) {
            
            canalPorBloques cpX = canalPorBloques_default;
            initialize_block(cpX, blockSize);
            
            canalPorBloques cpY = canalPorBloques_default;
            initialize_block(cpY, blockSize);
            
            canalPorBloques cpB = canalPorBloques_default;
            initialize_block(cpB, blockSize);
            
            for (int blockX = 0; blockX < 256; blockX += blockSize) {
                for (int blockY = 0; blockY < 256; blockY += blockSize) {
                    
                    for (int i = 0; i < blockSize; i++) {
                        for (int j = 0; j < blockSize; j++) {
                            int row = X_256 + blockX + i;
                            int column = Y_256 + blockY + j;
                            
                            int pos = row * width + column;
                            
                            int r = pixels[3 * pos    ];
                            int g = pixels[3 * pos + 1] ;
                            int b = pixels[3 * pos + 2];
                            
                            pixel_cs pixel = color_transform(r, g, b, quality, xybsystem);
                            cpX.matrix[i*blockSize+j] = pixel.c1;
                            cpY.matrix[i*blockSize+j] = pixel.c2;
                            cpB.matrix[i*blockSize+j] = pixel.c3;
                        }
                    }
                    encodePBlock256(&cpX, blockSize, DCTMatrix, ZigZag);
                    encodePBlock256(&cpY, blockSize, DCTMatrix, ZigZag);
                    encodePBlock256(&cpB, blockSize, DCTMatrix, ZigZag);
                }
            }
            bitWriter.clean();
            
            encodePBitstream(cpX, bitWriter, blockSize);
            delete_block(cpX);
            
            encodePBitstream(cpY, bitWriter, blockSize);
            delete_block(cpY);
            
            encodePBitstream(cpB, bitWriter, blockSize);
            delete_block(cpB);
            
            bitWriter.flush();
        }
    }

    // EOI marker
    bitWriter << 0xFF << 0xD9;
    bitWriter.terminate();
    return true;
}

