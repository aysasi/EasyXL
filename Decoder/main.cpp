#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdint>
#include <stdio.h>


#include "jpeg.h"
#include "colorspaces.h"

#include "fileReader.h"
#include "decoders/ans.h"

#include "block.h"
#include "squareBlocks/block128x128.h"
#include "squareBlocks/block64x64.h"
#include "squareBlocks/block32x32.h"
#include "squareBlocks/block16x16.h"
#include "squareBlocks/block8x8.h"
#include "squareBlocks/block4x4.h"

//#include "squareBlocks/block2x2.h"
//#include "squareBlocks/block256x256.h"


using namespace std;


const char* filename = ""; //NAME TO FILE ENCODED USING EASYXL

FileReader file_reader;

bool necessaryData[3] = { false };

Image imagen;

int blockSize;
int blockSizeSquared;
int numBlocks;
int numBlocksInSquareSide;
int quality;

int bitsNumElems;
int bitsFinalState; //para alcanzar 256 (bloques 16x16), 1024 (8x8) o 4096 (4x4)

pixel_cs (*color_transform) (double, double, double, int, xybP (*)(double, double, double)) = nullptr;
xybP (*xybsystem) (double, double, double) = nullptr;


void writePythonImage() {
    const char* filename2 = "salida.out";
    std::ofstream outFile(filename2, std::ios::binary | std::ios::out);
    
    if (outFile.is_open()) {
        outFile << imagen.height << endl;
        outFile << imagen.width << endl;
        for (int i = 0; i < imagen.height; i++) {
            for (int j = 0; j < imagen.width; j++) {
                for (int ch = 0; ch < imagen.numberChannels; ch++) {
                    outFile << (unsigned int)imagen.matriz[i*imagen.width*imagen.numberChannels + j*imagen.numberChannels + ch] << " ";
                }
            }
            outFile << "\n";
        }
        delete [] imagen.matriz;
        std::cout << "Archivo escrito correctamente" << std::endl;
        outFile.close();
        system("python3 ReadDecodedImage.py");
    }
    else {
        std::cout << "No abierto" << std::endl;
    }
}


void VarDCT(double block[], uint8_t stride, const double * IDCTMatrix) {
    double ** blocksPointers = new double * [blockSize];
    
    for (int i = 0; i < blockSize; i++) {
        blocksPointers[i] = &block[i * stride];
    }
            
    double * blocks_rets = new double[blockSize];
    double * blocks = new double[blockSize];
    
    for (int i = 0; i < blockSize; i++) {
        blocks[i] = *blocksPointers[i];
    }
    
    for (int num_block = 0; num_block < blockSize; num_block++) {
        double aux = 0.0;
        for (int pos = 0; pos < blockSize; pos++) {
            aux += blocks[pos] * IDCTMatrix[num_block * blockSize + pos];
        }
        blocks_rets[num_block] = aux;
    }
        
    for (int i = 0; i < blockSize; i++){
        *blocksPointers[i] = blocks_rets[i];
    }

    delete [] blocksPointers;
    delete [] blocks_rets;
    delete [] blocks;

}


int segmentLength() {  //two bytes segment
    return file_reader.read_bits(16);
}


//APLICATION FRAME APP0 0XE0
void readApplicationFrame() {
    //0xFF APP0, length, identifier, version, units, Xdensity, Ydensity, Xthumbnail, Ythumbnail, (RGB)n

    int length = segmentLength(), i = 0;
    
    ubyte * frame = new ubyte[length-2];
    
    file_reader.readUByte(frame, length-2);
    
    while(frame[i] != 0){
        i++;
    } if (i < length -3) {
        i++;
        std::cout << " Version: " << (int)frame[i++] << "." << (int)frame[i++] << std::endl;
    }
    delete[] frame;
}


//START OF FILE 0xC0
void readSOFFrame() {
    int length = segmentLength();
    
    ubyte * frame = new ubyte[length -2];
    
    file_reader.readUByte(frame, length - 2);
    
    imagen.bitsPerChannel = (uint8_t)frame[0];
    imagen.height = ((uint8_t)(frame[1]) << 8) + (uint8_t)(frame[2]);
    imagen.width = ((uint8_t)(frame[3]) << 8) + (uint8_t)(frame[4]);
    imagen.numberChannels = (uint8_t)frame[5];
    
    cout << endl    << "Anchura: "    << imagen.width << endl;
    cout            << "Altura: "     << imagen.height << endl;
    
    imagen.matriz = new uint8_t[imagen.height * imagen.width * imagen.numberChannels]; //tener en cuenta el numero de canales (por ahora solo uno)
        
    blockSize = (uint8_t)frame[6];
    if (blockSize == 0)
        blockSize = 256;
    cout << "Square block size: " << blockSize << endl;
    
    blockSizeSquared = blockSize * blockSize;
    numBlocksInSquareSide = (256 / blockSize);
    numBlocks = numBlocksInSquareSide * numBlocksInSquareSide;
    
    int spacecolor = (uint8_t)frame[7];
    quality = spacecolor & 0x0F;
    spacecolor = spacecolor >> 4;
    
    if (spacecolor) {
        cout << "Espacio de color: XYB" << endl;
        color_transform = xyb2rgb;
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
    }
    else {
        cout << "Espacio de color: YCbCr" << endl;
        color_transform = ycbcr2rgb;
        cout << "Calidad: " << quality << endl;
    }
    
    cout << endl;
    delete [] frame;
}


void decodeBlock16(squaredBlock * bloque, const double * IDCTMatrix, const int ZigZag[]) {
    
    double * block = bloque->dct_block;
    int num_elems = bloque->num_elems;
    int num_ze = bloque->num_zeros;
    
    
    double * block256 = new double[blockSizeSquared];
    memset(block256, 0, blockSizeSquared * sizeof(double));
    
    int i = 0;
    int j = 0;
    int cont = 0;
    int num_zeroes = 0;
    
    while (cont < num_elems) {
        if (i == bloque->pos_zeros[j].posIni && num_zeroes < num_ze) { //hay un cero en esta posición
            for (int k = i; k <= bloque->pos_zeros[j].posFin; k++)
                block256[ZigZag[k]] = 0;
            i = bloque->pos_zeros[j].posFin + 1;
            num_zeroes++;
            j++;
        }
        else {
            block256[ZigZag[i]] = block[cont];
            i++;
            cont++;
        }
    }
    
    // IDCT: rows
    for (auto offset = 0; offset < blockSize; offset++)
        VarDCT(block256 + offset*blockSize, 1, IDCTMatrix);
    // IDCT: columns
    for (auto offset = 0; offset < blockSize; offset++)
        VarDCT(block256 + offset*1, blockSize, IDCTMatrix);

    
    for (int i = 0; i < blockSizeSquared; i++) {
        double x = block256[i];
        bloque->dct_block[i] = x;
    }
    
    delete [] block256;
}

void readNumElemsSection(int * elementos_array) {
    
    int * distribution_num = new int [blockSizeSquared + 1]; // = {0}; //los números que más se repiten
    memset(distribution_num, 0, sizeof(int) * (blockSizeSquared + 1));
    
    int min = file_reader.read_bits(bitsNumElems);
    int max = file_reader.read_bits(bitsNumElems);
    int num_bits = file_reader.read_bits(4);
        
    for (int i = min; i <= max; i++)
        distribution_num[i] = file_reader.read_bits(num_bits);
    
    int M = numBlocks;
    int finalState = file_reader.read_bits(bitsFinalState) + numBlocks;

    int * cumuls = new int [blockSizeSquared + 2]; // = {0};
    memset(cumuls, 0, sizeof(int) * (blockSizeSquared + 2));
    
    int tamanio = blockSizeSquared + 2;
    int * c_fun_num = new int[M+1];
    memset(c_fun_num, 0, sizeof(int) * (M+1));
    
    for (int i = 1; i < tamanio; i++) {
        cumuls[i] = cumuls[i-1] + distribution_num[i-1];
        for (int j = cumuls[i-1]; j < cumuls[i]; j++)
            c_fun_num[j] = i-1;
    }
    
    decode_num_elems(M, finalState, distribution_num, cumuls, file_reader, numBlocks, c_fun_num, elementos_array);
    
    delete [] distribution_num;
    delete [] cumuls;
    delete [] c_fun_num;
}



void readSection(canalPorBloques * canal, int * distribution, const int bits_dist, const bool zeros, const int tamanio) {
    
    int min = file_reader.read_bits(bits_dist);
    int max = file_reader.read_bits(bits_dist);
    int num_bits = file_reader.read_bits(5);
    
    int M = 0;
    
    if (zeros) {
        for(int i = min; i <= max; i++) {
            distribution[i] = file_reader.read_bits(num_bits);
            M += distribution[i];
        }
    }
    
    else {
        int i = min;
        num_bits++;
        while (i <= max) {
            int dist = file_reader.read_bits(num_bits);
            if (dist >> (num_bits - 1)) { // bit mas significativo =  1 (está codificando ceros)
                dist -= 1 << (num_bits - 1);
                i += dist;
            }
            else {
                distribution[i] = dist;
                M += distribution[i];
                i++;
            }
        }
    }
    
    int * cumuls = new int [tamanio + 2]; // = {0};
    memset(cumuls, 0, sizeof(int) * (tamanio + 2));
    
    int * c_fun = new int[M+1];  //este da problemas a veces también
    memset(c_fun, 0, sizeof(int) * (M + 1));
    for (int i = 1; i < tamanio+2; i++) {
        cumuls[i] = cumuls[i-1] + distribution[i-1];
        for (int j = cumuls[i-1]; j < cumuls[i]; j++)
            c_fun[j] = i-1;
    }
    
    int finalState = file_reader.read_bits(18);
    
    if (zeros) {
        for (int i = 0; i < numBlocks; i++) {
            double * block_zeros = new double[2 * canal->numZeros[i]];
            
            decode_elems(M, finalState, distribution, cumuls, file_reader, 2 * canal->numZeros[i], c_fun, block_zeros);
            
            pairZeros * pares = new pairZeros[canal->numZeros[i]];
            
            for (int j = 0; j < 2 * canal->numZeros[i]; j+=2){
                pares[j/2].posIni = block_zeros[j] + 900;
                pares[j/2].posFin = block_zeros[j+1] + 900;
            }
            
            canal->bloques[i] = {NULL, pares, canal->numElems[i], canal->numZeros[i]};
            delete [] block_zeros;
        }
    }
    
    else {
        for (int i = 0; i < numBlocks; i++) {
            double * block = new double[blockSizeSquared];
            memset(block, 0, blockSizeSquared * sizeof(double));
            
            decode_elems(M, finalState, distribution, cumuls, file_reader, canal->numElems[i], c_fun, block);
            
            canal->bloques[canal->cont_block].dct_block = block;
            canal->cont_block++;
        }
    }
    
    delete [] c_fun;
    delete [] cumuls;
}


void readChannel16(canalPorBloques * canal, const double * IDCTMatrix, const int ZigZag[]) {
    
    readNumElemsSection(canal->numElems);
    readNumElemsSection(canal->numZeros);

    
    readSection(canal, canal->zeros_distribution, bitsNumElems, true, blockSizeSquared);
    readSection(canal, canal->distribution, 12, false, 2047);

    for (int i = 0; i < numBlocks; i++) {
        decodeBlock16(&canal->bloques[i], IDCTMatrix, ZigZag);
    }
}


//START OF SCAN 0XDA
void readSOSFrame() {
    segmentLength();
    
    int * ZigZag = nullptr;
    double * IDCTMatrix = nullptr; // = new double [blockSizeSquared];
    
    /*
    if (blockSize == 256) {
        ZigZag = ZigZag256x256;
        IDCTMatrix = (double *) &IDCTValues256x256;
        
        bitsNumElems = 17;
        bitsFinalState = 3;
    }   */
    
    if (blockSize == 128) {
        ZigZag = ZigZag128x128;
        IDCTMatrix = (double *) &IDCTValues128x128;
        
        bitsNumElems = 15;
        bitsFinalState = 3;
    }
    
    else if (blockSize == 64) {
        ZigZag = ZigZag64x64;
        IDCTMatrix = (double *) &IDCTValues64x64;
        
        bitsNumElems = 13;
        bitsFinalState = 5;
    }
    
    else if (blockSize == 32) {
        ZigZag = ZigZag32x32;
        IDCTMatrix = (double *) &IDCTValues32x32;
        
        bitsNumElems = 11;
        bitsFinalState = 7;
    }
    
    else if (blockSize == 16) {
        ZigZag = ZigZag16x16;
        IDCTMatrix = (double *) &IDCTValues16x16;
        
        bitsNumElems = 9;
        bitsFinalState = 9;
    }
    
    else if (blockSize == 8){
        ZigZag = ZigZag8x8;
        IDCTMatrix = (double *) &IDCTValues8x8;
        
        bitsNumElems = 7;
        bitsFinalState = 11;
    }
    
    else if (blockSize == 4){
        ZigZag = ZigZag4x4;
        IDCTMatrix = (double *) &IDCTValues4x4;
        
        bitsNumElems = 5;
        bitsFinalState = 13;
    }
    
    /*
    else if (blockSize == 2){
        ZigZag = ZigZag2x2;
        IDCTMatrix = (double *) &IDCTValues2x2;
        
        bitsNumElems = 3;
        bitsFinalState = 15;
        
    } */
    
    file_reader.clean();
    for (int x0 = 0; x0 < imagen.height; x0+=256) {
        for (int y0 = 0; y0 < imagen.width; y0 += 256) {
            
            canalPorBloques cX = canalPorBloques_default;
            initialize_block(cX, blockSize);
            
            canalPorBloques cY = canalPorBloques_default;
            initialize_block(cY, blockSize);
            
            canalPorBloques cB = canalPorBloques_default;
            initialize_block(cB, blockSize);
            
            file_reader.clean();
            readChannel16(&cX, IDCTMatrix, ZigZag);
            readChannel16(&cY, IDCTMatrix, ZigZag);
            readChannel16(&cB, IDCTMatrix, ZigZag);
            
            for(int i = 0; i < numBlocksInSquareSide; i++) {
                for(int j = 0; j < numBlocksInSquareSide; j++) {
                    for(int row_aux = 0; row_aux < blockSize; row_aux++) {
                        for(int col_aux = 0; col_aux < blockSize; col_aux++) {
                            double X = cX.bloques[i*numBlocksInSquareSide+j].dct_block[row_aux*blockSize+col_aux];
                            double Y = cY.bloques[i*numBlocksInSquareSide+j].dct_block[row_aux*blockSize+col_aux];
                            double B = cB.bloques[i*numBlocksInSquareSide+j].dct_block[row_aux*blockSize+col_aux];
                            
                            pixel_cs pixel = color_transform(X, Y, B, quality, xybsystem);
                            
                            int row = x0 + (i*blockSize) + row_aux;
                            int column = y0 + (j*blockSize) + col_aux;
                            int pos = row * imagen.width + column;
                            
                            imagen.matriz[pos * 3] = pixel.c1;
                            imagen.matriz[pos * 3 + 1] = pixel.c2;
                            imagen.matriz[pos * 3 + 2] = pixel.c3;
                        }
                    }
                }
            }
            
            delete_block(&cX, blockSize);
            delete_block(&cY, blockSize);
            delete_block(&cB, blockSize);
        
        }
    }
    //delete [] IDCTMatrix;
    file_reader.clean();
}

bool validMarker(char marker) {
    switch (marker) {
        case '\xC0':
            cout << "Start of File marker found" << endl;
            readSOFFrame();
            necessaryData[0] = true;
            return true;
        case '\xDA':
            cout << "Start of Scan marker found" << endl;
            readSOSFrame();
            necessaryData[1] = true;
            return true;
        case '\xD9':
            cout << "End of File" << endl;
            necessaryData[2] = true;
            return false;
        default:
            return false;
    }
}

int main(int argc, const char * argv[]) {
    
    file_reader.link_file(filename);
    
    if (!file_reader.file_open())
        std::cout << "failed to open " << filename << '\n';
    
    else {
        ubyte ff, marker;
        
        file_reader.readUByte(&ff, sizeof(char));
        file_reader.readUByte(&marker, sizeof(char));
        
        // Is this a JPEG file?
        if (ff != 0xFF || marker != 0xD8)
            throw "This is not a JPEG file";
        
        file_reader.readUByte(&ff, sizeof(char));
        file_reader.readUByte(&marker, sizeof(char));
        
        if (ff != 0xFF || marker != 0xE0) {
            throw "Application header not found";
        }
        readApplicationFrame();
        
        file_reader.readUByte(&ff, sizeof(char));
        file_reader.readUByte(&marker, sizeof(char));
        
        while(ff == 0xFF && validMarker(marker)) {
            file_reader.readUByte(&ff, sizeof(char));
            file_reader.readUByte(&marker, sizeof(char));
        }
        
        bool valid = necessaryData[0];
        for (int i = 1; i < 3; i++) {
            valid = valid && necessaryData[i];
        }
        
        if (valid)
            writePythonImage();
        
        else
            cout << "Archivo corrupto" << endl;
    }
    
    return 0;
}
