from Crypto.Util import number
import random

# wrapper function used to encrypt plaintext
def rsa_encrypt(plain, e, n):
	cipher_tmp = []
	for char in plain:
		cipher_tmp.append(pow(ord(char), e, n))
	return cipher_tmp

# wrapper function used to decrypt ciphertext
def rsa_decrypt(cipher, d, n):
	plain_tmp = []
	for char in cipher:
		plain_tmp.append(chr(pow(char, d, n)))
	return "".join(plain_tmp)

# print encrypted cipher text
def print_cipher(cipher):
	print("Encrypted message: ", end = "")
	for char in cipher:
		print(str(char), end = "")
	print()

# extended euclidean algorithm used to find d
def multiplicative_inv(e, phi):
	tmp = e % phi;
	for i in range(1, phi):
		if((tmp * i) % phi == 1):
			return i
	return 1

def main():
	msg = input("Message to be encrypted: ")

	# Get key pair values
	p = number.getPrime(14)#random.randint(0, 2048)) # random prime number
	q = number.getPrime(14)#random.randint(0, 2048)) # random prime number
	n = p * q # value used for key pairs
	e = 65537 # exponent
	phi = (p - 1) * (q - 1) # totient of phi(n)
	d = multiplicative_inv(e, phi) # private value used for key pairs

	# Public Key Pair: (e, n)
	# Private Key Pair: (d, n)

	# print values used to calculate keys
	print("p   = ", p)
	print("q   = ", q)
	print("n   = ", n)
	print("e   = ", e)
	print("phi = ", phi)
	print("d   = ", d)
	print("--------------------------------------------------")

	#encrypt and decrypt input message
	encrypted = rsa_encrypt(msg, e, n)
	print_cipher(encrypted)
	print("Decrypted message: ", rsa_decrypt(encrypted, d, n))


if __name__ == '__main__':
	main()