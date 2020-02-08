#define KEY_LEN         80
#define TEXT_LEN        16
#define SUBKEY_LEN      16
#define S_BOX_NUM       4
#define PAIR_NUM        10000
#define ROUND_NUM       4
#define BIT             char
#define DEBUG           0
#define RS_DEBUG        0

// Permutation table
int p_table[]           = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

// S-Box table
int s_box[]             = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

// Inverse S-Box table
int inv_s_box[]         = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};

// Fixed key (0x0, 0x1, 0x2, ..., 0xF, 0x0, 0x1, 0x2, 0x3)
BIT key[KEY_LEN]        = {0,0,0,1, 0,0,1,1, 0,1,0,1, 0,1,1,1, 0,1,0,0, 0,1,0,1, 0,1,1,0, 0,1,1,1,
                           1,0,0,0, 1,0,0,1, 1,0,1,0, 1,0,1,1, 1,1,0,0, 1,1,0,1, 1,1,1,0, 1,1,1,1,
                           1,0,0,0, 0,1,0,1, 0,1,0,0, 0,1,1,1};