from number_theory import *

SAMPLE_TEST = True

def rsa_enc(e, n, M):
    # C = (M ** e) % n
    C = squareNmultiply(M, e, n)
    return C

def rsa_enc_2(e, n, M):
    # C = (M ** e) % n
    C = squareNmultiply_2(M, e, n)
    return C

def rsa_dec(d, n, C):
    # M = (C ** d) % n
    M = squareNmultiply(C, d, n)
    return M

def rsa_dec_2(d, n, C):
    # M = (C ** d) % n
    M = squareNmultiply_2(C, d, n)
    return M

def elgamal_xa_finder(a, q, YA):
	for x in range(2, q - 1):
		if (a ** x) % q == YA:
			return x

def elgamal_dec(Cs, q, XA):
	# Recover key K
	K = squareNmultiply(Cs[0], XA, q)
	# Computing M
	# 
	M = (Cs[1] * m_inverse(q, K)) % q
	return K, M

def elgamal_dec_2(Cs, q, XA):
	# Recover key K
	K = squareNmultiply_2(Cs[0], XA, q)
	# Computing M
	# 
	M = (Cs[1] * m_inverse(q, K)) % q
	return K, M

# PROBLEM1: RSA
def problem1():
	# <RSA>
	# 
	# You have to implement 
	#  - EXTENDED EUCLIDEAN ALGORITHM 
	#  - SQUARE-AND-MULTIPLY ALGORITHM
	
	# Plaintext-Ciphertext pairs
	samples = [(8835383948117812667, 528567365900595529), (852845877651, 8792215503885098117)]

	n = 9943237852845877651		# base-64bit
	e = 13						# receiver's public key
	C = 1220703125				# ciphertext

	# # Using Factorization code (take a while -> COMMENTED)
	# factors = primeFactorization(n)
	# p = factors[0]
	# q = factors[1]

	p = 2701212721
	q = 3681027331
	pi_n = (p - 1) * (q - 1)
	d = extended_euclid(pi_n, e)[2] % pi_n

	print("#1")
	print("[Problem1.1] d is {}".format(d))

	# RSA decrypt using Square & Multiply Algorithm
	if SAMPLE_TEST:
		print("\t[Problem1.Sample1]               Decrypt({}) = {}".format(samples[0][1], rsa_dec(d, n, samples[0][1])))
		print("\t[Problem1.Sample1 - Slide based] Decrypt({}) = {}".format(samples[0][1], rsa_dec_2(d, n, samples[0][1])))

		print("\t[Problem1.Sample2]               Decrypt({}) = {}".format(samples[1][1], rsa_dec(d, n, samples[1][1])))
		print("\t[Problem1.Sample2 - Slide based] Decrypt({}) = {}".format(samples[1][1], rsa_dec_2(d, n, samples[1][1])))

	print()
	print("[Problem1.2]             Decrypt({}) = {}".format(C, rsa_dec(d, n, C)))
	print("[Problem1.2 - Slide based] Decrypt({}) = {}".format(C, rsa_dec_2(d, n, C)))
	
# PROBLEM2: ELGAMAL
def problem2():
	# <ElGamal>
	# 
	# You have to implement 
	#  - EXTENDED EUCLIDEAN ALGORITHM 
	#  - SQUARE-AND-MULTIPLY ALGORITHM
	
	# Plaintext-Ciphertext pairs
	samples = [(189465461, [2909170161, 1004005362]), (848963461,[2909170161, 2081016632])]

	q = 2934201397				# GF(2934201397)-32bits
	a = 37						# primitive root of q
	YA = 2174919958				# receiver's public key
	C1 = 2909170161				# ciphertext
	C2 = 2565161545	

	# Get XA
	XA = elgamal_xa_finder(a, q, YA)
	print("#2")
	print("[Problem2.1] XA is {}".format(XA))
	
	# ElGamal decrypt using Square & Multiply Algorithm
	if SAMPLE_TEST:
		print("\t[Problem2.Sample1]               Decrypt({}) = {}".format(samples[0][1], elgamal_dec(samples[0][1], q, XA)[1]))
		print("\t[Problem2.Sample1 - Slide based] Decrypt({}) = {}".format(samples[0][1], elgamal_dec_2(samples[0][1], q, XA)[1]))

		print("\t[PRoblem2.Sample2]               Decrypt({}) = {}".format(samples[1][1], elgamal_dec(samples[1][1], q, XA)[1]))
		print("\t[Problem2.Sample2 - Slide based] Decrypt({}) = {}".format(samples[1][1], elgamal_dec_2(samples[1][1], q, XA)[1]))

	print()
	print("[Problem2.2]               Decrypt({}) = {}".format([C1, C2], elgamal_dec([C1, C2], q, XA)[1]))
	print("[Problem2.2 - Slide based] Decrypt({}) = {}".format([C1, C2], elgamal_dec_2([C1, C2], q, XA)[1]))


if __name__ == '__main__':
	problem1()
	print()
	print("=" * 90)
	problem2()
	print()