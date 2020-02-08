import math

# Multiplicative Inverse of b in mod a
def m_inverse(a, b):
	return extended_euclid(a, b)[2]

# Extended Euclidean Algorithm
def extended_euclid(a, b):
    if b == 0:
        return a, 1, 0
    d1, x1, y1 = extended_euclid(b, a % b)
    return d1, y1, x1 - (a // b) * y1

# Square & Multiply Algorithm
# Compute (base ** degree) % mod
# Using Binary form of degree
# 	ex) (11 ** 23 % 187) 
# 		= (11 ** 1 % 187)(11 ** 2 % 187)(11 ** 4 % 187)(11 ** 16 % 187)
def squareNmultiply(base, degree, mod):
	result = 1
	degree_bin = bin(degree)[2:]
	for d in degree_bin:
		result = (result * result) % mod
		if d == "1":
			result = (result * base) % mod
	return result


# Square & Multiply Algorithm
# Compute (base ** degree) % mod
# With reference to ch09 slide p24
# 	ex) (11 ** 23 % 187) 
# 		= (11 ** 1 % 187)(11 ** 2 % 187)(11 ** 4 % 187)(11 ** 8 % 187)(11 ** 8 % 187)
def squareNmultiply_2(base, degree, mod):
	result = 1
	d = 1
	nList = []

	nItem = (base ** d) % mod
	nList.append((d, nItem))
	remained = degree - 1

	while True:
		if remained < d * 2:
			break
		nItem = (nItem * nItem) % mod
		nList.append((d * 2, nItem))
		remained -= d
		d *= 2

	tmp = 0
	for i in range(len(nList) - 1, -1, -1):
		# print(i, nList[i])
		while True:
			degree -= nList[i][0]
			if degree < 0:
				degree += nList[i][0]
				break
			result = (result * nList[i][1]) % mod
			# print(degree, result)
	# print(result)
	return result


# composite number n to two prime numbers p, q s.t. n = p x q
def primeFactorization(n):
	number = n
	factors = []
	for i in range(int(math.sqrt(n)), 2, -1):
		if number % i == 0:
			factors.append(i)
			number /= i
			factors.append(number)
			break
	return factors