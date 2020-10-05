# Vigenere Cipher

## DESCRIPTION
This project involved deciphering ciphertexts encrypted using the Vigenere cipher algorithm described here. This is a slightly more complicated substitution encryption algorithm that replaces letters based on a key phrase. Each letter in the key phrase essentially acts as a caesar cipher key shift value in which letters are encoded accordingly. 3 encrypted test files can be found in the encrypted directory.

For example, if a user wants to encrypt the phrase “ATTACKATDAWN” with the password “LEMON”, the user would look at the row with L from LEMON and the column with A from ATTACK. This gives the value of L for the cipher text. Then the user goes to row E, column T for the next letter and so on. When the user reaches the end of the keyphrase, it simply repeats itself and starts at L again. Repeating this process gives a ciphertext of “LXFOPVEFRNHR”.


## INSTRUCTIONS
There are no additional libraries needed to run this program. To run enter the following into a terminal:

python3 vigenere.py [file to be decrypted]
