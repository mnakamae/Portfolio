# BCrypt Hashes

## Description
bcrypt is a hashing algorithm often used on passwords or other highly confidential information. This algorithm is used because it takes very long to break given the one way nature of the computations. Additionally, when salts are added on to the passwords, they become even more complex to break.

This project takes a given set of user/encrypted password combinations, and decrypts the passwords by brute forcing the correct guess using the PyCrypto library. To reiterate how difficult these computations are, the overall runtime to break all the passwords was approximately 31.4 hours and took nearly 1.05 million total computations. The user/password combinations are given in shadow.txt.


## Instructions
Note that the PyCrypto library was used in the implementation of these solutions. To run enter the following into a terminal:

python3 crack_passwords.py shadow.txt
