/*                       
    [Objective]         1. Make Linear Approximation Table for S-box
                        2. Make Linear Approximation Table for Inverse S-box
*/

#include <stdio.h>
#include <stdlib.h>
#include "toy_cipher.h"

// Decimal to Binary
// return mallocated BIT* val
BIT* dec_to_bin(int dec, int size){
    BIT* bin = malloc(sizeof(BIT) * size);
    for(int i = 0; i < size; i++){
        bin[size - 1 - i] = dec % 2;
        dec /= 2;
    }
    return bin;
}

// Make Linear Approximation Table of S-Box
void make_approx_table(){
    //Linear Approximation Table
    BIT approx_table[16][16];
    // Input Sum a
    for(int a = 0; a < 16; a++){
        BIT* a_bin = dec_to_bin(a, 4);
        // Output Sum B
        for(int b = 0; b < 16; b++){
            approx_table[a][b] = -8;
            BIT* b_bin = dec_to_bin(b, 4); 
            for(int X_in = 0; X_in < 16; X_in++){
                int Y_out = s_box[X_in];
                // Input X
                BIT* X_in_bin = dec_to_bin(X_in, 4);
                // Output Y
                BIT* Y_out_bin = dec_to_bin(Y_out, 4);
                BIT result = 0;
                for(int i = 0; i < 4; i++){
                    if(a_bin[i] == 1){
                        result ^= X_in_bin[i];
                    }
                }
                for(int j = 0; j < 4; j++){
                    if(b_bin[j] == 1){
                        result ^= Y_out_bin[j];
                    }
                }
                if(result == 0){
                    approx_table[a][b]++;
                }
                free(X_in_bin);
                free(Y_out_bin);
            }
            free(b_bin);
        }
        free(a_bin);
    }
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            printf("%4d,", approx_table[i][j]);
        }
        printf("\n");
    }
}

// Make Linear Approximation Table of Inverse S-Box
void make_inv_approx_table(){
    //Linear Approximation Table
    BIT approx_table[16][16];
    // Input Sum a
    for(int a = 0; a < 16; a++){
        BIT* a_bin = dec_to_bin(a, 4);
        // Output Sum B
        for(int b = 0; b < 16; b++){
            approx_table[a][b] = -8;
            BIT* b_bin = dec_to_bin(b, 4); 
            for(int X_in = 0; X_in < 16; X_in++){
                int Y_out = inv_s_box[X_in];
                // Input X
                BIT* X_in_bin = dec_to_bin(X_in, 4);
                // Output Y
                BIT* Y_out_bin = dec_to_bin(Y_out, 4);
                BIT result = 0;
                for(int i = 0; i < 4; i++){
                    if(a_bin[i] == 1){
                        result ^= X_in_bin[i];
                    }
                }
                for(int j = 0; j < 4; j++){
                    if(b_bin[j] == 1){
                        result ^= Y_out_bin[j];
                    }
                }
                if(result == 0){
                    approx_table[a][b]++;
                }
                free(X_in_bin);
                free(Y_out_bin);
            }
            free(b_bin);
        }
        free(a_bin);
    }
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            printf("%4d,", approx_table[i][j]);
        }
        printf("\n");
    }
}

// Main Func
int main(){
    make_approx_table();
    /*
    BIT m_table[0x10][0x10] =    {  8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                    0,   0,  -2,  -2,   0,   0,  -2,   6,   2,   2,   0,   0,   2,   2,   0,   0,
                                    0,   0,  -2,  -2,   0,   0,  -2,  -2,   0,   0,   2,   2,   0,   0,  -6,   2,
                                    0,   0,   0,   0,   0,   0,   0,   0,   2,  -6,  -2,  -2,   2,   2,  -2,  -2,
                                    0,   2,   0,  -2,  -2,  -4,  -2,   0,   0,  -2,   0,   2,   2,  -4,   2,   0,
                                    0,  -2,  -2,   0,  -2,   0,   4,   2,  -2,   0,  -4,   2,   0,  -2,  -2,   0,
                                    0,   2,  -2,   4,   2,   0,   0,   2,   0,  -2,   2,   4,  -2,   0,   0,  -2,
                                    0,  -2,   0,   2,   2,  -4,   2,   0,  -2,   0,   2,   0,   4,   2,   0,   2,
                                    0,   0,   0,   0,   0,   0,   0,   0,  -2,   2,   2,  -2,   2,  -2,  -2,  -6,
                                    0,   0,  -2,  -2,   0,   0,  -2,  -2,  -4,   0,  -2,   2,   0,   4,   2,  -2,
                                    0,   4,  -2,   2,  -4,   0,   2,  -2,   2,   2,   0,   0,   2,   2,   0,   0,
                                    0,   4,   0,  -4,   4,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                    0,  -2,   4,  -2,  -2,   0,   2,   0,   2,   0,   2,   4,   0,   2,   0,  -2,
                                    0,   2,   2,   0,  -2,   4,   0,   2,  -4,  -2,   2,   0,   2,   0,   0,   2,
                                    0,   2,   2,   0,  -2,  -4,   0,   2,  -2,   0,   0,  -2,  -4,   2,  -2,   0,
                                    0,  -2,  -4,  -2,  -2,   0,   2,   0,   0,  -2,   4,  -2,  -2,   0,   2,   0    };
    */
    make_inv_approx_table();
    /*
    BIT inv_m_table[0x10][0x10] = { 8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                    0,   0,   0,   0,   2,  -2,   2,  -2,   0,   0,   4,   4,  -2,   2,   2,  -2,
                                    0,  -2,  -2,   0,   0,  -2,  -2,   0,   0,  -2,  -2,   0,   4,   2,   2,  -4,
                                    0,  -2,  -2,   0,  -2,   0,   4,   2,   0,  -2,   2,  -4,  -2,   0,   0,  -2,
                                    0,   0,   0,   0,  -2,  -2,   2,   2,   0,   0,  -4,   4,  -2,  -2,  -2,  -2,
                                    0,   0,   0,   0,  -4,   0,   0,  -4,   0,   0,   0,   0,   0,   4,  -4,   0,
                                    0,  -2,  -2,   0,  -2,   4,   0,   2,   0,  -2,   2,   4,   2,   0,   0,   2,
                                    0,   6,  -2,   0,   0,   2,   2,   0,   0,  -2,  -2,   0,   0,   2,   2,   0,
                                    0,   2,   0,   2,   0,  -2,   0,  -2,  -2,  -4,   2,   0,   2,  -4,  -2,   0,
                                    0,   2,   0,  -6,  -2,   0,  -2,   0,   2,   0,   2,   0,   0,  -2,   0,  -2,
                                    0,   0,   2,  -2,   0,  -4,   2,   2,   2,  -2,   0,   0,   2,   2,   0,   4,
                                    0,   0,   2,  -2,   2,   2,   4,   0,  -2,   2,   0,   0,   4,   0,  -2,  -2,
                                    0,   2,   0,   2,   2,   0,  -2,   4,   2,   0,   2,   0,   0,   2,  -4,  -2,
                                    0,   2,   0,   2,  -4,  -2,   0,   2,  -2,   4,   2,   0,   2,   0,   2,   0,
                                    0,   0,  -6,  -2,   2,  -2,   0,   0,  -2,   2,   0,   0,   0,   0,  -2,   2,
                                    0,   0,   2,  -2,   0,   0,  -2,   2,  -6,  -2,   0,   0,  -2,   2,   0,   0    };
    */

}