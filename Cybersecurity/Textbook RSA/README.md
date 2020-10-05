# Vigenere Cipher

## DESCRIPTION
This project looks closely at RSA protocols and the relation between both public and private key pairs. The most notable equations for this portion of the assignment are the encryption [E(m) = me mod n] and decryption [m(c) = cd mod n] equations given public (e, n) and private (d, n) key pairs. I also explored malleability attacks in which attackers are allowed to create their own ciphertexts due to the mathematical properties of the encryption/decryption algorithms. As a result, it is very important to also use padding when using any RSA schemes. Both the public and private keys are derived from randomly generated prime numbers, p and q.


## INSTRUCTIONS
Note that the library PyCrypto was used to implement this solution. To run enter the following into a terminal:

python3 RSA.py

The program will then ask the user to enter a message to be encrypted, and will print the public key, private key, encrypted message, and resulting decrypted message using the private key.
