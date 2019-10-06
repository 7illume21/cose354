import des_table

def dec_to_n_bit_bin(hex_str, bin_size):
    return [int(x) for x in bin(int(hex_str, 10))[2:].zfill(bin_size)]

def hex_to_n_bit_bin(hex_str, bin_size):
    return [int(x) for x in bin(int(hex_str, 16))[2:].zfill(bin_size)]

def n_bit_bin_to_hex(bin_str, hex_size):
    return hex(int(bin_str, 2))[2:].zfill(hex_size)

def list_split(bin_list):
    return bin_list[:len(bin_list)//2], bin_list[len(bin_list)//2:]

def view_bin(bin_list, n):
    i = 0
    for b in bin_list:
        if (i % n == 0):
            if i == 0:
                print("\t", end='')
            else:
                print("\n\t", end='')
        print(b, end=', ')
        i += 1  
    print("\n")
        

class des:
    def __init__(self, key, plain_text, round_num):
        self.key = hex_to_n_bit_bin(key, 64)
        self.subkey = ""
        self.plain_text = hex_to_n_bit_bin(plain_text, 64)
        self.cipher_text = ""
        self.round_num = round_num

    def permute(self, block, table):
        return [block[x - 1] for x in table]

    def expand(self, block, table):
        return [block[x - 1] for x in table]

    def xor(self, t1, t2):
        return [x^y for x, y in zip(t1, t2)]

    def left_shift(self, block, n):
        return block[n:] + block[0:n]

    def substitute(self, block, table):
        result = []
        print("<S-box> (row, col, val)")
        for i in range(8):
            line = "".join(str(x) for x in block[i * 6 :i * 6 + 6])
            row = int(line[0] + line[5], 2)
            col = int(line[1:5], 2)
            print("\t{0}, {1} - {2}".format(row, col, table[i][row][col]))
            result += dec_to_n_bit_bin(str(table[i][row][col]), 4)
        print()
        return result    

    def F(self, r_block, key):
        R = self.expand(r_block, des_table.E)
        tmp1 = self.xor(R, key)
        tmp2 = self.substitute(tmp1, des_table.s_box)
        print("<F(R0, K1)>")
        view_bin(self.permute(tmp2, des_table.P), 8)
        return self.permute(tmp2, des_table.P)


    def one_round(self):
        La, Ra = list_split(self.cipher_text)
        Lb = Ra
        Rb = self.xor(La, self.F(Ra, self.subkey))
        return Lb + Rb

    def run(self):
        # p_text init permute
        self.cipher_text = self.permute(self.plain_text, des_table.Init_P)
        print("<Plaintext - Initial Permutation>")
        view_bin(self.cipher_text, 8)

        # k pc 1
        self.key = self.permute(self.key, des_table.PC1)
        print("<Key - Permuted Choice 1>")
        view_bin(self.key, 7)

        # round loop
        for i in range(self.round_num):
            print("-" * 50)
            print("[Round {0}]".format(i + 1), end="\n\n")

            # Left circular Shift
            shift_num = des_table.SHIFT[i]
            C0, D0 = list_split(self.key)
            self.key = self.left_shift(C0, shift_num) + self.left_shift(D0, shift_num)
            print("<Key - Left circular shift>")
            view_bin(self.key, 7)

            # k pc 2
            self.subkey = self.permute(self.key, des_table.PC2)
            print("<Key - Permutation Choice 2>")
            view_bin(self.subkey, 6)
            
            print("<Subkey - K{0}>".format(i + 1), end="\n")
            print("\tK{0}:".format(i + 1), n_bit_bin_to_hex("".join(str(x) for x in self.subkey), 8).upper(), end="\n\n")
            
            # one round
            self.cipher_text = self.one_round()
            print("<L{0}, R{0}>".format(i + 1))
            L, R = list_split(self.cipher_text)
            print("\tL{0}:".format(i + 1), (n_bit_bin_to_hex("".join(str(x) for x in L), 8).upper()), end="\n")
            print("\tR{0}:".format(i + 1), (n_bit_bin_to_hex("".join(str(x) for x in R), 8).upper()), end="\n\n")
            print("-" * 50)

        # p_text 32 bit swap
        Ly, Ry = list_split(self.cipher_text)
        self.cipher_text = Ry + Ly
        print("<32-bit Swap>")
        view_bin(self.cipher_text, 8)

        # p_text inverse permute
        self.cipher_text = self.permute(self.cipher_text, des_table.Init_P_INV)
        print("<Inverse Initial Permutation>")
        view_bin(self.cipher_text, 8)       

        print("=" * 50)
        return n_bit_bin_to_hex("".join(str(x) for x in self.cipher_text), 8)


if __name__ == '__main__':
    k = "0123456789ABCDEF"
    p = "FEDCBA9876543210"
    # k = "0f1571c947d9e859"
    # p = "02468aceeca86420"


    my_des = des(k, p, 1)
    # my_des = des(k, p, 16)
    e = my_des.run()
    print("<<Encrypt Result>>", e.upper())