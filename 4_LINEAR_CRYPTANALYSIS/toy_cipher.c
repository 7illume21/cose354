/*
    [Objective]         1. Implementing toy cipher in C language
                            - Write comments for the important part of source code
                        2. Make 10,000 pairs of plaintext and ciphertext given the key
                            - key: (0x0, 0x1, 0x2, ..., 0xF, 0x0, 0x1, 0x2, 0x3)

    [Specification]     Key Size:   80 bit (10 byte)    
                        Block Size: 16 bit (2 byte)     // INPUT
                        Round #:    4
                        Subkey #:   5 
                            - derived from the 80 bit key
                            - subkey K_i = x_(16 * i - 15) .. x_(16 * i)

                        Subkey Mixing:  Mi = Pi XOR K1i


    [4 bit S-box]       [E, 4, D, 1, 2, F, B, 8, 3, A, 6, C, 5, 9, 0, 7]
    [Permutation]       [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "toy_cipher.h"

// For debugging, show bits
void show_bits(BIT bits[], int len){
    for (int i = 0; i < len; i++){
        if (i % 4 == 0){
            printf("/");
        }
        printf("%d ", bits[i]);
    }
    printf("\n");
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

// Bit XOR
// return mallocated BIT* val
BIT* bit_xor(BIT a[], BIT b[], int len){
    BIT* output = (BIT*) malloc(sizeof(BIT) * len);
    for(int i = 0; i < len; i++){
        output[i] = a[i] ^ b[i];
    }   
    return output;
}

// 'bits' as 16 bit input P
// 'key' as 80 bit key K
// 'round_num' : 0, 1, 2, 3
// return bits
BIT* subkey_mix(BIT bits[], BIT key[], int round_num){
    BIT* out;
    BIT subkey[SUBKEY_LEN];
    for(int i = 0; i < SUBKEY_LEN; i++){
        subkey[i] = key[round_num * SUBKEY_LEN + i];
    }    
    out = bit_xor(bits, subkey, TEXT_LEN);
    for(int i = 0; i < TEXT_LEN; i++){
        bits[i] = out[i];
    }
    free(out);
    return bits;
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

// Permute "bits" using 'p_table' 
BIT* permute(BIT bits[TEXT_LEN]){
    BIT temp[TEXT_LEN];
    for(int i = 0; i < TEXT_LEN; i++){
        temp[i] = bits[i];
    }
    for(int i = 0; i < TEXT_LEN; i++){
        bits[p_table[i] - 1] = temp[i]; 
    }
    return bits;
}

// Make count random numbers (len(random_nums) = count)
void make_random_num(int random_nums[], int count){
    for(int i = 0; i < count; i++){ 
        while(1){
            int flag = 1;
            int r = (double)rand() / RAND_MAX * 65535; // 0x0000 ~ 0xFFFF
            for(int j = 0; j < i; j++){
                // if r is in the array
                if(random_nums[j] == r){
                    flag = 0;
                } 
            }
            // if r is not in the array
            if(flag == 1){
                random_nums[i] = r;
                break;
            }
        }
    }
    // FILE* fp = fopen("random_numbers.txt", "w");
    // for(int i = 0; i < count; i++){
    //     fprintf(fp, "%d, ", random_nums[i]);   
    // }
}

// Toy Cipher func
int toy_cipher(BIT p_text[], BIT c_text[], BIT k[]){
    BIT input[TEXT_LEN];
    for (int i = 0; i < TEXT_LEN; i++){
        input[i] = p_text[i];
    }

    for(int i = 0; i < ROUND_NUM; i++){
        if(DEBUG){
            printf("--------------------------------------------------------------------\n");
            printf("[*] round %d input: \t\t", i + 1);
            show_bits(input, TEXT_LEN);
        }

        // Subkey K mixing
        subkey_mix(input, k, i);
        if(DEBUG){
            printf("[D] After subkey_mix: \t");
            show_bits(input, TEXT_LEN);
        }

        // Substitute
        substitute(input);
        if(DEBUG){
            printf("[D] After substitute: \t");
            show_bits(input, TEXT_LEN);
        }

        // Permute using p_table except Last round
        if(i < ROUND_NUM - 1){
            permute(input);
        }
        if(DEBUG && i < ROUND_NUM - 1){
            printf("[D] After permute: \t\t");
            show_bits(input, TEXT_LEN);
        }
    }

    // Additional Subkey K mixing
    subkey_mix(input, k, 4);
    if(DEBUG){
        printf("--------------------------------------------------------------------\n");
        printf("[D] After subkey_mix: \t");
        show_bits(input, TEXT_LEN);
    }
    for (int i = 0; i < TEXT_LEN; i++){
        c_text[i] = input[i];
    }
    return 1;
}

// Main func
int main(){
    // [Objective] Make 10,000 pairs of plaintext and ciphertext given the key
    FILE* fp = fopen("pc_pairs_random.txt", "w");

    int random[PAIR_NUM];
    srand((int)time(NULL));
    
    // Make PAIR_NUM random numbers
    make_random_num(random, PAIR_NUM);

    // Make PAIR_NUM plaintext-ciphertext pairs
    for(int j = 0; j < PAIR_NUM; j++){
        if(DEBUG){
            printf("[random[%d]]\n", j);
        }
        BIT* plain_text = dec_to_bin(random[j], TEXT_LEN);
        BIT cipher_text[TEXT_LEN];
        toy_cipher(plain_text, cipher_text, key);
        
        for(int i = 0; i < TEXT_LEN; i++){  
            fprintf(fp, "%d", plain_text[i]);   
        }
        fputs(" ", fp);
        for(int i = 0; i < TEXT_LEN; i++){  
            fprintf(fp, "%d", cipher_text[i]); 
        }
        fputs("\n", fp);

        if(RS_DEBUG){
            printf("====================================================================\n");
            printf("[+] <plaintext>  \t\t");
            show_bits(plain_text, TEXT_LEN);

            printf("[+] <ciphertext> \t\t");
            show_bits(cipher_text, TEXT_LEN);
            printf("====================================================================\n\n");
        }
        free(plain_text);
    }
    fclose(fp); 
}