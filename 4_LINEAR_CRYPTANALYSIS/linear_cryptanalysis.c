/*
    [Objective]         Assuming that you do not know the secret key,
                        retreive the first round's and last round's subkey (2 * 16bit)      
                            - use plaintext and ciphertext pairs made in Problem1
                            - use linear cryptanalysis and C programming
                            - show all of the processes for solving the problem
                        
    [Objective]         1. Look for route that has largest linear bias
                        2. Find active S-box with bias
                        3. Calculate bias approximation value for each 8 bit subkey (0x00 ~ 0xFF)
                        4. Select a subkey by comparing bias approximation value with active S-box bias
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "toy_cipher.h"

// Global Array for 10,000 pc pairs
BIT pc_pair[PAIR_NUM][2][200];      // [0]: plaintext
                                    // [1]: ciphertext

// Convert ASCII '0', '1' of pc_pair to 0, 1
void ascii_to_bin(){
    for(int i = 0; i < PAIR_NUM; i++){
        for(int j = 0; j < TEXT_LEN; j++){
            pc_pair[i][0][j] -= '0';
            pc_pair[i][1][j] -= '0';
        }
    }
}

// For debugging, show bits
void show_bits(BIT bits[], int len){
    for (int i = 0; i < len; i++){
        if (i % 4 == 0){
            printf("/ ");
        }
        printf("%d ", bits[i]);
    }
    printf("\n");
}

// Array shallow copy
void array_copy(BIT dst[], BIT src[], int len){
    for (int i = 0; i < len; i++){
        dst[i] = src[i];
    }
}

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

// Binary to Decimal
// return int val
int bin_to_dec(BIT bin[], int size){
    int dec = 0;
    for(int i = 0; i < size; i++){
        dec *= 2;
        dec += bin[i];
    }
    return dec;
}

// Sub-func for substitute
void substitute_iter(BIT bits[], int idx){
    int in = 0;
    int out;
    BIT* out_bin;

    in = bin_to_dec(bits + 4 * idx, 4);
    out = s_box[in];
    out_bin = dec_to_bin(out, 4);

    if(DEBUG){
        printf("(%X -> %X), ", in, out);
    }

    for(int i = 0; i < 4; i++){
        bits[4 * idx + i] = out_bin[i];
    }
    free(out_bin);
}

// Substitue "bits" by calling sub-func 'substitute_iter'
BIT* substitute(BIT bits[]){
    if(DEBUG){
        printf("\t\t\t\t\t\t");
    }
    for (int i = 0; i < 4; i++){
        // Apply S-box for each 4bit
        substitute_iter(bits, i);
    }
    if(DEBUG){
        printf("\n");
    }
    return bits;
}

// Sub-func for inverse substitute
void inv_substitute_iter(BIT bits[], int idx){
    int in = 0;
    int out;
    BIT* out_bin;

    in = bin_to_dec(bits + 4 * idx, 4);
    out = inv_s_box[in];
    out_bin = dec_to_bin(out, 4);

    if(DEBUG){
        printf("(%X -> %X), ", in, out);
    }

    for(int i = 0; i < 4; i++){
        bits[4 * idx + i] = out_bin[i];
    }
    free(out_bin);
}

// Inverse substitue "bits" by calling sub-func 'inv_substitute_iter'
BIT* inv_substitute(BIT bits[]){
    if(DEBUG){
        printf("\t\t\t\t\t\t");
    }
    for (int i = 0; i < 4; i++){
        // Apply Inverse S-box for each 4bit
        inv_substitute_iter(bits, i);
    }
    if(DEBUG){
        printf("\n");
    }
    return bits;
}

// Get bias by count value
double get_bias(int count){
    return (double)abs(count - PAIR_NUM / 2) / PAIR_NUM;
}

// Get partial subkey (Last round's key)
int get_partial_subkey(BIT P_idx[], BIT U_idx[], BIT K[], int P_len, int U_len, int K_len){
    // Table of experimental results for Linear Attack
    double bias_table[SUBKEY_LEN * SUBKEY_LEN];

    double max = 0;
    int maxIdx = -1;
    for(int i = 0; i < SUBKEY_LEN * SUBKEY_LEN; i++){
        // for one entry of the bias table (i is decimal value of partial subkey. 0x00 ~ 0xFF)
        int low_key = i / SUBKEY_LEN;
        int high_key = i % SUBKEY_LEN; 
        
        // Append some 0 paddings
        BIT* partial_key = dec_to_bin(low_key * (int)pow(SUBKEY_LEN, 3 - K[0]) 
                                        + high_key * (int)pow(SUBKEY_LEN, 3 - K[1]), 
                                      SUBKEY_LEN);
        if(DEBUG) show_bits(key, SUBKEY_LEN);

        int count = 0;
        for(int j = 0; j < PAIR_NUM; j++){ 
            // For PAIR_NUM pc pairs
            BIT p_text[TEXT_LEN];
            BIT c_text[TEXT_LEN];
            BIT V[TEXT_LEN];
            BIT* U;
            BIT result = 0;

            array_copy(p_text, pc_pair[j][0], sizeof(p_text));
            array_copy(c_text, pc_pair[j][1], sizeof(c_text));

            // Partially Decrypt the ciphertext
            for(int i = 0; i < TEXT_LEN; i++){
                V[i] = c_text[i] ^ partial_key[i];
            }
            // Get value of U by using Inverse S-box
            U = inv_substitute(V);    

            if(DEBUG){
                printf(" [P] ");
                for(int i = 0; i < TEXT_LEN; i++){
                    printf("%d", p_text[i]);
                }
                printf(" [C] ");
                for(int i = 0; i < TEXT_LEN; i++){
                    printf("%d", c_text[i]);
                }
                printf("\n");
                printf("[U] ");
                show_bits(U, TEXT_LEN);
            }
            
            // XOR the specified P and U bits, and save it as the result
            for(int p = 0; p < P_len; p++){
                result ^= p_text[P_idx[p]];
                if(DEBUG) printf("p_text[%d]: %d, result: %d\n", P_idx[p], p_text[P_idx[p]], result);
            }   
            for(int u = 0; u < U_len; u++){
                result ^= U[U_idx[u]];
                if(DEBUG) printf("U[%d]: %d, result: %d\n", U_idx[u], U[U_idx[u]], result);
            }

            // if the result is ZERO, increment the count
            if(result == 0){
                count++;
            }
        }
        // Update the value of bias_table
        bias_table[i] = get_bias(count);
        if (bias_table[i] > max){
            max = bias_table[i];
            maxIdx = i;
        }
        free(partial_key);
        if(DEBUG) printf(" [%2X] count: %d, bias: %f\n", i, count, bias_table[i]);
    }
    printf("[%02x] bias: %f", maxIdx, bias_table[maxIdx]);
    return maxIdx;  // return partial subkey with maximum bias magnitude
}

// Get partial subkey (First round's key)
int get_partial_subkey_2(BIT C_idx[], BIT U_idx[], BIT K[], int C_len, int U_len, int K_len){
    // Table of experimental results for Linear Attack
    double bias_table[SUBKEY_LEN * SUBKEY_LEN];

    double max = 0;
    int maxIdx = -1;
    for(int i = 0; i < SUBKEY_LEN * SUBKEY_LEN; i++){
        // for one entry of the bias table (i is decimal value of partial subkey. 0x00 ~ 0xFF)
        int low_key = i / SUBKEY_LEN;
        int high_key = i % SUBKEY_LEN; 
        
        // Append some 0 paddings
        BIT* partial_key = dec_to_bin(low_key * (int)pow(SUBKEY_LEN, 3 - K[0]) 
                                        + high_key * (int)pow(SUBKEY_LEN, 3 - K[1]), 
                                      SUBKEY_LEN);
        if(DEBUG) show_bits(key, SUBKEY_LEN);

        int count = 0;
        for(int j = 0; j < PAIR_NUM; j++){ 
            // For PAIR_NUM pc pairs
            BIT p_text[TEXT_LEN];
            BIT c_text[TEXT_LEN];
            BIT V[TEXT_LEN];
            BIT* U;
            BIT result = 0;

            array_copy(p_text, pc_pair[j][0], sizeof(p_text));
            array_copy(c_text, pc_pair[j][1], sizeof(c_text));

            // Partially Decrypt the ciphertext
            for(int i = 0; i < TEXT_LEN; i++){
                V[i] = p_text[i] ^ partial_key[i];
            }
            // Get value of U by using Inverse S-box
            U = substitute(V);    

            if(DEBUG){
                printf(" [P] ");
                for(int i = 0; i < TEXT_LEN; i++){
                    printf("%d", p_text[i]);
                }
                printf(" [C] ");
                for(int i = 0; i < TEXT_LEN; i++){
                    printf("%d", c_text[i]);
                }
                printf("\n");
                printf("[U] ");
                show_bits(U, TEXT_LEN);
            }
            
            // XOR the specified P and U bits, and save it as the result
            for(int c = 0; c < C_len; c++){
                result ^= c_text[C_idx[c]];
                if(DEBUG) printf("c_text[%d]: %d, result: %d\n", C_idx[c], c_text[C_idx[c]], result);
            }   
            for(int u = 0; u < U_len; u++){
                result ^= U[U_idx[u]];
                if(DEBUG) printf("U[%d]: %d, result: %d\n", U_idx[u], U[U_idx[u]], result);
            }

            // if the result is ZERO, increment the count
            if(result == 0){
                count++;
            }
        }
        // Update the value of bias_table
        bias_table[i] = get_bias(count);
        if (bias_table[i] > max){
            max = bias_table[i];
            maxIdx = i;
        }
        free(partial_key);
        if(DEBUG) printf(" [%2X] count: %d, bias: %f\n", i, count, bias_table[i]);
    }
    printf("[%02x] bias: %f", maxIdx, bias_table[maxIdx]);
    return maxIdx;  // return partial subkey with maximum bias magnitude
}

// Main func
int main(){
    // [Objective] Retreive the first round's and last round's subkey (2 * 16bit)
    FILE* fp = fopen("pc_pairs_random.txt", "r");

    int cnt = 0;
    if(DEBUG) printf("[   #] Plaintext        Ciphertext\n");
    while(!feof(fp)){
        fscanf(fp, "%s %s\n", pc_pair[cnt][0], pc_pair[cnt][1]);
        if(DEBUG) printf("[%4d] %s\t%s\n", cnt, pc_pair[cnt][0], pc_pair[cnt][1]);
        cnt++;
    }
    fclose(fp); 
    ascii_to_bin();

    BIT k5[SUBKEY_LEN];
    BIT k1[SUBKEY_LEN];
    BIT* temp;

    // Last round's key 1..4, 9..12
    BIT last_P_idx[] = {5, 6, 7};
    BIT last_U_idx[] = {1, 3, 9, 11};
    BIT last_K[]     = {0, 2};
    printf("K5,1..4 / K5,9..12   ->  ");
    int last_maxIdx = get_partial_subkey(last_P_idx, last_U_idx, last_K, sizeof(last_P_idx), sizeof(last_U_idx), sizeof(last_K));
    printf(" (active S-box bias: %f)\n", 16.0 * 4 * 4 * 4 * 4 * 4 / (16 * 16 * 16 * 16 * 16));
    temp = dec_to_bin(last_maxIdx, 8);
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 4; i++){
            k5[4 * last_K[j] + i] = temp[4 * j + i];
        }
    }
    free(temp);
    
    // Last round's key 5..8, 13..16
    BIT last_P_idx_2[] = {4, 6, 7};
    BIT last_U_idx_2[] = {5, 7, 13, 15};
    BIT last_K_2[]     = {1, 3};
    printf("K5,5..8 / K5,13..16  ->  ");
    int last_maxIdx_2 = get_partial_subkey(last_P_idx_2, last_U_idx_2, last_K_2, sizeof(last_P_idx_2), sizeof(last_U_idx_2), sizeof(last_K_2));
    printf(" (active S-box bias: %f)\n", 8.0 * 4 * 4 * 4 * 4 / (16 * 16 * 16 * 16));
    temp = dec_to_bin(last_maxIdx_2, 8);
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 4; i++){
            k5[4 * last_K_2[j] + i] = temp[4 * j + i];
        }
    }
    free(temp);

    // Last round's key K5
    printf(">> Last round's key:     ");
    show_bits(k5, 16);
    puts("");

    // First round's key 1..4, 9..12
    BIT first_C_idx[] = {9, 10, 11};
    BIT first_U_idx[] = {0, 1, 8, 9};
    BIT first_K[]     = {0, 2};
    printf("K1,1..4 / K1,9..12   ->  ");
    int first_maxIdx = get_partial_subkey_2(first_C_idx, first_U_idx, first_K, sizeof(first_C_idx), sizeof(first_U_idx), sizeof(first_K));
    printf(" (active S-box bias: %f)\n", 8.0 * 4 * 4 * 4 * 6 / (16 * 16 * 16 * 16));
    temp = dec_to_bin(first_maxIdx, 8);
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 4; i++){
            k1[4 * first_K[j] + i] = temp[4 * j + i];
        }
    }
    free(temp);

    // First round's key 5..8, 13..16
    BIT first_C_idx_2[] = {10, 11};
    BIT first_U_idx_2[] = {4, 5, 12, 13};
    BIT first_K_2[]     = {1, 3};
    printf("K1,5..8 / K1,13..16  ->  ");
    int first_maxIdx_2 = get_partial_subkey_2(first_C_idx_2, first_U_idx_2, first_K_2, sizeof(first_C_idx_2), sizeof(first_U_idx_2), sizeof(first_K_2));
    printf(" (active S-box bias: %f)\n", 16.0 * 4 * 4 * 4 * 4 * 4 / (16 * 16 * 16 * 16 * 16));
    temp = dec_to_bin(first_maxIdx_2, 8);
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 4; i++){
            k1[4 * first_K_2[j] + i] = temp[4 * j + i];
        }
    }
    free(temp);

    // First round's key K1
    printf(">> First round's key:    ");
    show_bits(k1, 16);
}
