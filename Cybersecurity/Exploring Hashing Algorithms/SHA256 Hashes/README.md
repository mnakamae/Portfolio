# SHA256 Hashes

## Description
The SHA2 (Secure Hash Algorithm 2) is a fairly well-known hashing algorithm. This implementation focuses on an implementation that uses 8-bit words to 50-bit words. The upper bound of 50-bits was arbitrarily chosen, as implementing the full 256-bit iteration would have taken too long to compute.

This project specifically looked at collisions using the PyCrypto library. A word was randomly generated, hashed, and entered into a Python dictionary. Then, the program continued randomly generating words, hashing them, and comparing them to the original dictionary entry to see if a collision occurred. Generally speaking, larger bit-words had longer runtimes, since they took longer to find a collision in a dictionary entry from the hash. This also correlated to more dictionary entries as well. The data was then entered into an Excel Spreadsheet using the xlwt library.


## Instructions
Note the PyCrypto and xlwt libraries were used in the implementation of this solution. To run enter the following into a terminal:

python3 SHA256.py
