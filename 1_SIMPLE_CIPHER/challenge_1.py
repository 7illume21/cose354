# Find the key and decrypt the following ciphertext 
# which is encrypted by shift cipher with the key.
# Encryption scheme: shift cipher
# Ciphertext: RGNEIDVGPEWNXHXCITGTHIXCV

def caesar_decrypt(cipher_text):
	result = []
	for i in range(1, 26):
		plain_text = ""
		for cipher_t in cipher_text.upper():
			cipher_t_ascii = ord(cipher_t) - ord('A') # 0 ~ 25
			plain_text += chr(ord('A') + (cipher_t_ascii - i) % 26)
		result.append({"KEY": i, "PLAIN_TEXT": plain_text})
	return result

c_text = "RGNEIDVGPEWNXHXCITGTHIXCV"
decrypt_result = caesar_decrypt(c_text)
for rs in decrypt_result:
	print(rs)

# {'KEY': 15, 'PLAIN_TEXT': 'CRYPTOGRAPHYISINTERESTING'}