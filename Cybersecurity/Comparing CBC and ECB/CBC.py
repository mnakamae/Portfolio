#### HELPER FUNCTIONS ####
# Generic wrapper to f.write 
def writeToFile(fileName, transactionString):
    f = open(fileName, "a")  # Open File  
    f.write(transactionString)           
    f.close()
     
# Generic wrapper to read file contents
def readFile(fileNameString):
    with open(fileNameString, encoding="utf8") as fp:
        return fp.read()
        
# Generic wrapper to read file contents as binary
def readFileBinary(fileNameString):
    with open(fileNameString, 'rb') as fp:
        return fp.read()

# Generic wrapper to read specified number of bytes as binary
def readFileBytes(fileNameString, numBytes):
    with open (fileNameString, 'rb') as fp:
        return fp.read(numBytes)
         
# Generic wrapper to write bytes to a file
def writeToFileBytes(fileName, payload):
    f = open(fileName, "wb")  # Open File    
    f.write(payload)           
    f.close()

# Returns a random number
def getRandomNumber():
    return os.urandom(1)

# Returns a random value of specified size in bytes
def getRandomBytes(size):
    randomBytes = os.urandom(size)
    return randomBytes

# Generic wrapper for AES-128 ECB encrypting
def ecbEncrypt(fileContents, key = None):
    global globalKeyVariable
    fileContentsBytes = bytes(fileContents, 'utf-8')
    lengthFileContents = len(fileContentsBytes)
    if key == None:
        AES_KEY = getRandomBytes(16)
    else:
        AES_KEY = key

    ECB_CIPHER = AES.new(AES_KEY, AES.MODE_ECB)
    encrypt = ECB_CIPHER.encrypt(fileContentsBytes)
    encrypt = encrypt[:lengthFileContents]                 # remove padded bytes before writing to file
    return encrypt

#### CBC MAIN ####
def main():
    AES_KEY = getRandomBytes(16)
    CBC_CIPHER = AES.new(AES_KEY, AES.MODE_CBC)

    # read file contents
    fileToBeEncrypted = "cp-logo.bmp"
    fileContents = readFileBinary(fileToBeEncrypted)
    bmpFileHeader = readFileBytes(fileToBeEncrypted, 54)
    lengthFileContents = len(fileContents)

    # pad bytes before encrypting
    fileContents += getRandomBytes(16 - (lengthFileContents % 16))
    
    # encrypt padded file contents
    encrypt = CBC_CIPHER.encrypt(fileContents)

    # remove padded bytes before writing to file
    encrypt = encrypt[:lengthFileContents]

    encryptedFileName = fileToBeEncrypted + "_CBC.bmp"
    writeToFileBytes(encryptedFileName, encrypt)

    # now replace .bmp header
    f = open(encryptedFileName, 'rb')
    s = f.read()
    f.close()

    bmpFileHeaderEncrypted = readFileBytes(encryptedFileName, 54)
    s = s.replace(bmpFileHeaderEncrypted, bmpFileHeader)
    writeToFileBytes(encryptedFileName, s)
