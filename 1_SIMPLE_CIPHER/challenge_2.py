# Challenge 2 (hard)
# Find the key with a length of 3 and decrpyt the following ciphertext 
# which is encrypted the Vigenere cipher with the key.
# Encryption scheme: Vigenere cipher
# Ciphertext: BSIHCJJHLNBCTFJFHFTBPJQRWWQD
# (Hint: the plaintext starts with 'W')

def find_key(cipher, plain):
	for i in range(0, 26):
		if plain.upper() == chr(ord("A") + (ord(cipher.upper()) - ord("A") - i) % 26):
			return chr(ord("A") + i)

def vigenere_decrypt(cipher_text, key_list):
	result = []
	for key in key_list:
		i = 0
		plain_text = ""
		for cipher_t in cipher_text.upper():
			plain_text += chr(ord("A") + (ord(cipher_t) - ord(key[i % len(key)])) % 26)
			i += 1
		result.append({"KEY": key, "PLAIN_TEXT": plain_text})
	return result

c_text = "BSIHCJJHLNBCTFJFHFTBPJQRWWQD"
key_list = [find_key(c_text[0], "W") + chr(x) + chr(y) \
				for x in range(ord('A'), ord('Z') + 1) \
				for y in range(ord('A'), ord('Z') + 1)]

decrypt_result = vigenere_decrypt(c_text, key_list)
for rs in decrypt_result:
	print(rs)

# {'KEY': 'FOX', 'PLAIN_TEXT': 'WELCOMETOINFORMATIONSECURITY'}
