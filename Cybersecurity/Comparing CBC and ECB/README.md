# CBC vs. ECB

## DESCRIPTION
The goal of this project was to further understand symmetric key cryptography by further exploring the Electronic Codebook (ECB) and Cipher Block Chaining (CBC) methods. The results showed that ECB does not handle encrypting patterns well since all blocks are encrypted with the same key. For this reason, the output .bmp file still had defining features similar to the input .bmp file. On the other hand, CBC solves this issue by utilizing the previous block’s encryption result as the key for the next block’s encryption. As a result, no discernible patterns can be seen in the output .bmp file.


## INSTRUCTIONS
Note that the AES library from PyCryptodome was used to implement these solutions. To run either program, enter the following into a terminal:

python3 ECB.py [file to be encrypted]

### -- OR --

python3 CBC.py [file to be encrypted]
